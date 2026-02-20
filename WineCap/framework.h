/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define info(...) printf("[WineCap] INFO "); printf(__VA_ARGS__); printf("\n");
#define warn(...) printf("[WineCap] WARN "); printf(__VA_ARGS__); printf("\n");

#ifdef _DEBUG
#define debug(...) printf("[WineCap] DEBUG "); printf(__VA_ARGS__); printf("\n");
#else
#define debug(...)
#endif
