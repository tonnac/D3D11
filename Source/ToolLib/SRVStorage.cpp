#include "SRVStorage.h"

using Microsoft::WRL::ComPtr;

void SrvStorage::SetDevice(ID3D11Device * device)
{
	md3Device = device;
}

ID3D11ShaderResourceView * SrvStorage::LoadSRV(const std::wstring& fileName)
{
	std::wstring file(fileName, fileName.find_last_of('\\') + 1, fileName.length());

	if (mMaterials.find(file) != mMaterials.end())
	{
		return mMaterials[file].Get();
	}

	ComPtr<ID3D11ShaderResourceView> Srv;

	d3dUtil::CreateShaderResourceView(md3Device, fileName, Srv.GetAddressOf());

	mMaterials.insert(std::make_pair(file, std::move(Srv)));

	return mMaterials[file].Get();
}
