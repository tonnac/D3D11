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
}

void DxRT::End(ID3D11DeviceContext * pContext, DxRT * pDxrt)
{
	if (pDxrt != nullptr)
	{
		pContext->RSSetViewports(1, &pDxrt->m_Viewport);
		ID3D11RenderTargetView* pRender = nullptr;
		pContext->OMSetRenderTargets(1, &pRender, nullptr);
		pContext->OMSetRenderTargets(1, pDxrt->m_pRenderTargetView.GetAddressOf(), pDxrt->m_pDepthStencilView.Get());
	}
	ID3D11RenderTargetView* pRender = nullptr;
	pContext->OMSetRenderTargets(1, &pRender, nullptr);
	ID3D11ShaderResourceView * sview[] = { nullptr, nullptr , nullptr, nullptr, nullptr, nullptr };
	pContext->PSSetShaderResources(0, (UINT)std::size(sview), sview);
}

void DxRT::OnResize(ID3D11Device* pd3Device, float width, float height, ID3D11Texture2D* pTexture)
{
	Initialize(pd3Device, width, height, pTexture);
}

void DxRT::Reset()
{
	m_pRenderTargetView.Reset();
	m_pDepthStencilView.Reset();
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

ID3D11ShaderResourceView * DxRT::ShaderResourceView()
{
	return m_pShaderResourceView.Get();
}

D3D11_VIEWPORT DxRT::Viewport() const
{
	return m_Viewport;
}

ID3D11RenderTargetView ** DxRT::RenderTargetView()
{
	return m_pRenderTargetView.GetAddressOf();
}

ID3D11DepthStencilView * DxRT::DepthStencilView()
{
	return m_pDepthStencilView.Get();
}

ID3D11Texture2D * DxRT::Texture()
{
	return m_pTexture.Get();
}

void DxRT::CreateDepthStencilView(ID3D11Device * pd3Device, UINT width, UINT height)
{
	m_pDepthStencilView.Reset();

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.ArraySize = 1;
	descDepth.MipLevels = 1;
	descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ThrowifFailed(pd3Device->CreateTexture2D(&descDepth, nullptr, m_DepthStencil.GetAddressOf()));



	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(
		m_DepthStencil.Get(),
		D3D11_DSV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_D24_UNORM_S8_UINT);

	ThrowifFailed(pd3Device->CreateDepthStencilView(m_DepthStencil.Get(), &dsvDesc,
		m_pDepthStencilView.GetAddressOf()));


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	ThrowifFailed(pd3Device->CreateShaderResourceView(m_DepthStencil.Get(), &srvDesc, m_pDSSrv.GetAddressOf()));

	m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);
}
