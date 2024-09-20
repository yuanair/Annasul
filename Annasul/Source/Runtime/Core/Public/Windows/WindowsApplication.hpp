#pragma once

#include "GenericApplication.hpp"

namespace Annasul
{
	
	struct FWindowsApplication : public FGenericApplication
	{
		
		static int32 Loop();
		
		static void Quit(int32 exitCode);
		
	};
	
	typedef FWindowsApplication FApplication;
	
}
