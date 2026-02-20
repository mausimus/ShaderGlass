/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0

Based on:
 * pipewire.c
 *
 * Copyright 2020 Georges Basile Stavracas Neto <georges.stavracas@gmail.com>
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

#include <fcntl.h>
#include <spa/param/video/format-utils.h>
#include <spa/debug/types.h>
#include <spa/param/video/type-info.h>
#include <spa/utils/defs.h>

struct data
{
  int pipewire_fd;
  int pipewire_node;
  PIPEWIRE_CALLBACK_FUNC callback;

  struct pw_loop *pw_loop;
  struct pw_context *pw_ctx;
  struct pw_core *pw_core;
  struct spa_hook core_listener;
  struct spa_hook stream_listener;
  int sync_id;
  struct pw_stream *stream;
  struct spa_video_info format;
  struct pw_stream_events events;
  struct pw_core_events core_events;
} data;

static void on_core_info_cb(void *user_data, const struct pw_core_info *info)
{
  debug("on_core_info_cb");
}

static void on_core_error_cb(void *user_data, uint32_t id, int seq, int res, const char *message)
{
  warn("Error id:%u seq:%d res:%d (%s): %s", id, seq, res, "", message);
}

static void on_core_done_cb(void *user_data, uint32_t id, int seq)
{
  debug("on_core_done_cb");
}

int pipewire_connect_fd()
{
  data.pw_core = pw_context_connect_fd(data.pw_ctx, fcntl(data.pipewire_fd, F_DUPFD_CLOEXEC, 5), NULL, 0);
  if (!data.pw_core)
  {
    warn("Error connecting to PipeWire FD");
    return -1;
  }

  spa_zero(data.core_events);
  data.core_events.version = PW_VERSION_CORE_EVENTS;
  data.core_events.info = on_core_info_cb;
  data.core_events.done = on_core_done_cb;
  data.core_events.error = on_core_error_cb;

  pw_core_add_listener(data.pw_core, &data.core_listener, &data.core_events, &data);
  data.sync_id = pw_core_sync(data.pw_core, PW_ID_CORE, data.sync_id);
  info("Connected to PipeWire FD");
  return 0;
}

static void on_process(void *userdata)
{
  struct data *data = userdata;
  struct pw_buffer *b;
  struct spa_buffer *buf;

  // debug("on_process");

  if (userdata == NULL)
  {
    warn("NULL userdata\n");
    abort();
  }

  if ((b = pw_stream_dequeue_buffer(data->stream)) == NULL)
  {
    warn("Out of PW buffers!");
    return;
  }

  buf = b->buffer;
  if (buf->datas[0].data == NULL)
    return;

  int width = data->format.info.raw.size.width;
  int height = data->format.info.raw.size.height;
  int stride = buf->datas[0].chunk->stride;

  struct spa_meta_region *region;
  region = spa_buffer_find_meta_data(buf, SPA_META_VideoCrop, sizeof(*region));
  if (region && spa_meta_region_is_valid(region) && (region->region.size.width != width || region->region.size.height != height))
  {
    debug("Got cropped frame: %dx%d at %dx%d (from %dx%d)", region->region.size.width, region->region.size.height,
          region->region.position.x, region->region.position.y, width, height);
    if (data->callback)
    {
      uint8_t *cropped_data = (uint8_t *)buf->datas[0].data;
      cropped_data += (region->region.position.x * 4) + (region->region.position.y * stride);
      data->callback(cropped_data, region->region.size.width, region->region.size.height, stride);
    }
  }
  else
  {
    debug("Got full frame: %dx%d", width, height);
    if (data->callback)
    {
      data->callback(buf->datas[0].data, width, height, stride);
    }
  }

  pw_stream_queue_buffer(data->stream, b);
}

static void on_param_changed(void *userdata, uint32_t id, const struct spa_pod *param)
{
  struct data *data = userdata;

  if (param == NULL || id != SPA_PARAM_Format)
    return;

  if (spa_format_parse(param,
                       &data->format.media_type,
                       &data->format.media_subtype) < 0)
    return;

  if (data->format.media_type != SPA_MEDIA_TYPE_video ||
      data->format.media_subtype != SPA_MEDIA_SUBTYPE_raw)
    return;

  if (spa_format_video_raw_parse(param, &data->format.info.raw) < 0)
    return;

  info("> format: %d (%s)", data->format.info.raw.format,
       spa_debug_type_find_name(spa_type_video_format,
                                data->format.info.raw.format));
  info("> size: %dx%d", data->format.info.raw.size.width,
       data->format.info.raw.size.height);
  info("> framerate: %d/%d", data->format.info.raw.framerate.num,
       data->format.info.raw.framerate.denom);

  struct spa_pod_builder pod_builder;
  const struct spa_pod *params[7];
  uint8_t params_buffer[1024];
  uint32_t n_params = 0;
  pod_builder = SPA_POD_BUILDER_INIT(params_buffer, sizeof(params_buffer));
  params[n_params++] = spa_pod_builder_add_object(&pod_builder, SPA_TYPE_OBJECT_ParamMeta, SPA_PARAM_Meta,
                                                  SPA_PARAM_META_type, SPA_POD_Id(SPA_META_VideoCrop),
                                                  SPA_PARAM_META_size,
                                                  SPA_POD_Int(sizeof(struct spa_meta_region)));
  pw_stream_update_params(data->stream, params, n_params);
}

