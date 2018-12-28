#pragma once
#include "d3dUtil.h"

class SrvStorage
{
private:
	SrvStorage() = default;

public:
	static SrvStorage* GetStorage()
	{
		static SrvStorage storage;
		return &storage;
	}
	void SetDevice(ID3D11Device* device);

public:
	ID3D11ShaderResourceView * LoadSRV(const std::wstring& fileName);

private:
	ID3D11Device* md3Device = nullptr;

	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> mMaterials;
};