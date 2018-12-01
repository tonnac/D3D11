#pragma once


#include "DxRT.h"
#include "Shape.h"

class Minimap : public DxRT
{
public:
	void BuildMinimap(ID3D11Device* pd3Device, float width, float height,
					  const std::tstring& szShaderFile, const DirectX::FXMMATRIX& matView);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext, const D3D11_RECT& MapPos);

	void SetViewMatrix(const DirectX::FXMMATRIX& matView);

private:
	PlaneShape		m_PlaneShape;
	D3D11_VIEWPORT	m_MinimapVP;

	DirectX::XMFLOAT4X4		m_matView;
};