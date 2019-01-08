#pragma once
#include "DxObj.h"
#include "MathHelper.h"

struct VertexC
{
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT4 c;
	VertexC()
	{}
	VertexC(DirectX::XMFLOAT3 vp, DirectX::XMFLOAT4 vc)
	{
		p = vp;
		c = vc;
	}
};

struct Vertex
{
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT3 n;
	DirectX::XMFLOAT4 c;
	DirectX::XMFLOAT2 t;
	DirectX::XMFLOAT3 Tangent;
	Vertex()
	{}
	Vertex(DirectX::XMFLOAT3 vp, DirectX::XMFLOAT3 np, DirectX::XMFLOAT4 vc, DirectX::XMFLOAT2 vt, DirectX::XMFLOAT3 vtan)
	{
		p = vp;
		n = np;
		c = vc;
		t = vt;
		Tangent = vtan;
	}
	Vertex(DirectX::XMFLOAT3 vp, DirectX::XMFLOAT3 np, DirectX::XMFLOAT4 vc, DirectX::XMFLOAT2 vt)
	{
		p = vp;
		n = np;
		c = vc;
		t = vt;
	}
	Vertex(const Vertex& rhs)
	{
		p = rhs.p;
		n = rhs.n;
		c = rhs.c;
		t = rhs.t;
		Tangent = rhs.Tangent;
	}
};

struct SkinnedVertex
{
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT3 n;
	DirectX::XMFLOAT4 c;
	DirectX::XMFLOAT2 t;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT4 BoneWeights;
	BYTE BoneIndices[4];
};

struct VertexT
{
	DirectX::XMFLOAT3 p;
	DirectX::XMFLOAT2 t;
};

