#include "shaderheader.hlsl"

CVertexOut VS(CVertexIn vIn)
{
	CVertexOut vOut;
	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, gWorld);
	float4 vView = mul(vWorld, gView);
	float4 vProj = mul(vView, gProj);
	vOut.p = vProj;
	vOut.c = vIn.c;
	return vOut;
}

float4 PS(CVertexOut vOut) : SV_Target
{
	float4 Color = vOut.c;
	return Color;
}