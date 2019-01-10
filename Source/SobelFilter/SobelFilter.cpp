#include "SobelFilter.h"
#include "ShaderStorage.h"

using Microsoft::WRL::ComPtr;

SobelFilter::SobelFilter(ID3D11Device * pd3Device, UINT width, UINT height, DXGI_FORMAT format, const std::wstring& shaderFile)
	: md3Device(pd3Device), mWidth(width), mHeight(height), mFormat(format)
{
	BuildResource();
	BuildView();
	BuildShader(shaderFile);
}

void SobelFilter::Execute(ID3D11DeviceContext * context, ID3D11ShaderResourceView** texture)
{
	UINT numGroupsX = (UINT)ceilf(mWidth / 16.0f);
	UINT numGroupsY = (UINT)ceilf(mHeight / 16.0f);

	context->CSSetShader(mComputeShader, nullptr, 0);
	context->CSSetShaderResources(0, 1, texture);
	context->CSSetUnorderedAccessViews(0, 1, mUav.GetAddressOf(), nullptr);
	context->Dispatch(numGroupsX, numGroupsY, 1);

	ID3D11UnorderedAccessView * uav[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	ID3D11ShaderResourceView * srv[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	context->CSSetShaderResources(0, (UINT)std::size(srv), srv);
	context->CSSetUnorderedAccessViews(0, (UINT)std::size(uav), uav, nullptr);
}

ID3D11ShaderResourceView ** SobelFilter::ShaderResrouceView()
{
	return mSrv.GetAddressOf();
}

void SobelFilter::BuildShader(const std::wstring& shaderFile)
{
	std::wstring name(shaderFile, 0, shaderFile.find_last_of('.'));
	ComPtr<ID3D11ComputeShader> compute;
	ComPtr<ID3DBlob> blob;

	blob = d3dUtil::CompileShaderFromFile(shaderFile.c_str(), nullptr, "SobelCS", "cs_5_0");
	md3Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, compute.GetAddressOf());

	mComputeShader = ShaderStorage::Storage()->AddComputeShader(compute, name);
}

void SobelFilter::BuildResource()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = mFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	md3Device->CreateTexture2D(&texDesc, nullptr, mTexture.GetAddressOf());
}

void SobelFilter::BuildView()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = mFormat;

	uavDesc.Format = mFormat;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	md3Device->CreateShaderResourceView(mTexture.Get(), &srvDesc, mSrv.GetAddressOf());
	md3Device->CreateUnorderedAccessView(mTexture.Get(), &uavDesc, mUav.GetAddressOf());
}
