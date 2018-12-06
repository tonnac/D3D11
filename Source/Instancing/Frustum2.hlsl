


struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};

Texture2D g_txDiffuse	: register (t0);

float4 PS(VS_OUTPUT pIn) : SV_Target
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}
