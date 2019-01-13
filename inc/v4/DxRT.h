#pragma once

#include "DxState.h"
#include "Mesh.h"


class DxRT
{
	friend class DxRT;
public:
	DxRT(ID3D11Device* pd3Device, UINT width, UINT height);

public:
	void Begin(ID3D11DeviceContext* pContext, DirectX::FXMVECTOR vColor);
	void End(ID3D11DeviceContext* pContext);

	void OnResize(UINT width, UINT height);

	bool Render(ID3D11DeviceContext* context, Mesh* mesh, DxObj* dxobj);

	ID3D11ShaderResourceView** GetSRV();
	ID3D11ShaderResourceView** GetDSSrv();

	ID3D11Texture2D* Texture();

	D3D11_VIEWPORT Viewport()const;

private:
	void CreateResource();
	void CreateDepthStencilView();
protected:
	UINT mWidth = 0;
	UINT mHeight = 0;

	ID3D11Device * md3Device = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture = nullptr;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pDSSrv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencil = nullptr;

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc;
	D3D11_TEXTURE2D_DESC m_TexDesc;

	D3D11_VIEWPORT m_Viewport;
};