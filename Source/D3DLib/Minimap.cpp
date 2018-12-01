#include "Minimap.h"
#include "DirectWrite.h"

using namespace DirectX;

void Minimap::BuildMinimap(ID3D11Device* pd3Device, float width, float height,
						   const std::tstring& szShaderFile, const DirectX::FXMMATRIX& matView)
{
	DxRT::Initialize(pd3Device, width, height);
	m_cbData = std::make_unique<PassConstants>();
	d3dUtil::CreateConstantBuffer(pd3Device, 1, sizeof(PassConstants), m_PassCB.GetAddressOf());

	m_PlaneShape.Create(pd3Device, szShaderFile.c_str());

	XMStoreFloat4x4(&m_matView, matView);

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

bool Minimap::Render(ID3D11DeviceContext* pContext, const D3D11_RECT& MapPos)
{
	m_MinimapVP.TopLeftX = Casting(float, MapPos.left);
	m_MinimapVP.TopLeftY = Casting(float, MapPos.top);
	m_MinimapVP.Width = Casting(float, MapPos.right);
	m_MinimapVP.Height = Casting(float,MapPos.bottom );

	m_PlaneShape.SetShaderResourceView(m_pShaderResourceView.Get());
	pContext->RSSetViewports(1, &m_MinimapVP);
	m_PlaneShape.Render(pContext);
	S_Write.DrawText({ m_MinimapVP.TopLeftX ,m_MinimapVP.TopLeftY, 
		m_MinimapVP.TopLeftX + m_MinimapVP.Width, 
		m_MinimapVP.TopLeftY + m_MinimapVP.Height }, 
		L"MiniMap", Colors::Black, DWRITE_TEXT_ALIGNMENT_CENTER);
	return false;
}

void Minimap::SetViewMatrix(const DirectX::FXMMATRIX & matView)
{
	XMStoreFloat4x4(&m_matView, matView);
}

