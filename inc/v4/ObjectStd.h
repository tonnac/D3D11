#pragma once
#include "DxObj.h"

struct PC_VERTEX
{
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT4 c;
};

struct PNCT_VERTEX
{
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT3 n;
	DirectX::XMFLOAT4 c;
	DirectX::XMFLOAT2 t;
	PNCT_VERTEX()
	{}
	PNCT_VERTEX(DirectX::XMFLOAT3 vp, DirectX::XMFLOAT3 np, DirectX::XMFLOAT4 vc, DirectX::XMFLOAT2 vt)
	{
		p = vp;
		n = np;
		c = vc;
		t = vt;
	}
	PNCT_VERTEX(const PNCT_VERTEX& rhs)
	{
		p = rhs.p;
		n = rhs.n;
		c = rhs.c;
		t = rhs.t;
	}
};

struct PT_VERTEX
{
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT2 t;
};

struct CB_DATA
{
	DirectX::XMFLOAT4X4 matWorld;
	DirectX::XMFLOAT4X4 matView;
	DirectX::XMFLOAT4X4 matProj;
	float fTime;
	float fActiveVP;
	float z, w;
	DirectX::XMFLOAT4 vColor;
};
