/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0

Based on: screencast-portal.c
 *
 * Copyright 2022 Georges Basile Stavracas Neto <georges.stavracas@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "framework.h"
#include "WineCap.h"
#include "pipewire.h"
#include "portal.h"

#include <spa/utils/defs.h>
#include <gio/gunixfdlist.h>

enum portal_capture_type
{
	PORTAL_CAPTURE_TYPE_MONITOR = 1 << 0,
	PORTAL_CAPTURE_TYPE_WINDOW = 1 << 1,
	PORTAL_CAPTURE_TYPE_VIRTUAL = 1 << 2,
};

enum portal_cursor_mode
{
	PORTAL_CURSOR_MODE_HIDDEN = 1 << 0,
	PORTAL_CURSOR_MODE_EMBEDDED = 1 << 1,
	PORTAL_CURSOR_MODE_METADATA = 1 << 2,
};

struct
{
	GDBusConnection *connection;
	GDBusProxy *screencast_proxy;
	GMainLoop *main_loop;
	GCancellable *cancellable;
	char *session_handle;
	uint32_t pipewire_node;
	struct pw_loop *pw_loop;
	struct pw_context *pw_ctx;
	int type;
	bool cursor;
	PIPEWIRE_CALLBACK_FUNC callback;
	bool error;
} sc;

static uint32_t get_available_capture_types()
{
	g_autoptr(GVariant) cached_source_types = NULL;
	uint32_t available_source_types;
	cached_source_types = g_dbus_proxy_get_cached_property(sc.screencast_proxy, "AvailableSourceTypes");
	available_source_types = cached_source_types ? g_variant_get_uint32(cached_source_types) : 0;
	return available_source_types;
}

static uint32_t get_available_cursor_modes()
{
	g_autoptr(GVariant) cached_cursor_modes = NULL;
	uint32_t available_cursor_modes;
	cached_cursor_modes = g_dbus_proxy_get_cached_property(sc.screencast_proxy, "AvailableCursorModes");
	available_cursor_modes = cached_cursor_modes ? g_variant_get_uint32(cached_cursor_modes) : 0;
	return available_cursor_modes;
}

