#include "DxRT.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

void DxRT::Initialize(ID3D11Device * pd3Device, float width, float height, ID3D11Texture2D * pTexture)
{
	m_pRenderTargetView.Reset();

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = width;
	m_Viewport.Height = height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	if (pTexture != nullptr)
	{
		ThrowifFailed(pd3Device->CreateRenderTargetView(
			pTexture, nullptr, m_pRenderTargetView.GetAddressOf()));
	}
	else
	{
		m_TexDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, Casting(UINT, width), Casting(UINT, height));

		m_TexDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		m_TexDesc.MipLevels = 1;

		ThrowifFailed(pd3Device->CreateTexture2D(&m_TexDesc, nullptr, m_pTexture.GetAddressOf()));
		ThrowifFailed(pd3Device->CreateShaderResourceView(m_pTexture.Get(), nullptr, m_pShaderResourceView.GetAddressOf()));
		ThrowifFailed(pd3Device->CreateRenderTargetView(m_pTexture.Get(), nullptr, m_pRenderTargetView.GetAddressOf()));
	}

	CreateDepthStencilView(pd3Device, Casting(UINT, width), Casting(UINT, height));
}

void DxRT::Begin(ID3D11DeviceContext * pContext, DirectX::FXMVECTOR vColor)
{
	ID3D11RenderTargetView* pRender = nullptr;
	pContext->RSSetViewports(1, &m_Viewport);
	pContext->OMSetRenderTargets(1, &pRender, nullptr);
	pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	XMVECTOR Color = vColor;
	pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<FLOAT*>(&Color));
	pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	if (m_cbData != nullptr)
	{
		pContext->UpdateSubresource(m_PassCB.Get(), 0, nullptr, m_cbData.get(), 0, 0);
		pContext->VSSetConstantBuffers(0, 1, m_PassCB.GetAddressOf());
	}
}

void DxRT::End(ID3D11DeviceContext * pContext, DxRT * pDxrt)
{
	if (pDxrt != nullptr)
	{
		pContext->RSSetViewports(1, &pDxrt->m_Viewport);
		ID3D11RenderTargetView* pRender = nullptr;
		pContext->OMSetRenderTargets(1, &pRender, nullptr);
		pContext->OMSetRenderTargets(1, pDxrt->m_pRenderTargetView.GetAddressOf(), pDxrt->m_pDepthStencilView.Get());
		if (m_cbData != nullptr)
			pContext->VSSetConstantBuffers(0, 1, pDxrt->m_PassCB.GetAddressOf());
	}
}

void DxRT::OnResize(ID3D11Device* pd3Device, float width, float height)
{
	Initialize(pd3Device, width, height);
}

void DxRT::Reset()
{
	m_pRenderTargetView.Reset();
	m_pDepthStencilView.Reset();
}

ID3D11ShaderResourceView * DxRT::ShaderResourceView() 
{
	return m_pShaderResourceView.Get();
}

void DxRT::CreateDepthStencilView(ID3D11Device * pd3Device, UINT width, UINT height)
{
	m_pDepthStencilView.Reset();

	ComPtr<ID3D11Texture2D> pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC descDepth = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, width, height);
	descDepth.MipLevels = 1;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ThrowifFailed(pd3Device->CreateTexture2D(&descDepth, nullptr, pDepthStencilTexture.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(
		pDepthStencilTexture.Get(),
		D3D11_DSV_DIMENSION_TEXTURE2D,
		descDepth.Format);

	ThrowifFailed(pd3Device->CreateDepthStencilView(pDepthStencilTexture.Get(), &dsvDesc,
		m_pDepthStencilView.GetAddressOf()));

	m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);
}
