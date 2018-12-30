#include "MaterialStorage.h"

using namespace DirectX;

void Material::SetResource(ID3D11DeviceContext * context)
{
	context->PSSetShaderResources(0, 1, &ShaderResourceView);
	context->PSSetShaderResources(1, 1, &NormalView);
	context->VSSetConstantBuffers(2, 1, MaterialBuffer.GetAddressOf());
	context->PSSetConstantBuffers(2, 1, MaterialBuffer.GetAddressOf());
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

void MaterialStorage::UpdateMaterialCBs(ID3D11DeviceContext * context)
{
	for (auto&x : mMaterials)
	{
		if (x.second->NumFramesDirty > 0)
		{
			MaterialData matData;
			matData.Ambient = XMFLOAT4(x.second->Ambient.x, x.second->Ambient.y, x.second->Ambient.z, 1.0f);
			matData.Diffuse = XMFLOAT4(x.second->Diffuse.x, x.second->Diffuse.y, x.second->Diffuse.z, 1.0f);
			matData.Specular = XMFLOAT4(x.second->Specular.x, x.second->Specular.y, x.second->Specular.z, 1.0f);
			matData.FresnelR0 = x.second->FresnelR0;

			XMMATRIX M = XMLoadFloat4x4(&x.second->MatTransform);
			XMStoreFloat4x4(&matData.MatTransform, M);
			matData.Roughness = x.second->Roughness;

			x.second->MaterialBuffer;
			context->UpdateSubresource(x.second->MaterialBuffer.Get(), 0, nullptr, &matData, 0, 0);

			--x.second->NumFramesDirty;
		}
	}
}