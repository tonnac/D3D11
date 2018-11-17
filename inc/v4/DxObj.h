#pragma once

#include "d3dUtil.h"

class DxObj
{
public:
	DxObj() = default;
	~DxObj() = default;

public:
	bool PreRender(ID3D11DeviceContext* pContext);
	bool PostRender(ID3D11DeviceContext* pContext);
	bool Render(ID3D11DeviceContext* pContext);

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pVSBlob = nullptr;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;

	UINT m_iNumIndex = 0;
	UINT m_iNumVertex = 0;
	UINT m_iVertexSize = 0;
	UINT m_iIndexSize = 0;
};