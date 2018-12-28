#define MaxLights 16

struct Light
{
	float3 Strength;
	float FalloffStart;
	float3 Direction;
	float FalloffEnd;
	float3 Position;
	float SpotPower;
};

struct Material
{
	float4 TexColor;
	float4 Diffuse;
	float4 Specular;
	float3 FresnelR0;
	float Shininess;
	float4 Ambient;
};

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	const float m = mat.Shininess * 256.0f;
	float3 halfVec = normalize(toEye + lightVec);

	float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SclickFrensel(mat.FresnelR0, halfVec, lightVec);

	float3 specAlbedo = fresnelFactor * roughnessFactor;

	specAlbedo = specAlbedo / (specAlbedo + 1.0f);

	return (mat.Diffuse.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{
	float3 lightVec = -L.Direction;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.Strength * ndotl;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float4 ComputeLighting(Light gLights[MaxLights], Material mat,
					   float3 pos, float3 normal, float3 toEye,
					   float3 shadowFactor)
{
	float3 result = 0.0f;
	float3 ambient = 0.0f;

	for (int i = 0; i < gDirctionNum; ++i)
	{
		ambient = mat.Ambient * gLights[i].Strength;
		result += shadowFactor[i] * ComputeDirectionalLight(gLights[i], mat, normal, toEye) + ambient;
	}

	for (int i = gDirctionNum; i < gDirctionNum + gPointNum; ++i)
	{
		ambient = mat.Ambient * gLights[i].Strength;
		result += ComputePointLight(gLights[i], mat, pos, normal, toEye);
	}

	for (int i = gDirctionNum + gPointNum; i < gDirctionNum + gPointNum + gSpotNum; ++i)
	{
		ambient = mat.Ambient * gLights[i].Strength;
		result += ComputeSpotLight(gLights[i], mat, pos, normal, toEye);
	}

	return float4(result, 0.0f);
}