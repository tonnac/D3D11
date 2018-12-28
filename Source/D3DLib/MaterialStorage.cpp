#include "MaterialStorage.h"

void Material::SetResource(ID3D11DeviceContext * context)
{
	context->PSSetShaderResources(0, 1, &ShaderResourceView);
	context->PSSetShaderResources(1, 1, &NormalView);
}


void MaterialStorage::StoreMaterial(std::unique_ptr<Material>& material)
{
	material->MatCBIndex = ++MaterialIndex;

	if ((int)mCache.size() > 3)
		mCache.pop_back();

	mCache.push_front(material.get());

	mMaterials.insert(std::make_pair(material->Name, std::move(material)));
}

Material * MaterialStorage::GetMaterial(const std::wstring & name)
{
	if (name.empty())
		return nullptr;

	for (auto & x : mCache)
	{
		if (x->Name == name)
			return x;
	}

	auto iter = mMaterials.find(name);
	if (iter == mMaterials.end())
		return nullptr;

	if ((int)mCache.size() > 3)
		mCache.pop_back();

	mCache.push_front(iter->second.get());

	return iter->second.get();
}

