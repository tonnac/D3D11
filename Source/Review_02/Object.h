#pragma once
#include "d3dUtil.h"
#include "Shader.h"
#include "Texture.h"
#include <DirectXMath.h>

#define GPU
//#define CPU

struct P3_VERTEX
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexPos;

};
struct VS_CB
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT4 Util;
};

class Object
{
public:
	bool						InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath);
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
	virtual bool				Render(ID3D11DeviceContext* pContext);
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual bool				Release();
protected:
	virtual void				CreateVertexBuffer(ID3D11Device* pDevice);
	virtual void				CreateIndexBuffer(ID3D11Device* pDevice);
	virtual void				CreateConstantBuffer(ID3D11Device* pDevice);
	virtual void				CreateTexture(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath);
	virtual void				CreateShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath);
	virtual void				setBlendState(ID3D11Device* pDevice);
	virtual void				CreateaInputLayout(ID3D11Device* pDevice);
protected:
	std::vector<P3_VERTEX>		m_VertexList;
	std::vector<DWORD>			m_indiciesList;
	VS_CB						m_ConstantData;
	ID3D11Buffer *				m_pIndexBuffer = nullptr;
	ID3D11Buffer *				m_pConstantBuffer;
	ID3D11Buffer *				m_pVertexBuffer;
	ID3D11InputLayout*			m_pInputLayout;

	Shader*						m_pShader;
	Texture*					m_pTexture;

	ID3D11BlendState*			m_pBlendState;
};