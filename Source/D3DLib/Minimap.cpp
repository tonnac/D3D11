#include "Minimap.h"
#include "DirectWrite.h"

using namespace DirectX;

void Minimap::BuildMinimap(ID3D11Device* pd3Device, float width, float height,
						   const std::tstring& szShaderFile, const DirectX::FXMMATRIX& matView)
{
	DxRT::Initialize(pd3Device, width, height);
	m_cbData = std::make_unique<PassConstants>();
	d3dUtil::CreateConstantBuffer(pd3Device, 1, sizeof(PassConstants), m_PassCB.GetAddressOf());

	XMStoreFloat4x4(&m_matView, matView);

	d3dUtil::LoadVertexShaderFile(pd3Device, L"minimap.hlsl", mVertexShader.GetAddressOf(), "VS", pVertexBlob.GetAddressOf());
	d3dUtil::LoadPixelShaderFile(pd3Device, L"minimap.hlsl", mPixelShader.GetAddressOf());

	m_MinimapVP.MinDepth = 0.0f;
	m_MinimapVP.MaxDepth = 1.0f;
}

bool Minimap::Frame()
{
	XMMATRIX View = XMLoadFloat4x4(&m_matView);

	XMMATRIX Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, Casting(float, g_ClientWidth) / Casting(float, g_ClientHeight), 1.0f, 1000.0f);

	XMMATRIX InvView = XMMatrixInverse(&XMMatrixDeterminant(View), View);
	XMMATRIX InvProj = XMMatrixInverse(&XMMatrixDeterminant(Proj), Proj);

	XMMATRIX ViewProj = View * Proj;
	XMMATRIX InvViewProj = XMMatrixInverse(&XMMatrixDeterminant(ViewProj), ViewProj);

	XMStoreFloat4x4(&m_cbData->View, XMMatrixTranspose(View));
	XMStoreFloat4x4(&m_cbData->Proj, XMMatrixTranspose(Proj));
	XMStoreFloat4x4(&m_cbData->InvView, XMMatrixTranspose(InvView));
	XMStoreFloat4x4(&m_cbData->InvProj, XMMatrixTranspose(InvProj));
	XMStoreFloat4x4(&m_cbData->ViewProj, XMMatrixTranspose(ViewProj));
	XMStoreFloat4x4(&m_cbData->InvViewProj, XMMatrixTranspose(InvViewProj));

	m_cbData->NearZ = 1.0f;
	m_cbData->FarZ = 1000.0f;
//	m_MinimapPassCB.TotalTime = m_Timer.DeltaTime();
//	m_MinimapPassCB.DeltaTime = m_Timer.TotalTime();

	return true;
}

bool Minimap::Render(ID3D11DeviceContext* pContext, 
	float Left, float Top, float Width, float Height, const std::tstring& text)
{
	m_MinimapVP.TopLeftX = Left;
	m_MinimapVP.TopLeftY = Top;
	m_MinimapVP.Width = Width;
	m_MinimapVP.Height = Height;

	pContext->OMSetBlendState(DxState::m_BSS[(int)E_BSS::No].Get(), 0, -1);
	pContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	pContext->PSSetShaderResources(0, 1, m_pShaderResourceView.GetAddressOf());
	pContext->RSSetViewports(1, &m_MinimapVP);
	pContext->Draw(6, 1);

	S_Write.DrawText({ m_MinimapVP.TopLeftX ,m_MinimapVP.TopLeftY, 
		m_MinimapVP.TopLeftX + m_MinimapVP.Width, 
		m_MinimapVP.TopLeftY + m_MinimapVP.Height }, 
		L"MiniMap", Colors::Black, DWRITE_TEXT_ALIGNMENT_CENTER);
	if (!text.empty())
	{
		S_Write.DrawText({ m_MinimapVP.TopLeftX ,m_MinimapVP.TopLeftY + m_MinimapVP.Height - 30,
			m_MinimapVP.TopLeftX + m_MinimapVP.Width,
			m_MinimapVP.TopLeftY + m_MinimapVP.Height + 200 },
			text.c_str(), Colors::Black, DWRITE_TEXT_ALIGNMENT_CENTER);
	}
	return true;
}

void Minimap::SetViewMatrix(const DirectX::FXMMATRIX & matView)
{
	XMStoreFloat4x4(&m_matView, matView);
}

