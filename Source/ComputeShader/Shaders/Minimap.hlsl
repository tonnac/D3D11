
struct VertexOut
{
};

struct GeoOut
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD;
};

Texture2D g_txDiffuse : register (t0);

SamplerState g_samPointWrap        : register(s0);
SamplerState g_samPointClamp       : register(s1);
SamplerState g_samLinearWrap       : register(s2);
SamplerState g_samLinearClamp      : register(s3);
SamplerState g_samAnisotropicWrap  : register(s4);
SamplerState g_samAnisotropicClamp : register(s5);

VertexOut VS()
{
	VertexOut vOut = (VertexOut)0.0f;
	return vOut;
}

[maxvertexcount(4)]
void GS(in point VertexOut p[1],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<GeoOut> tristream)
{
	const float2 TexCoords[4] =
	{
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f),
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
	};

	GeoOut gOut[4];
	for (int i = 0; i < 4; ++i)
	{
		gOut[i].t = TexCoords[i];
		gOut[i].p = float4(2.0f * gOut[i].t.x - 1.0f, 1.0f - 2.0f * gOut[i].t.y, 0.0f, 1.0f);
	}

	tristream.Append(gOut[0]);
	tristream.Append(gOut[1]);
	tristream.Append(gOut[2]);
	tristream.Append(gOut[3]);
}

float4 PS(in GeoOut pIn) : SV_Target
{
	float4 TexColor = g_txDiffuse.Sample(g_samPointWrap, pIn.t);
	return TexColor;
}