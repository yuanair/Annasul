#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	
	class FGenericRenderEngine
	{
	public:
		
		FGenericRenderEngine() = default;
		
		virtual ~FGenericRenderEngine() = default;
	
	public:
	
	
	};
	
}

#if PLATFORM_WINDOWS

#include "Windows/DirectXRenderEngine.hpp"

#elif PLATFORM_MAC
#include "Mac/MacRenderEngine.hpp"
#elif PLATFORM_LINUX
#include "Linux/LinuxRenderEngine.hpp"
#else
#error "Unsupported Platform"
#endif
