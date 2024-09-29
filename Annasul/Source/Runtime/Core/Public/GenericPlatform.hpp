#pragma once

#ifndef ENABLE_FEATURE_DEBUG
#define ENABLE_FEATURE_DEBUG 0
#endif
#ifndef ENABLE_FEATURE_EDITORS
#define ENABLE_FEATURE_EDITORS 0
#endif
#ifndef ENABLE_FEATURE_RELEASE
#define ENABLE_FEATURE_RELEASE 0
#endif

namespace Annasul
{
	
	struct FGenericPlatformTypes
	{
		typedef unsigned char uint8;
		typedef unsigned short int uint16;
		typedef unsigned int uint32;
		typedef unsigned long long uint64;
		typedef signed char int8;
		typedef signed short int int16;
		typedef signed int int32;
		typedef signed long long int64;
		
		typedef char ANSICHAR;
		typedef wchar_t WIDECHAR;
		typedef WIDECHAR TCHAR;
	};
	
	struct FGenericPlatform
	{
		static bool IsDarkMode() = delete;
		
		static bool IsLightMode() = delete;
		
		static bool IsUserAnAdmin() = delete;
	};
	
}

#include "HAL/Platform.hpp"

#if PLATFORM_WINDOWS
	
	#include "Windows/WindowsPlatform.hpp"

#elif PLATFORM_MAC
	#include "Mac/MacPlatform.hpp"
#elif PLATFORM_LINUX
	#include "Linux/LinuxPlatform.hpp"
#elif PLATFORM_ANDROID
	#include "Android/AndroidPlatform.hpp"
#endif
