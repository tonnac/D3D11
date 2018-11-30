

struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct PC_IN
{
	float3 p : POSITION;
	float4 c : COLOR;
};

struct PC_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

cbuffer cbPass : register(b0)
{
	float4x4 g_matView;
	float4x4 InvView;
	float4x4 g_matProj;
	float4x4 InvProj;
	float4x4 ViewProj;
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
	float4x4 g_matWorld;
}

Texture2D g_txDiffuse	: register (t0);
SamplerState g_SamState	: register (s0);