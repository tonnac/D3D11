#include "common.hlsl"

struct VertexIn
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
	float3 TangentL : TANGENT;
};

struct VertexOut
{
	float4 p : SV_POSITION;
	float3 PosW : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
	float3 TangentW : TANGENT;
};

float4 PS(VertexOut vOut) : SV_Target
{
	float3 ambientLight = float3(0.35f, 0.35f, 0.35f);
	float4 texColor = vOut.c;

	vOut.n = normalize(vOut.n);

	float3 toEyeW = normalize(gEyePos - vOut.PosW);

	const float shininess = 1.0f - gRoughness;
	Material mat = { texColor, gDiffuse, gSpecular, gFresnelR0, shininess };
	float3 shadowFactor = 1.0f;

	float3 litColor = ComputeLighting(gLights, mat, vOut.PosW, vOut.n, toEyeW, shadowFactor, gDirctionNum, gPointNum, gSpotNum);
	float3 ambient = gAmbient.xyz * ambientLight;

	float3 finalColor =(ambient + litColor) * texColor.rgb;

	return float4(finalColor, 1.0f);
}