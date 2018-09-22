#pragma once
#include "d3dUtil.h"
#include "Shader.h"
#include "Texture.h"



struct P3_VERTEX
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 TexPos;
};
struct VS_CB
{
	D3DXVECTOR4 Color;
	D3DXVECTOR4 Util;
	D3DXMATRIX	Proj;
};

class Object_Basic
{
public:
	bool						PreRender(ID3D11DeviceContext* pContext);
	bool						Release();
public:
	void						CreateBuffer(ID3D11Device * pDevice, const D3D11_BIND_FLAG& type = D3D11_BIND_CONSTANT_BUFFER,
											void * pData = nullptr, const UINT& Size = sizeof(VS_CB));
	void						CreateTexture(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath);
	void						CreateShader(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& Filepath, const std::string& VSFunc, const std::string& PSFunc);
	void						setBlendState(ID3D11Device* pDevice);
	void						CreateaInputLayout(ID3D11Device* pDevice);
	void						CreateRasterizer(ID3D11Device* pDevice);
public:
	ID3D11Buffer *				getVertexBuffer() const;
	ID3D11Buffer *				getIndexBuffer() const;
	ID3D11Buffer *				getConstantBuffer() const;
	ID3D11BlendState*			getBlendState() const;
	Texture*					getTexture() const;
private:
	ID3D11Buffer *				m_pIndexBuffer = nullptr;
	ID3D11Buffer *				m_pConstantBuffer = nullptr;
	ID3D11Buffer *				m_pVertexBuffer = nullptr;
	ID3D11InputLayout*			m_pInputLayout = nullptr;

	Shader*						m_pShader = nullptr;
	Texture*					m_pTexture = nullptr;

	ID3D11BlendState*			m_pBlendState = nullptr;
	ID3D11RasterizerState*		m_pWireFrameRS = nullptr;
	ID3D11RasterizerState*		m_pSolidRS = nullptr;
};