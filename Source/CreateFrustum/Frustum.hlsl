

struct VertexIn
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
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
}

VertexOut VS(in VertexIn vIn)
{
	VertexOut vOut = (VertexOut)0.0f;
	float4 PosH = mul(float4(vIn.Pos, 1.0f), g_matView);
	PosH = mul(PosH, g_matProj);

	vOut.PosH = PosH;
	vOut.Color = vIn.Color;

	return vOut;
}

float4 PS(in VertexOut pIn) : SV_Target
{
	float4 C = pIn.Color;
	C.a = 0.5f;
	return C;
}