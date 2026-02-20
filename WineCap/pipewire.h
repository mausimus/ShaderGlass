/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include <pipewire/pipewire.h>

int pipewire_start(struct pw_loop* loop, struct pw_context* ctx, int pipewire_fd, int pipewire_node, PIPEWIRE_CALLBACK_FUNC callback);
void pipewire_stop();
