#pragma once

#include "Platform.hpp"
#include "GenericRenderEngine.hpp"

namespace Annasul
{
	enum class ERenderEngineType
	{
		DirectX3D11,
		DirectX3D12,
		DirectX2D01
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

