#include "Minimap.h"
#include "DirectWrite.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

void Minimap::BuildMinimap(ID3D11Device* pd3Device, float width, float height,
						   const std::tstring& szShaderFile)
{
	DxRT::Initialize(pd3Device, width, height);

	ComPtr<ID3DBlob> vertexBlob = nullptr;
	
	d3dUtil::LoadVertexShaderFile(pd3Device, L"minimap.hlsl", nullptr, mVertexShader.GetAddressOf(), "VS", vertexBlob.GetAddressOf());
	d3dUtil::LoadGeometryShaderFile(pd3Device, L"minimap.hlsl", nullptr, mGeometryShader.GetAddressOf());
	d3dUtil::LoadPixelShaderFile(pd3Device, L"minimap.hlsl", nullptr, mPixelShader.GetAddressOf());

	m_MinimapVP.MinDepth = 0.0f;
	m_MinimapVP.MaxDepth = 1.0f;
}

bool Minimap::Frame()
{
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
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	pContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	pContext->GSSetShader(mGeometryShader.Get(), nullptr, 0);
	pContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
	pContext->PSSetShaderResources(0, 1, m_pShaderResourceView.GetAddressOf());
	pContext->RSSetViewports(1, &m_MinimapVP);
	pContext->Draw(1, 0);

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