#pragma once

#include "Platform.hpp"
#include <cstring>

namespace Annasul
{

	struct FGenericMemory
	{

		FORCEINLINE static void* Copy(void* dest, const void* src, const SIZE_T size)
		{
			return ::memcpy(dest, src, size);
		}

		FORCEINLINE static void* Move(void* dest, const void* src, const SIZE_T size)
		{
			return ::memmove(dest, src, size);
		}

	};
}

#ifdef PLATFORM_WINDOWS
	#include "Windows/WindowsMemory.hpp"
#elif defined(PLATFORM_LINUX)
	#include "Linux/LinuxMemory.hpp"
#elif defined(PLATFORM_MACOS)
	#include "MacOS/MacOSMemory.hpp"
#elif defined(PLATFORM_IOS)
	#include "IOS/IOSMemory.hpp"
#else
	#error "Unsupported Platform!"
#endif
