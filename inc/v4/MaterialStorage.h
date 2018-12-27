#pragma once
#include "d3dUtil.h"
#include "MathHelper.h"

struct Material
{
	std::string Name;

	int MatCBIndex = -1;

	int NumFramesDirty;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> NormalView = nullptr;

	DirectX::XMFLOAT4 Diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 Ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 Specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 FresnelR0 = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);
	float Roughness = .25f;
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};


class MaterialStorage
{
private:
	MaterialStorage() = default;

public:
	static MaterialStorage* GetStorage()
	{
		static MaterialStorage storage;
		return &storage;
	}

public:
	void StoreMaterial(std::unique_ptr<Material>& material);

	Material* GetMaterial(const std::string& name);
private:
	std::list<Material*> mCache;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
};