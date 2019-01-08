#include "common.hlsl"

struct VertexIn
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
	float3 TangentL : TANGENT;
#ifdef SKINNED
	float4 BoneWeights  : WEIGHTS;
	uint4  BoneIndices  : BONEINDICES;
#endif
};

struct VertexOut
{
	float3 PosW : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
	float3 TangentW : TANGENT;
};

struct GeoOut
{
	float4 p : SV_POSITION;
	float3 PosW : POSITION;
	float4 c : COLOR;
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
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);

	for (uint i = 0; i < Index; ++i)
	{
		posL += weights[i] * mul(float4(vIn.p, 1.0f), gBoneTransforms[vIn.BoneIndices[i]]).xyz;
		normalL += weights[i] * mul(vIn.n, (float3x3)gBoneTransforms[vIn.BoneIndices[i]]);
		tangentL += weights[i] * mul(vIn.TangentL, (float3x3)gBoneTransforms[vIn.BoneIndices[i]]);
	}

	vIn.p = posL;
	vIn.n = normalL;
	vIn.TangentL = tangentL;
#endif

	float4 vPos = float4(vIn.p, 1.0f);
	float4 vPosW = mul(vPos, gWorld);
	vOut.PosW = vPosW.xyz;
	vOut.c = vIn.c;
	vOut.n = mul(vIn.n, (float3x3)gWorld);
	vOut.TangentW = mul(vIn.TangentL, (float3x3)gWorld);

	float4 texC = mul(float4(vIn.t, 0.0f, 1.0f), gTexTransform);
	texC = mul(texC, gMatTransform);
	vOut.t = texC.xy;
	return vOut;
}

//[maxvertexcount(2)]
//void GS(point VertexOut gin[1],
//	inout LineStream<GeoOut> lineStream)
//{
//	GeoOut gout[2];
//
//	gout[0] = (GeoOut)0.0f;
//	gout[0].PosW = gin[0].PosW;
//	gout[0].p = mul(float4(gout[0].PosW, 1.0f), gViewProj);
//
//
//	gout[1] = (GeoOut)0.0f;
//	gout[1].PosW = gin[0].PosW + (5.0f * gin[0].n);
//	gout[1].p = mul(float4(gout[1].PosW, 1.0f), gViewProj);
//
//
//	lineStream.Append(gout[0]);
//	lineStream.Append(gout[1]);
//}

[maxvertexcount(6)]
void GS(triangle VertexOut gin[3],
	inout LineStream<GeoOut> lineStream)
{
	float3 PosW = (gin[0].PosW + gin[1].PosW + gin[2].PosW) / 3.0f;
	float3 NormalW = normalize((gin[0].n + gin[1].n + gin[2].n) / 3.0f);

	GeoOut gout[2];
	gout[0].PosW = PosW;
	gout[0].p = mul(float4(gout[0].PosW, 1.0f), gViewProj);
	gout[0].c = float4(1.0f, 1.0f, 1.0f, 1.0f);

	gout[1].PosW = PosW + NormalW * 15.0f;
	gout[1].p = mul(float4(gout[1].PosW, 1.0f), gViewProj);
	gout[1].c = float4(0.0f, 0.0f, 0.0f, 1.0f);

	lineStream.Append(gout[0]);
	lineStream.Append(gout[1]);

//	[unroll]
	//for (int i = 0; i < 6; ++i)
	//{
	//	GeoOut gout = (GeoOut)0.0f;
	//	gout.PosW = gin[i / 2].PosW;
	//	gout.c = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//	if (i % 2 == 1)
	//	{
	//		gout.PosW += normalize(gin[i / 2].n) * 15.0f;
	//		gout.c = float4(0.0f, 0.0f, 0.0f, 1.0f);
	//	}
	//	gout.p = mul(float4(gout.PosW, 1.0f), gViewProj);
	//	lineStream.Append(gout);
	//}
}


float4 PS(GeoOut pin) : SV_Target
{
	return pin.c;
}
