#pragma once
#pragma warning( disable : 4005) 
#include "Core.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <DirectWrite.h>

#pragma comment(lib, "d3dx11.lib")

//#define CPU
#define GPU

struct P3VERTEX
{
	float x, y, z;
	float r, g, b, a;
	float u, v;
};
struct VS_CB
{
	float vColor[4];
	float fTime[4];
};

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	void						CreateVertexBuffer();
	void						CreateIndexBuffer();
	void						CreateConstantBuffer();
	void						LoadShaderAndInputLayout();
	void						LoadTextureFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName);
	void						setBlendState();
public:
	bool						Init();
	bool						Frame();
	bool						Render();
	bool						Release();
private:
	VS_CB						m_ConstantData;
	ID3D11Buffer *				m_pVertexBuffer;
	ID3D11Buffer *				m_pIndexBuffer;
	ID3D11Buffer *				m_pConstantBuffer;
	ID3D11InputLayout*			m_pVertexLayout;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	ID3D11PixelShader*			m_pPixelShader1;
	std::vector<P3VERTEX>		m_VertexList;
	std::vector<DWORD>			m_IndexList;
	ID3D11ShaderResourceView*	m_pTextureSRV;
	ID3D11ShaderResourceView*	m_pTexNOALPHA;
	ID3D11SamplerState*			m_pSamplerState;

	ID3D11BlendState*			m_pBlendState;
};