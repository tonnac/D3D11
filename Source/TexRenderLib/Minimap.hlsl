

struct VertexIn
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

struct VertexOut
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

Texture2D g_txDiffuse : register (t0);

SamplerState g_samPointWrap        : register(s0);
SamplerState g_samPointClamp       : register(s1);
SamplerState g_samLinearWrap       : register(s2);
SamplerState g_samLinearClamp      : register(s3);
SamplerState g_samAnisotropicWrap  : register(s4);
SamplerState g_samAnisotropicClamp : register(s5);

VertexOut VS(in VertexIn vIn, uint vertID : SV_VertexID)
{
	VertexOut vOut = (VertexOut)0.0f;
	vOut.p = float4(vIn.p, 1.0f);
	vOut.n = vIn.n;
	vOut.c = vIn.c;
	vOut.t = vIn.t;
	return vOut;
}

float4 PS(in VertexOut pIn) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_samLinearClamp, pIn.t);
	return TexColor;
}