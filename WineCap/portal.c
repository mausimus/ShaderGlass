/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0

Based on: portal.c
 *
 * Copyright 2021 Georges Basile Stavracas Neto <georges.stavracas@gmail.com>
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

struct portal_signal_call {
	GCancellable *cancellable;
	portal_signal_callback callback;
	gpointer user_data;
	char *request_path;
	guint signal_id;
	gulong cancelled_id;
};

#define REQUEST_PATH "/org/freedesktop/portal/desktop/request/%s/ShaderGlass%u"
#define SESSION_PATH "/org/freedesktop/portal/desktop/session/%s/ShaderGlass%u"
#define MAX_TOKEN 512

static GDBusConnection *connection = NULL;

static void ensure_connection(void)
{
	g_autoptr(GError) error = NULL;
	if (!connection) {
		connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);

		if (error) {
			warn("Error retrieving D-Bus connection: %s", error->message);
			return;
		}
	}
}

char *get_sender_name(void)
{
	char *sender_name;
	char *aux;
	ensure_connection();
	sender_name = strdup(g_dbus_connection_get_unique_name(connection) + 1);
	/* Replace dots by underscores */
	while ((aux = strstr(sender_name, ".")) != NULL)
		*aux = '_';
	return sender_name;
}

GDBusConnection *portal_get_dbus_connection(void)
{
	ensure_connection();
	return connection;
}

void portal_create_request_path(char **out_path, char **out_token)
{
	static uint32_t request_token_count = 0;
	request_token_count++;
	if (out_token) {
		*out_token = (char*)malloc(MAX_TOKEN);
		snprintf(*out_token, MAX_TOKEN, "ShaderGlass%u", request_token_count);
	}
	if (out_path) {
		char *sender_name;
		sender_name = get_sender_name();
		*out_path = (char*)malloc(MAX_TOKEN);
		snprintf(*out_path, MAX_TOKEN, REQUEST_PATH, sender_name, request_token_count);
		free(sender_name);
	}
}

void portal_create_session_path(char **out_path, char **out_token)
{
	static uint32_t session_token_count = 0;
	session_token_count++;
	if (out_token) {
		*out_token = (char*)malloc(256);
		snprintf(*out_token, 256, "ShaderGlass%u", session_token_count);
	}
	if (out_path) {
		char *sender_name;
		sender_name = get_sender_name();
		*out_path = (char*)malloc(256);
		snprintf(*out_path, 256, SESSION_PATH, sender_name, session_token_count);
		free(sender_name);
	}
}

static void portal_signal_call_free(struct portal_signal_call *call)
{
	if (call->signal_id)
		g_dbus_connection_signal_unsubscribe(portal_get_dbus_connection(), call->signal_id);
	if (call->cancelled_id > 0)
		g_signal_handler_disconnect(call->cancellable, call->cancelled_id);
	g_clear_pointer(&call->request_path, free);
	free(call);
}

static void on_cancelled_cb(GCancellable *cancellable, void *data)
{
	struct portal_signal_call *call = data;
	warn("Request cancelled");
	g_dbus_connection_call(portal_get_dbus_connection(), "org.freedesktop.portal.Desktop", call->request_path,
			       "org.freedesktop.portal.Request", "Close", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			       NULL, NULL);
	portal_signal_call_free(call);
}

static void on_response_received_cb(GDBusConnection *connection, const char *sender_name, const char *object_path,
				    const char *interface_name, const char *signal_name, GVariant *parameters,
				    void *user_data)
{
	struct portal_signal_call *call = user_data;

	if (call->callback)
		call->callback(parameters, call->user_data);

	portal_signal_call_free(call);
}

void portal_signal_subscribe(const char *path, GCancellable *cancellable, portal_signal_callback callback,
			     gpointer user_data)
{
	struct portal_signal_call *call;

	call = malloc(sizeof(struct portal_signal_call));
	call->request_path = strdup(path);
	call->callback = callback;
	call->user_data = user_data;
	call->cancellable = cancellable ? g_object_ref(cancellable) : NULL;
	call->cancelled_id = cancellable ? g_signal_connect(cancellable, "cancelled", G_CALLBACK(on_cancelled_cb), call)
					 : 0;
	call->signal_id = g_dbus_connection_signal_subscribe(
		portal_get_dbus_connection(), "org.freedesktop.portal.Desktop", "org.freedesktop.portal.Request",
		"Response", call->request_path, NULL, G_DBUS_SIGNAL_FLAGS_NO_MATCH_RULE, on_response_received_cb, call,
		NULL);
}