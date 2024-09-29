#pragma once

#include "Windows/DirectXRenderEngine.hpp"

#include "DirectX3D11.hpp"

namespace Annasul
{
	class FDirectX3D11RenderEngine : public FDirectXRenderEngine
	{
	public:
		
		FDirectX3D11RenderEngine();
		~FDirectX3D11RenderEngine() override;
	
	private:
		
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
		
	};
}
