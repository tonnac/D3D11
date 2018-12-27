#include "common.hlsl"

struct VertexIn
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
#ifdef SKINNED
	float4 BoneWeights  : WEIGHTS;
	uint4  BoneIndices  : BONEINDICES;
#endif
};

struct VertexOut
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;

#ifdef SKINNED
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	uint Index;

	weights[0] = vIn.BoneWeights.x;
	weights[1] = vIn.BoneWeights.y;
	weights[2] = vIn.BoneWeights.z;
	weights[3] = 1.0f - (weights[0] + weights[1] + weights[2]);
	Index = (uint)vIn.BoneWeights.w;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for (uint i = 0; i < Index; ++i)
	{
		posL += weights[i] * mul(float4(vIn.p, 1.0f), gBoneTransforms[vIn.BoneIndices[i]]).xyz;
		normalL += weights[i] * mul(vIn.n, (float3x3)gBoneTransforms[vIn.BoneIndices[i]]);
	}
	
	vIn.p = posL;
	vIn.n = normalL;
#endif

	float4 vPos = float4(vIn.p, 1.0f);
	vPos = mul(vPos, gWorld);
	vOut.p = mul(vPos, gViewProj);
	vOut.c = vIn.c;
	vOut.n = mul(vIn.n, (float3x3)gWorld);

	float4 texC = mul(float4(vIn.t, 0.0f, 1.0f), gTexTransform);
//	vOut.t = mul(texC, matData.MatTransform).xy;
	vOut.t = texC.xy;
	return vOut;
}

float4 PS(VertexOut vOut) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_samLinearWrap, vOut.t);
	return TexColor;
}

