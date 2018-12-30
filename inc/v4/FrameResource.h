#pragma once
#include "d3dUtil.h"
#include "MathHelper.h"

constexpr UINT MaxBoneNum = 255;
constexpr UINT MaxLights = 16;

struct Light
{
	DirectX::XMFLOAT3 Strength = {0.5f, 0.5f, 0.5f};
	float FalloffStart = 1.0f;
	DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };
	float FalloffEnd = 10.0f;
	DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };
	float SpotPower = 64.0f;
};

struct PassConstants
{
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;
	int DirectionalNum = 0;
	int PointNum = 0;
	int SpotNum = 0;
	int cbPerObjectPad2 = 0;

	Light Lights[MaxLights];
};

struct SkinnedConstants
{
	DirectX::XMFLOAT4X4 BoneTransforms[MaxBoneNum];
};

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 WorldInv = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();
};

struct MaterialData
{
	DirectX::XMFLOAT4 Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 Ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 Specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 FresnelR0 = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);
	float Roughness = .25f;

	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};