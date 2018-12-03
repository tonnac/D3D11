
struct VertexOut
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD;
};

static const float2 gTexCoords[6] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f)
};

Texture2D g_txDiffuse : register (t0);

SamplerState g_samPointWrap        : register(s0);
SamplerState g_samPointClamp       : register(s1);
SamplerState g_samLinearWrap       : register(s2);
SamplerState g_samLinearClamp      : register(s3);
SamplerState g_samAnisotropicWrap  : register(s4);
SamplerState g_samAnisotropicClamp : register(s5);

VertexOut VS(uint vertID : SV_VertexID)
{
	VertexOut vOut = (VertexOut)0.0f;

	vOut.t = gTexCoords[vertID];

	vOut.p = float4(2.0f * vOut.t.x - 1.0f, 1.0f - 2.0f * vOut.t.y, 0.0f, 1.0f);

	return vOut;
}

float4 PS(in VertexOut pIn) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_samPointWrap, pIn.t);
	return TexColor;
}