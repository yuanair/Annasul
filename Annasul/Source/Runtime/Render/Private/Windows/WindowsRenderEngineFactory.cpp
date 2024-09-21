#include "Windows/WindowsRenderEngineFactory.hpp"
#include "DirectX11RenderEngine.hpp"
#include "DirectX12RenderEngine.hpp"

namespace Annasul
{
	
	FGenericRenderEngine *FWindowsRenderEngineFactory::Create(ERenderEngineType type)
	{
		switch (type)
		{
			case ERenderEngineType::DirectX11:
				return new FDirectX11RenderEngine();
			case ERenderEngineType::DirectX12:
				return new FDirectX12RenderEngine();
			default:
				return nullptr;
		}
	}

}

