#include "Windows/WindowsRenderEngineFactory.hpp"
#include "DirectX3D11/DirectX3D11RenderEngine.hpp"
#include "DirectX3D12/DirectX3D12RenderEngine.hpp"
#include "DirectX2D01/DirectX2D01RenderEngine.hpp"

namespace Annasul
{
	
	FGenericRenderEngine *FWindowsRenderEngineFactory::Create(ERenderEngineType type)
	{
		switch (type)
		{
			case ERenderEngineType::DirectX3D11:
				return new FDirectX3D11RenderEngine();
			case ERenderEngineType::DirectX3D12:
				return new FDirectX3D12RenderEngine();
			case ERenderEngineType::DirectX2D01:
				return new FDirectX2D01RenderEngine();
			default:
				return nullptr;
		}
	}
	
}

