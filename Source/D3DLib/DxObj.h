#pragma once

#include "d3dUtil.h"

struct DxObj
{
	DxObj() = default;
	~DxObj() = default;

	void SetResource(ID3D11DeviceContext* pContext);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader = nullptr;
};