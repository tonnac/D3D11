#include "common.hlsl"

TextureCube gCubeMap	: register (t0);

struct VertexIn
{
	float3 PosL : POSITION;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vOut = (VertexOut)0.0f;

	vOut.PosL = vin.PosL;

	float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	PosW.xyz += gEyePos;
	vOut.PosH = mul(PosW, gViewProj).xyww;
	
	return vOut;
}

float4 PS(VertexOut pin) : SV_TARGET
{
	return gCubeMap.Sample(g_samLinearWrap, pin.PosL);
}