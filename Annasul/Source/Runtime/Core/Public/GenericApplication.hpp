#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	struct FGenericApplication
	{
		static int32 Loop() = delete;
		static void Quit(int32 exitCode) = delete;
	};
	
}

#ifdef PLATFORM_WINDOWS
#include "Windows/WindowsApplication.hpp"
#else
#error "Platform not supported"
#endif