static void on_pipewire_remote_opened_cb(GObject *source, GAsyncResult *res, void *user_data)
{
	g_autoptr(GUnixFDList) fd_list = NULL;
	g_autoptr(GVariant) result = NULL;
	g_autoptr(GError) error = NULL;
	int pipewire_fd;
	int fd_index;

	debug("on_pipewire_remote_opened_cb");

	result = g_dbus_proxy_call_with_unix_fd_list_finish(G_DBUS_PROXY(source), &fd_list, res, &error);
	if (error)
	{
		if (!g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
		{
			warn("Error retrieving pipewire fd: %s", error->message);
			sc.error = true;
		}
		return;
	}

	g_variant_get(result, "(h)", &fd_index, &error);

	pipewire_fd = g_unix_fd_list_get(fd_list, fd_index, &error);
	if (error)
	{
		if (!g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
		{
			warn("Error retrieving pipewire fd: %s", error->message);
			sc.error = true;
		}
		return;
	}

	if (pipewire_start(sc.pw_loop, sc.pw_ctx, pipewire_fd, sc.pipewire_node, sc.callback) < 0)
	{
		warn("Error starting PipeWire");
		sc.error = true;
	}
}

static void open_pipewire_remote()
{
	GVariantBuilder builder;
	g_variant_builder_init(&builder, G_VARIANT_TYPE_VARDICT);
	g_dbus_proxy_call_with_unix_fd_list(sc.screencast_proxy, "OpenPipeWireRemote",
										g_variant_new("(oa{sv})", sc.session_handle, &builder),
										G_DBUS_CALL_FLAGS_NONE, -1, NULL, sc.cancellable,
										on_pipewire_remote_opened_cb, NULL);
}

static void on_start_response_received_cb(GVariant *parameters, void *user_data)
{
	g_autoptr(GVariant) stream_properties = NULL;
	g_autoptr(GVariant) streams = NULL;
	g_autoptr(GVariant) result = NULL;
	GVariantIter iter;
	uint32_t response;
	unsigned n_streams;

	g_variant_get(parameters, "(u@a{sv})", &response, &result);

	if (response != 0)
	{
		warn("Failed to start ScreenCast, denied or cancelled by user");
		sc.error = true;
		return;
	}

	streams = g_variant_lookup_value(result, "streams", G_VARIANT_TYPE_ARRAY);
	g_variant_iter_init(&iter, streams);
	n_streams = g_variant_iter_n_children(&iter);
	if (n_streams != 1)
	{
		warn("Received more than one stream when only one was expected. "
			 "This is probably a bug in the desktop portal implementation you are "
			 "using.");

		// The KDE Desktop portal implementation sometimes sends an invalid
		// response where more than one stream is attached, and only the
		// last one is the one we're looking for. This is the only known
		// buggy implementation, so let's at least try to make it work here.
		for (unsigned i = 0; i < n_streams - 1; i++)
		{
			g_autoptr(GVariant) throwaway_properties = NULL;
			uint32_t throwaway_pipewire_node;

			g_variant_iter_loop(&iter, "(u@a{sv})", &throwaway_pipewire_node, &throwaway_properties);
		}
	}

	g_variant_iter_loop(&iter, "(u@a{sv})", &sc.pipewire_node, &stream_properties);
	info("Source selected, setting up ScreenCast");
	open_pipewire_remote();
}

static void on_started_cb(GObject *source, GAsyncResult *res, void *user_data)
{
	g_autoptr(GVariant) result = NULL;
	g_autoptr(GError) error = NULL;

	result = g_dbus_proxy_call_finish(G_DBUS_PROXY(source), res, &error);
	if (error)
	{
		if (!g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
		{
			warn("Error selecting ScreenCast source: %s", error->message);
			sc.error = true;
		}
	}
}

static void start_session()
{
	GVariantBuilder builder;
	char *request_token;
	char *request_path;
	portal_create_request_path(&request_path, &request_token);
	portal_signal_subscribe(request_path, sc.cancellable, on_start_response_received_cb, NULL);
	g_variant_builder_init(&builder, G_VARIANT_TYPE_VARDICT);
	g_variant_builder_add(&builder, "{sv}", "handle_token", g_variant_new_string(request_token));
	g_dbus_proxy_call(sc.screencast_proxy, "Start",
					  g_variant_new("(osa{sv})", sc.session_handle, "", &builder), G_DBUS_CALL_FLAGS_NONE, -1,
					  sc.cancellable, on_started_cb, NULL);
	free(request_token);
	free(request_path);
}

static void on_select_source_response_received_cb(GVariant *parameters, void *user_data)
{
	g_autoptr(GVariant) ret = NULL;
	uint32_t response;

	debug("Response to select source received");
	g_variant_get(parameters, "(u@a{sv})", &response, &ret);
	if (response != 0)
	{
		warn("Failed to select source, denied or cancelled by user");
		sc.error = true;
		return;
	}
	start_session();
}

static void on_source_selected_cb(GObject *source, GAsyncResult *res, void *user_data)
{
	g_autoptr(GVariant) result = NULL;
	g_autoptr(GError) error = NULL;
	result = g_dbus_proxy_call_finish(G_DBUS_PROXY(source), res, &error);
	if (error)
	{
		if (!g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
		{
			warn("Error selecting ScreenCast source: %s", error->message);
			sc.error = true;
		}
	}
}

static void select_source()
{
	GVariantBuilder builder;
	uint32_t available_cursor_modes;
	char *request_token;
	char *request_path;

	portal_create_request_path(&request_path, &request_token);
	portal_signal_subscribe(request_path, sc.cancellable, on_select_source_response_received_cb, NULL);
	g_variant_builder_init(&builder, G_VARIANT_TYPE_VARDICT);
	g_variant_builder_add(&builder, "{sv}", "types", g_variant_new_uint32(1 | 2 | 4));
	g_variant_builder_add(&builder, "{sv}", "multiple", g_variant_new_boolean(FALSE));
	g_variant_builder_add(&builder, "{sv}", "handle_token", g_variant_new_string(request_token));

	available_cursor_modes = get_available_cursor_modes();
	if ((available_cursor_modes & PORTAL_CURSOR_MODE_EMBEDDED))
		g_variant_builder_add(&builder, "{sv}", "cursor_mode", g_variant_new_uint32(PORTAL_CURSOR_MODE_EMBEDDED));
	else
		g_variant_builder_add(&builder, "{sv}", "cursor_mode", g_variant_new_uint32(PORTAL_CURSOR_MODE_HIDDEN));

	g_dbus_proxy_call(sc.screencast_proxy, "SelectSources",
					  g_variant_new("(oa{sv})", sc.session_handle, &builder), G_DBUS_CALL_FLAGS_NONE, -1,
					  sc.cancellable, on_source_selected_cb, NULL);
	free(request_token);
	free(request_path);
}

static void on_create_session_response_received_cb(GVariant *parameters, void *user_data)
{
	debug("on_create_session_response_received_cb");

	g_autoptr(GVariant) session_handle_variant = NULL;
	g_autoptr(GVariant) result = NULL;
	uint32_t response;
	g_variant_get(parameters, "(u@a{sv})", &response, &result);
	if (response != 0)
	{
		warn("Failed to create session, denied or cancelled by user");
		sc.error = true;
		return;
	}
	info("Screencast session created");
	session_handle_variant = g_variant_lookup_value(result, "session_handle", NULL);
	sc.session_handle = g_variant_dup_string(session_handle_variant, NULL);
	select_source();
}

static void on_session_created_cb(GObject *source, GAsyncResult *res, void *user_data)
{
	debug("on_session_created_cb");
	g_autoptr(GVariant) result = NULL;
	g_autoptr(GError) error = NULL;
	result = g_dbus_proxy_call_finish(G_DBUS_PROXY(source), res, &error);
	if (error)
	{
		if (!g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
		{
			warn("Error creating ScreenCast session: %s", error->message);
			sc.error = true;
		}
	}
}

static void create_session()
{
	debug("create_session");

	GVariantBuilder builder;
	char *session_token;
	char *request_token;
	char *request_path;

	portal_create_request_path(&request_path, &request_token);
	portal_create_session_path(NULL, &session_token);
	debug("request_path = %s", request_path);
	debug("request_token = %s", request_token);
	debug("session_token = %s", session_token);
	portal_signal_subscribe(request_path, sc.cancellable, on_create_session_response_received_cb, NULL);
	g_variant_builder_init(&builder, G_VARIANT_TYPE_VARDICT);
	g_variant_builder_add(&builder, "{sv}", "handle_token", g_variant_new_string(request_token));
	g_variant_builder_add(&builder, "{sv}", "session_handle_token", g_variant_new_string(session_token));
	g_dbus_proxy_call(sc.screencast_proxy, "CreateSession", g_variant_new("(a{sv})", &builder),
					  G_DBUS_CALL_FLAGS_NONE, -1, sc.cancellable, on_session_created_cb, NULL);
	free(session_token);
	free(request_token);
	free(request_path);
}

int screencast_init()
{
	spa_zero(sc);

	{
		g_autoptr(GError) error = NULL;
		sc.connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
		if (error)
		{
			warn("Error retrieving D-Bus connection: %s", error->message);
			sc.error = true;
			return -1;
		}
	}

	{
		g_autoptr(GError) error = NULL;
		sc.screencast_proxy = g_dbus_proxy_new_sync(portal_get_dbus_connection(), G_DBUS_PROXY_FLAGS_NONE, NULL,
													"org.freedesktop.portal.Desktop",
													"/org/freedesktop/portal/desktop",
													"org.freedesktop.portal.ScreenCast", NULL, &error);
		if (error)
		{
			warn("Error retrieving D-Bus proxy: %s", error->message);
			sc.error = true;
			return -1;
		}
	}

	uint32_t available_capture_types = get_available_capture_types();
	if (available_capture_types == 0)
	{
		warn("No capture sources");
		sc.error = true;
		return -1;
	}
	return 0;
}

typedef struct _PipeWireSource
{
	GSource base;
	struct pw_loop *loop;
} PipeWireSource;

static gboolean pipewire_loop_source_dispatch(GSource *source, GSourceFunc callback, gpointer user_data)
{
	PipeWireSource *s = (PipeWireSource *)source;
	int result;

	result = pw_loop_iterate(s->loop, 0);
	if (result < 0)
	{
		warn("pipewire_loop_iterate failed: %d", result);
	}
	return TRUE;
}

static GSourceFuncs pipewire_source_funcs = {
	.dispatch = pipewire_loop_source_dispatch,
};

void screencast_run()
{
	sc.main_loop = g_main_loop_new(NULL, FALSE);
	pw_init(NULL, NULL);
	sc.pw_loop = pw_loop_new(NULL);
	sc.pw_ctx = pw_context_new(sc.pw_loop, NULL, 0);
	PipeWireSource *source = (PipeWireSource *)g_source_new(&pipewire_source_funcs, sizeof(PipeWireSource));
	source->loop = sc.pw_loop;
	g_source_add_unix_fd(&source->base, pw_loop_get_fd(sc.pw_loop), G_IO_IN | G_IO_ERR);
	g_source_attach(&source->base, NULL);
	g_source_unref(&source->base);

	pw_loop_enter(sc.pw_loop);
	g_main_loop_run(sc.main_loop);
	debug("g_main_loop exit()");
	pw_loop_leave(sc.pw_loop);
	pw_loop_destroy(sc.pw_loop);
	g_main_loop_unref(sc.main_loop);
}

static int pw_start_proxy(gpointer data)
{
	create_session();
	return G_SOURCE_REMOVE;
}

void screencast_start(int type, bool cursor, PIPEWIRE_CALLBACK_FUNC callback)
{
	sc.cancellable = g_cancellable_new();
	sc.type = type;
	sc.cursor = cursor;
	sc.callback = callback;
	sc.error = false;

	g_idle_add(pw_start_proxy, NULL);
}

static int pw_stop_proxy(gpointer data)
{
	pipewire_stop();
	return G_SOURCE_REMOVE;
}

void screencast_stop()
{
	if (sc.session_handle)
	{
		g_dbus_connection_call(portal_get_dbus_connection(), "org.freedesktop.portal.Desktop",
							   sc.session_handle, "org.freedesktop.portal.Session", "Close", NULL, NULL,
							   G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL, NULL);

		g_clear_pointer(&sc.session_handle, g_free);
	}

	g_idle_add(pw_stop_proxy, NULL);
	g_cancellable_cancel(sc.cancellable);
	g_clear_object(&sc.cancellable);
}

void screencast_destroy()
{
	if (sc.main_loop)
		g_main_loop_quit(sc.main_loop);
}