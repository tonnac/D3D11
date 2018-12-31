#pragma once

#include "DxState.h"
#include "ShaderStorage.h"

struct DxObj
{
	DxObj() = default;
	~DxObj() = default;

	void SetResource(ID3D11DeviceContext* pContext);

	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11GeometryShader* m_pGeometryShader = nullptr;

	E_DSS m_DepthStencilState = E_DSS::Default;
	E_RSS m_RasterizerState = E_RSS::Default;
	E_BSS m_BlendState = E_BSS::Default;
};

enum class DxType
{
	DEFAULT,
	SKINNED,
	LINE,
	SKY,
	NOTEX
};

struct DxObjStorage
{
private:
	DxObjStorage() = default;

public:
	DxObjStorage(const DxObjStorage& rhs) = delete;
	DxObjStorage& operator=(const DxObjStorage& rhs) = delete;
	~DxObjStorage() = default;

	static DxObjStorage * Dxobj()
	{
		static DxObjStorage storage;
		return &storage;
	}

	static std::unordered_map<DxType, std::unique_ptr<DxObj>> GetDxobjList();
};