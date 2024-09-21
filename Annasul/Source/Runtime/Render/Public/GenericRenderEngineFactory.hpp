#pragma once

#include "Platform.hpp"
#include "GenericRenderEngine.hpp"

namespace Annasul
{
	enum class ERenderEngineType
	{
		DirectX11,
		DirectX12,
		Vulkan
	};
	
	struct FGenericRenderEngineFactory
	{
		
		static FGenericRenderEngine *Create(ERenderEngineType type) = delete;
		
	};
	
}

#if PLATFORM_WINDOWS

#include "Windows/WindowsRenderEngineFactory.hpp"

#else
#error "Unsupported Platform"
#endif

