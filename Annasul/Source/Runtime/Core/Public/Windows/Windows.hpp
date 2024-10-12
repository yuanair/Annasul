#pragma once

#include <Windows.h>
#include <shellapi.h>
#include <dwmapi.h>
#include <ShlObj.h>

#if defined(DEBUG) || defined(_DEBUG)

#include <crtdbg.h>

#endif

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#undef min
#undef max
