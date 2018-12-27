#include "MaterialStorage.h"

void MaterialStorage::StoreMaterial(std::unique_ptr<Material>& material)
{
	material->MatCBIndex = (int)mMaterials.size();

	if ((int)mCache.size() > 3)
		mCache.pop_back();

	mCache.push_front(material.get());

	mMaterials.insert(std::make_pair(material->Name, std::move(material)));
}

Material * MaterialStorage::GetMaterial(const std::string & name)
{
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
