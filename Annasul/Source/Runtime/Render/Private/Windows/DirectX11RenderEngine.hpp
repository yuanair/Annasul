#pragma once

#include "Windows/DirectXRenderEngine.hpp"

#include "DirectX11.hpp"

namespace Annasul
{
	class FDirectX11RenderEngine : public FDirectXRenderEngine
	{
	public:
		
		FDirectX11RenderEngine();
		~FDirectX11RenderEngine() override;
		
	private:
		
		Microsoft::WRL::ComPtr<ID3D11Device> Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
	
	};
}