static void on_state_changed(void *user_data, enum pw_stream_state old, enum pw_stream_state state,
                             const char *error)
{
  info("State: \"%s\" (error: %s)", pw_stream_state_as_string(state), error ? error : "none");
}

static void renegotiate_format(void *user_data, uint64_t expirations)
{
  info("Renegotiate format");

  const struct spa_pod *params[1];
  uint8_t buffer[4096];
  struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

  params[0] = spa_pod_builder_add_object(&b,
                                         SPA_TYPE_OBJECT_Format, SPA_PARAM_EnumFormat,
                                         SPA_FORMAT_mediaType, SPA_POD_Id(SPA_MEDIA_TYPE_video),
                                         SPA_FORMAT_mediaSubtype, SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
                                         SPA_FORMAT_VIDEO_format, SPA_POD_CHOICE_ENUM_Id(2, SPA_VIDEO_FORMAT_BGRx, SPA_VIDEO_FORMAT_BGRA),
                                         SPA_FORMAT_VIDEO_size, SPA_POD_CHOICE_RANGE_Rectangle(&SPA_RECTANGLE(320, 200), &SPA_RECTANGLE(1, 1), &SPA_RECTANGLE(4096, 4096)),
                                         SPA_FORMAT_VIDEO_framerate, SPA_POD_CHOICE_RANGE_Fraction(&SPA_FRACTION(60, 1), &SPA_FRACTION(0, 1), &SPA_FRACTION(1000, 1)));
  pw_stream_update_params(data.stream, params, 1);
}

int pipewire_connect_stream()
{
  struct pw_properties *props = pw_properties_new(PW_KEY_MEDIA_TYPE, "Video",
                                                  PW_KEY_MEDIA_CATEGORY, "Capture",
                                                  PW_KEY_MEDIA_ROLE, "Screen",
                                                  NULL);

  spa_zero(data.events);
  data.events.version = PW_VERSION_STREAM_EVENTS;
  data.events.param_changed = on_param_changed;
  data.events.state_changed = on_state_changed;
  data.events.process = on_process;

  pw_loop_add_event(data.pw_loop, renegotiate_format, &data);

  info("Creating stream...");
  data.stream = pw_stream_new(data.pw_core, "ShaderGlass Input", props);
  if (data.stream == NULL)
  {
    warn("Failed to create stream!");
    return -1;
  }

  pw_stream_add_listener(data.stream, &data.stream_listener, &data.events, &data);

  const struct spa_pod *params[1];
  uint8_t buffer[4096];
  struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

  params[0] = spa_pod_builder_add_object(&b,
                                         SPA_TYPE_OBJECT_Format, SPA_PARAM_EnumFormat,
                                         SPA_FORMAT_mediaType, SPA_POD_Id(SPA_MEDIA_TYPE_video),
                                         SPA_FORMAT_mediaSubtype, SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
                                         SPA_FORMAT_VIDEO_format, SPA_POD_CHOICE_ENUM_Id(2, SPA_VIDEO_FORMAT_BGRx, SPA_VIDEO_FORMAT_BGRA),
                                         SPA_FORMAT_VIDEO_size, SPA_POD_CHOICE_RANGE_Rectangle(&SPA_RECTANGLE(320, 200), &SPA_RECTANGLE(1, 1), &SPA_RECTANGLE(4096, 4096)),
                                         SPA_FORMAT_VIDEO_framerate, SPA_POD_CHOICE_RANGE_Fraction(&SPA_FRACTION(60, 1), &SPA_FRACTION(0, 1), &SPA_FRACTION(1000, 1)));

  info("Connecting to stream...");
  int res = pw_stream_connect(data.stream,
                              PW_DIRECTION_INPUT,
                              data.pipewire_node,
                              PW_STREAM_FLAG_AUTOCONNECT |
                                  PW_STREAM_FLAG_MAP_BUFFERS,
                              params, 1);
  if (res != 0)
  {
    warn("Unable to connect to stream: %d", res);
    return -1;
  }

  info("Connected to stream!");
  pw_stream_set_active(data.stream, true);
  return 0;
}

int pipewire_start(struct pw_loop *loop, struct pw_context *ctx, int pipewire_fd, int pipewire_node, PIPEWIRE_CALLBACK_FUNC callback)
{
  debug("pipewire_start");
  spa_zero(data);
  data.pw_loop = loop;
  data.pw_ctx = ctx;
  data.pipewire_fd = pipewire_fd;
  data.pipewire_node = pipewire_node;
  data.callback = callback;
  if (pipewire_connect_fd() < 0)
  {
    warn("Error connecting PipeWire FD");
    pipewire_stop();
    return -1;
  }
  if (pipewire_connect_stream() < 0)
  {
    warn("Error connecting PipeWire Stream");
    pipewire_stop();
    return -1;
  }
  return 0;
}

void pipewire_stop()
{
  debug("pipewire_stop");
  if (data.stream)
  {
    pw_stream_set_active(data.stream, false);
    pw_stream_disconnect(data.stream);
    pw_stream_destroy(data.stream);
    data.stream = NULL;
  }
  data.pw_core = NULL;
  if (data.pipewire_fd)
  {
    close(data.pipewire_fd);
    data.pipewire_fd = 0;
  }
  data.callback = NULL;
}
