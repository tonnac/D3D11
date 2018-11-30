#pragma once
#include "DxObj.h"
#include "MathHelper.h"

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

struct PassConstants
{
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;

};

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
};
