#pragma once

#include "Windows/DirectXRenderEngine.hpp"
#include "DirectX2D01.hpp"

namespace Annasul
{
	
	class FDirectX2D01RenderEngine : public FDirectXRenderEngine
	{
	public:
		
		FDirectX2D01RenderEngine();
		
		~FDirectX2D01RenderEngine() override;
	
	private:
		
		Microsoft::WRL::ComPtr<ID2D1Factory> m_pFactory;
		
	};
	
} // Annasul
