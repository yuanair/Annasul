#pragma once

namespace Annasul
{
	
	struct FGenericPlatformTypes
	{
		typedef unsigned char           uint8;
		typedef unsigned short int      uint16;
		typedef unsigned int            uint32;
		typedef unsigned long long      uint64;
		typedef signed char             int8;
		typedef signed short int        int16;
		typedef signed int              int32;
		typedef signed long long        int64;
		
		typedef char                    ANSICHAR;
		typedef wchar_t                 WIDECHAR;
		typedef WIDECHAR                TCHAR;
	};
	
}

#include "HAL/Platform.hpp"
#ifdef PLATFORM_WINDOWS
	#include "Windows/WindowsPlatform.hpp"
#elif PLATFORM_MAC
	#include "Mac/MacPlatform.hpp"
#elif PLATFORM_LINUX
	#include "Linux/LinuxPlatform.hpp"
#elif PLATFORM_IOS
	#include "IOS/IOSPlatform.hpp"
#elif PLATFORM_ANDROID
	#include "Android/AndroidPlatform.hpp"
#endif
