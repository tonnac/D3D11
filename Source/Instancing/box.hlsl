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
	float4x4 g_matWorld1;
}

struct InstanceData
{
	float4x4 g_matWorld;
};

SamplerState g_samPointWrap        : register(s0);
SamplerState g_samPointClamp       : register(s1);
SamplerState g_samLinearWrap       : register(s2);
SamplerState g_samLinearClamp      : register(s3);
SamplerState g_samAnisotropicWrap  : register(s4);
SamplerState g_samAnisotropicClamp : register(s5);

Texture2D g_txDiffuse : register (t0);

StructuredBuffer<InstanceData> gInstanceData : register(t1);

VS_OUTPUT VS(uint instanceID : SV_InstanceID, VS_IN vIn)
{
	VS_OUTPUT vOut;

	InstanceData instData = gInstanceData[instanceID];

	float4 vPos = float4(vIn.p, 1.0f);
	float4 vWorld = mul(vPos, instData.g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.c = vIn.c;
	vOut.n = vIn.n;
	vOut.t = vIn.t;
	return vOut;
}

float4 PS(VS_OUTPUT vOut) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_samLinearWrap, vOut.t);
	return TexColor;
}

