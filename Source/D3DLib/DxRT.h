#pragma once

#include "DxState.h"
#include "Mesh.h"


class DxRT
{
	friend class DxRT;
public:
	void Initialize(ID3D11Device* pd3Device, float width, float height, ID3D11Texture2D* pTexture = nullptr);

	void Begin(ID3D11DeviceContext* pContext, DirectX::FXMVECTOR vColor);
	void End(ID3D11DeviceContext* pContext, DxRT* pDxrt = nullptr);

	void OnResize(ID3D11Device* pd3Device, float width, float height, ID3D11Texture2D* pTexture = nullptr);
	void Reset();

	bool Render(ID3D11DeviceContext* context, Mesh* mesh, DxObj* dxobj);

	ID3D11ShaderResourceView** GetSRV();
	ID3D11ShaderResourceView* ShaderResourceView();
	ID3D11RenderTargetView** RenderTargetView();
	ID3D11DepthStencilView* DepthStencilView();
	ID3D11Texture2D* Texture();

	D3D11_VIEWPORT Viewport()const;

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
};