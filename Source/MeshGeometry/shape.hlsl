#include "shaderheader.hlsl"

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, gWorld);
	float4 vView = mul(vWorld, gViewProj);
	vOut.p = vView;
	vOut.c = vIn.c;
	vOut.n = vIn.n;
	vOut.t = vIn.t;
	return vOut;
}

float4 PS(VertexOut vOut) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_samLinearWrap, vOut.t);
	return TexColor;
}

