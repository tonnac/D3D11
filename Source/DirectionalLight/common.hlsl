#include "LightingUtil.hlsl"


cbuffer cbPass : register(b0)
{
	float4x4 gView;
	float4x4 gInvView;
	float4x4 gProj;
	float4x4 gInvProj;
	float4x4 gViewProj;
	float4x4 gInvViewProj;
	float3 gEyePos;
	float cbPerObjectPad1;
	float gNearZ;
	float gFarZ;
	float gTotalTime;
	float gDeltaTime;
	int gDirctionNum;
	int gPointNum;
	int gSpotNum;
	int cbPerObjectPad2;

	Light gLights[MaxLights];
};

cbuffer cbPerObj : register(b1)
{
	float4x4 gWorld;
	float4x4 gWoridInv;
	float4x4 gTexTransform;
}

cbuffer cbMaterial : register(b2)
{
	float4 gDiffuse;
	float4 gAmbient;
	float4 gSpecular;
	float3 gFresnelR0;
	float gRoughness;

	float4x4 gMatTransform;
}

cbuffer cbSkinned : register(b3)
{
	float4x4 gBoneTransforms[255];
}

Texture2D gTextureMap	: register (t0);

SamplerState g_samPointWrap        : register(s0);
SamplerState g_samPointClamp       : register(s1);
SamplerState g_samLinearWrap       : register(s2);
SamplerState g_samLinearClamp      : register(s3);
SamplerState g_samAnisotropicWrap  : register(s4);
SamplerState g_samAnisotropicClamp : register(s5);