#pragma once

#include "DxRT.h"

struct SobelFilter
{
public:
	SobelFilter(ID3D11Device* pd3Device, UINT width, UINT height, DXGI_FORMAT format, const std::wstring& shaderFile);
public:
	void OnResize(UINT width, UINT height);

	void Execute(ID3D11DeviceContext* context, ID3D11ShaderResourceView** texture);

	ID3D11ShaderResourceView** ShaderResrouceView();

private:
	void BuildShader(const std::wstring& shaderFile);
	void BuildResource();
	void BuildView();

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSrv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUav = nullptr;

	ID3D11ComputeShader * mComputeShader = nullptr;

	DXGI_FORMAT mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	ID3D11Device * md3Device = nullptr;

	UINT mWidth = -1;
	UINT mHeight = -1;
};
