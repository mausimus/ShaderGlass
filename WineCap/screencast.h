/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

int screencast_init();
void screencast_run();
void screencast_start(int type, bool cursor, PIPEWIRE_CALLBACK_FUNC callback);
void screencast_stop();
void screencast_destroy();
