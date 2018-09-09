#pragma once
#pragma warning( disable : 4005) 
#include "Core.h"
#include <D3DX11.h>

#pragma comment(lib, "d3dx11.lib")

struct P3VERTEX
{
	float x, y, z;
};

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	HRESULT				CreateVertexBuffer();
	HRESULT				LoadShaderAndInputLayout();
public:
	bool				Init();
	bool				Frame();
	bool				Render();
	bool				Release();
private:
	ID3D11Buffer *		m_pVertexBuffer;
	ID3D11InputLayout*	m_pVertexLayout;
	ID3D11VertexShader*	m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
};