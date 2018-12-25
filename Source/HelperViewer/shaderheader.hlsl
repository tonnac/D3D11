

struct VertexIn
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
#ifdef SKINNED
	float4 BoneWeights : WEIGHTS;
	uint4 BoneIndices  : BONEINDICES;
#endif
};

struct VertexOut
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct CVertexIn
{
	float3 p : POSITION;
	float4 c : COLOR;
};

struct CVertexOut
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

cbuffer cbPass : register(b0)
{
	float4x4 gView;
	float4x4 InvView;
	float4x4 gProj;
	float4x4 InvProj;
	float4x4 gViewProj;
	float4x4 InvViewProj;
	float3 EyePos;
	float pad1;
	float NearZ;
	float FarZ;
	float TotalTime;
	float DeltaTime;
};

cbuffer cbPerObj : register(b1)
{
	float4x4 gWorld;
	float4x4 gTexTransform;
}

cbuffer cbSkinned : register(b2)
{
	float4x4 gBoneTransforms[255];
}

Texture2D g_txDiffuse	: register (t0);

SamplerState g_samPointWrap        : register(s0);
SamplerState g_samPointClamp       : register(s1);
SamplerState g_samLinearWrap       : register(s2);
SamplerState g_samLinearClamp      : register(s3);
SamplerState g_samAnisotropicWrap  : register(s4);
SamplerState g_samAnisotropicClamp : register(s5);