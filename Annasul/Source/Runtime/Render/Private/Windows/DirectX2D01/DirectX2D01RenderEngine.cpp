
#include "DirectX2D01RenderEngine.hpp"

#include "GenericDebug.hpp"

namespace Annasul
{
	FDirectX2D01RenderEngine::FDirectX2D01RenderEngine()
	{
		D2D1_FACTORY_OPTIONS factoryOptions;
		factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
		FDebug::Get().ErrorLogIfFailed(
			EDebugLevel::Error, TEXT("Failed to create Direct2D factory"),
			::D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &factoryOptions,
				IID_PPV_ARGS_Helper(&m_pFactory))
		);
		
	}
	
	FDirectX2D01RenderEngine::~FDirectX2D01RenderEngine()
	{
	
	}
} // Annasul