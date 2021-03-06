#pragma once

#include "ObjectStd.h"
#include "DxState.h"

class DxRT
{
	friend class DxRT;
public:
	void Initialize(ID3D11Device* pd3Device, float width, float height, ID3D11Texture2D* pTexture = nullptr, int e = 0);

	void Begin(ID3D11DeviceContext* pContext, DirectX::FXMVECTOR vColor);
	void End(ID3D11DeviceContext* pContext, DxRT* pDxrt = nullptr);

	void OnResize(ID3D11Device* pd3Device, float width, float height);
	void Reset();

	bool Render(ID3D11DeviceContext* context);

	ID3D11ShaderResourceView* ShaderResourceView();

private:
	void CreateDepthStencilView(ID3D11Device* pd3Device, UINT width, UINT height);

protected:

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture = nullptr;

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc;
	D3D11_TEXTURE2D_DESC m_TexDesc;

	D3D11_VIEWPORT m_Viewport;

	std::unique_ptr<PassConstants> m_cbData = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_PassCB = nullptr;
};