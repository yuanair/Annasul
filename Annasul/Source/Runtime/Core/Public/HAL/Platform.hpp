#pragma once

#if defined(WIN32)
	#define PLATFORM_WINDOWS
	#ifdef WIN64
		#define PLATFORM_BIT_64
	#else
		#define PLATFORM_BIT_32
	#endif
#elif defined(__APPLE__)
	#define PLATFORM_MACOS
	#if __LP64__
		#define PLATFORM_BIT_64
	#else
		#define PLATFORM_BIT_32
	#endif
#elif defined(__linux__)
	#define PLATFORM_LINUX
	#if defined(__ANDROID__)
		#define PLATFORM_LINUX_ANDROID
	#else
		#define PLATFORM_LINUX_UNKNOWN
	#endif
	#if __LP64__
		#define PLATFORM_BIT_64
	#else
		#define PLATFORM_BIT_32
	#endif
#else
	#error "Platform not supported!"
#endif


