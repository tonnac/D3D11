#pragma once

#include "DxRT.h"

struct SobelFilter
{
public:
	void Initialize(ID3D11Device* pd3Device, UINT width, UINT height);


	void Execute(ID3D11DeviceContext* context, ID3D11ShaderResourceView** texture);

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSrv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUav = nullptr;
};
