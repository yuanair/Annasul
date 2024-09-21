#pragma once

#include "Platform.hpp"
#include "Program.hpp"

namespace Annasul
{
	
	struct FGenericApplication
	{
		static int32 Loop(FProgram &program) = delete;
		
		static void Quit(int32 exitCode) = delete;
	};
	
}

#if PLATFORM_WINDOWS

#include "Windows/WindowsApplication.hpp"

#else
#error "Platform not supported"
#endif
