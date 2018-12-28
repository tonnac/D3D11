#pragma once
#include "SRVStorage.h"
#include "MathHelper.h"

struct Material
{
	std::wstring Name;

	int MatCBIndex = -1;

	int NumFramesDirty;

	ID3D11ShaderResourceView* ShaderResourceView = nullptr;
	ID3D11ShaderResourceView* NormalView = nullptr;

	DirectX::XMFLOAT3 Diffuse = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 Ambient = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 Specular = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 FresnelR0 = DirectX::XMFLOAT3(0.03f, 0.03f, 0.03f);
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

	Material* GetMaterial(const std::wstring& name);
private:
	UINT MaterialIndex = -1;

	std::list<Material*> mCache;
	std::unordered_map<std::wstring, std::unique_ptr<Material>> mMaterials;
};