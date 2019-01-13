#include "..\Picking\Sample.h"
#include "..\Picking\Sample.h"

#include "Sample.h"
#include "DirectInput.h"

#include <chrono>

using namespace std::chrono;

using namespace DirectX;

struct Data
{
	XMFLOAT3 v1;
	XMFLOAT2 v2;
};

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{
	std::locale::global(std::locale("Korean"));
}

bool Sample::Init()
{
	BuildBuffer();
	BuildView();

	Microsoft::WRL::ComPtr<ID3DBlob> blob =	d3dUtil::CompileShaderFromFile(L"Computeshader.hlsl", nullptr, "CS", "cs_5_0");

	m_pd3dDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mComputeShader.GetAddressOf());

	m_pImmediateContext->CSSetShader(mComputeShader.Get(), nullptr, 0);
	m_pImmediateContext->CSSetShaderResources(0, 1, mDataAView.GetAddressOf());
	m_pImmediateContext->CSSetShaderResources(1, 1, mDataBView.GetAddressOf());
	m_pImmediateContext->CSSetUnorderedAccessViews(0, 1, mUav.GetAddressOf(), nullptr);
	m_pImmediateContext->Dispatch(1, 1, 1);

	m_pImmediateContext->CopyResource(mReadBack.Get(), mOutput.Get());

	D3D11_MAPPED_SUBRESOURCE mapped;
	Data* p = nullptr;
	m_pImmediateContext->Map(mReadBack.Get(), 0, D3D11_MAP_READ, 0, &mapped);

	p = (Data*)mapped.pData;

	std::ofstream fout("Data.txt");

	for (int i = 0; i < 20; ++i)
	{
		std::string s1 = std::to_string(i) + " v1 : " + std::to_string(p[i].v1.x) + " " + std::to_string(p[i].v1.y) + " " + std::to_string(p[i].v1.z);
		std::string s2 = std::to_string(i) + " v2 : " + std::to_string(p[i].v2.x) + " " + std::to_string(p[i].v2.y);
		fout << s1 << std::endl << s2 << std::endl << std::endl;
	}

	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	return true;
}

void Sample::BuildBuffer()
{
	std::vector<Data> dataA(20);
	std::vector<Data> dataB(20);

	for (int i = 0; i < 20; ++i)
	{
		dataA[i].v1 = XMFLOAT3(i, i, i);
		dataA[i].v2 = XMFLOAT2(i, 0);

		dataB[i].v1 = XMFLOAT3(2 * i, -i, i + 2);
		dataB[i].v2 = XMFLOAT2(0, -i);
	}

	const UINT byteSize = sizeof(Data) * (UINT)dataA.size();

	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufDesc.ByteWidth = byteSize;
	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufDesc.StructureByteStride = sizeof(Data);

	D3D11_SUBRESOURCE_DATA sd;
	
	sd.pSysMem = dataA.data();
	sd.SysMemPitch = byteSize;
	sd.SysMemSlicePitch = byteSize;
	m_pd3dDevice->CreateBuffer(&bufDesc, &sd, mDataA.GetAddressOf());

	sd.pSysMem = dataB.data();
	m_pd3dDevice->CreateBuffer(&bufDesc, &sd, mDataB.GetAddressOf());





	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;

	m_pd3dDevice->CreateBuffer(&bufDesc, nullptr, mOutput.GetAddressOf());

	D3D11_BUFFER_DESC bufDesc0;
	ZeroMemory(&bufDesc0, sizeof(D3D11_BUFFER_DESC));
	bufDesc0.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bufDesc0.Usage = D3D11_USAGE_STAGING;
	bufDesc0.ByteWidth = byteSize;
	bufDesc0.StructureByteStride = sizeof(Data);

	m_pd3dDevice->CreateBuffer(&bufDesc0, nullptr, mReadBack.GetAddressOf());
}

void Sample::BuildView()
{
	D3D11_BUFFER_DESC bufDesc;
	mDataA->GetDesc(&bufDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.BufferEx.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

	m_pd3dDevice->CreateShaderResourceView(mDataA.Get(), &srvDesc, mDataAView.GetAddressOf());
	m_pd3dDevice->CreateShaderResourceView(mDataB.Get(), &srvDesc, mDataBView.GetAddressOf());

	mOutput->GetDesc(&bufDesc);
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

	m_pd3dDevice->CreateUnorderedAccessView(mOutput.Get(), &uavDesc, mUav.GetAddressOf());
}

void Sample::Pick(int sx, int sy)
{
}

void Sample::OnResize()
{
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR szCmdLine, int nCmdShow)
{
	try
	{
		Sample sd(hInstance, 800, 600, L"FirstWindow");

		if (!sd.Initialize())
			return 0;

		return sd.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}