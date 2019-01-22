#pragma once


#include "DxRT.h"
#include "Shape.h"

class Minimap : public DxRT
{
public:
	void BuildMinimap(ID3D11Device* pd3Device, float width, float height,
					  const std::tstring& szShaderFile);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext, float Left, float Top, float Width, float Height, const std::tstring& text = std::tstring());

	void SetViewMatrix(const DirectX::FXMMATRIX& matView);

private:
	D3D11_VIEWPORT	m_MinimapVP;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader = nullptr;
};