#include "DxRT.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

DxRT::DxRT(ID3D11Device * pd3Device, UINT width, UINT height)
	: md3Device(pd3Device), mWidth(width), mHeight(height)
{
	CreateResource();
	CreateDepthStencilView();
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
}

void DxRT::End(ID3D11DeviceContext * pContext)
{
	ID3D11RenderTargetView* pRender = nullptr;
	pContext->OMSetRenderTargets(1, &pRender, nullptr);
	ID3D11ShaderResourceView * sview[] = { nullptr, nullptr , nullptr, nullptr, nullptr, nullptr };
	pContext->PSSetShaderResources(0, (UINT)std::size(sview), sview);
}

void DxRT::OnResize(UINT width, UINT height)
{
	if (width != mWidth || height != mHeight)
	{
		mWidth = width;
		mHeight = height;
		CreateResource();
		CreateDepthStencilView();
	}
}

bool DxRT::Render(ID3D11DeviceContext * context, Mesh* mesh, DxObj* dxobj)
{
	Begin(context, DirectX::Colors::Black);
	dxobj->SetResource(context);

	auto t = DxState::m_DSS[(int)E_DSS::Stencil];
	context->OMSetDepthStencilState(t.Get(), 1);
	mesh->Render(context);

	End(context);
	return true;
}

ID3D11ShaderResourceView ** DxRT::GetSRV()
{
	return m_pShaderResourceView.GetAddressOf();
}

ID3D11ShaderResourceView ** DxRT::GetDSSrv()
{
	return m_pDSSrv.GetAddressOf();
}

D3D11_VIEWPORT DxRT::Viewport() const
{
	return m_Viewport;
}

ID3D11Texture2D * DxRT::Texture()
{
	return m_pTexture.Get();
}

void DxRT::CreateResource()
{
	m_pTexture.Reset();
	m_pRenderTargetView.Reset();
	m_pShaderResourceView.Reset();

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = (float)mWidth;
	m_Viewport.Height = (float)mHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_TexDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, mWidth, mHeight);

	m_TexDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	m_TexDesc.MipLevels = 1;

	ThrowifFailed(md3Device->CreateTexture2D(&m_TexDesc, nullptr, m_pTexture.GetAddressOf()));
	ThrowifFailed(md3Device->CreateShaderResourceView(m_pTexture.Get(), nullptr, m_pShaderResourceView.GetAddressOf()));
	ThrowifFailed(md3Device->CreateRenderTargetView(m_pTexture.Get(), nullptr, m_pRenderTargetView.GetAddressOf()));
}

void DxRT::CreateDepthStencilView()
{
	m_pDSSrv.Reset();
	m_DepthStencil.Reset();
	m_pDepthStencilView.Reset();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = mWidth;
	descDepth.Height = mHeight;
	descDepth.ArraySize = 1;
	descDepth.MipLevels = 1;
	descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ThrowifFailed(md3Device->CreateTexture2D(&descDepth, nullptr, m_DepthStencil.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(
		m_DepthStencil.Get(),
		D3D11_DSV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_D24_UNORM_S8_UINT);

	ThrowifFailed(md3Device->CreateDepthStencilView(m_DepthStencil.Get(), &dsvDesc,
		m_pDepthStencilView.GetAddressOf()));


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	ThrowifFailed(md3Device->CreateShaderResourceView(m_DepthStencil.Get(), &srvDesc, m_pDSSrv.GetAddressOf()));

	m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);
}
