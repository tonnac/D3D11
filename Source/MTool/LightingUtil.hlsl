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
	float4 gSpecular;
	float3 FresnelR0;
	float Shininess;
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
	return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
	float cosIncidentAngle = saturate(dot(normal, lightVec));

	float f0 = 1.0f - cosIncidentAngle;
	float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

	return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
	const float m = mat.Shininess * 256.0f;
	float3 halfVec = normalize(toEye + lightVec);

	float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);
	
	float3 specAlbedo = fresnelFactor * roughnessFactor * mat.gSpecular.rgb;

	specAlbedo = specAlbedo / (specAlbedo + 1.0f);

	return (mat.Diffuse.rgb * mat.TexColor.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{
	float3 lightVec = -L.Direction;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.Strength * ndotl;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 ComputePointLight(Light L, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 lightVec = L.Position - pos;

	float d = length(lightVec);

	if (d > L.FalloffEnd)
		return 0.0f;

	lightVec /= d;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.Strength * ndotl;

	float att = CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);
	lightStrength *= att;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 ComputeSpotLight(Light L, Material mat, float3 pos, float3 normal, float3 toEye)
{
	float3 lightVec = L.Position - pos;

	float d = length(lightVec);

	if (d > L.FalloffEnd)
		return 0.0f;

	lightVec /= d;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.Strength * ndotl;

	float att = CalcAttenuation(d, L.FalloffStart, L.FalloffEnd);
	lightStrength *= att;

	float spotFactor = pow(max(dot(-lightVec, L.Direction), 0.0f), L.SpotPower);
	lightStrength *= spotFactor;

	return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float3 ComputeLighting(Light gLights[MaxLights], Material mat,
					   float3 pos, float3 normal, float3 toEye,
					   float3 shadowFactor, int DirectNum, int PointNum, int SpotNum)
{
	float3 result = 0.0f;
	float3 ambient = 0.0f;

	int i = 0;

	for (i = 0; i < DirectNum; ++i)
	{
		result += shadowFactor[i] * ComputeDirectionalLight(gLights[i], mat, normal, toEye);
	}

	for (i = DirectNum; i < DirectNum + PointNum; ++i)
	{
		result += ComputePointLight(gLights[i], mat, pos, normal, toEye);
	}

	for (i = DirectNum + PointNum; i < DirectNum + PointNum + SpotNum; ++i)
	{
		result += ComputeSpotLight(gLights[i], mat, pos, normal, toEye);
	}

	return result;
}