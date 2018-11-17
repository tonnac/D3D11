#include "Device.h"

using Microsoft::WRL::ComPtr;
using namespace std;

Device::Device()
{
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
}

bool Device::InitDirect3D()
{
	UINT DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	ThrowifFailed(CreateDXGIFactory(IID_PPV_ARGS(m_pdxgiFactory.GetAddressOf())));

	D3D_FEATURE_LEVEL featureLevel[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT hardwareResult = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		DeviceFlags, featureLevel, (UINT)std::size(featureLevel),
		D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(),
		&m_Featurelevel, m_pImmediateContext.GetAddressOf());

	if (FAILED(hardwareResult))
	{
		ThrowifFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
		DeviceFlags, featureLevel, (UINT)std::size(featureLevel),
		D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(),
		&m_Featurelevel, m_pImmediateContext.GetAddressOf()));
	}

#ifdef _DEBUG
	LogAdapters();
#endif

	CreateSwapChain();

	return true;
}

void Device::OnResize()
{
	assert(m_pd3dDevice);
	assert(m_pSwapchain);

	m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	m_pRenderTargetView.Reset();
	m_pDepthStencilView.Reset();

	m_SwapChainDesc.BufferDesc.Width = g_ClientWidth;
	m_SwapChainDesc.BufferDesc.Height = g_ClientHeight;

	ThrowifFailed(m_pSwapchain->ResizeBuffers(
		m_SwapChainDesc.BufferCount,
		m_SwapChainDesc.BufferDesc.Width,
		m_SwapChainDesc.BufferDesc.Height,
		m_SwapChainDesc.BufferDesc.Format,
		m_SwapChainDesc.Flags));

	SetRenderTargetView();
	SetDepthStencilView();
	SetViewport();
}


void Device::CreateSwapChain()
{
	m_pSwapchain.Reset();

	DXGI_MODE_DESC modedesc;
	ZeroMemory(&modedesc, sizeof(DXGI_MODE_DESC));
	modedesc.Width = g_ClientWidth;
	modedesc.Height = g_ClientHeight;
	modedesc.RefreshRate.Denominator = 1;
	modedesc.RefreshRate.Numerator = 60;
	modedesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_SwapChainDesc.BufferDesc = modedesc;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.OutputWindow = g_hWnd;
	m_SwapChainDesc.Windowed = TRUE;
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowifFailed(m_pdxgiFactory->CreateSwapChain(m_pd3dDevice.Get(), &m_SwapChainDesc, m_pSwapchain.GetAddressOf()));
}

void Device::SetRenderTargetView()
{
	ComPtr<ID3D11Texture2D> pTexture = nullptr;
	ThrowifFailed(m_pSwapchain->GetBuffer(0, IID_PPV_ARGS(pTexture.GetAddressOf())));
	ThrowifFailed(m_pd3dDevice->CreateRenderTargetView(
		pTexture.Get(),
		nullptr,
		m_pRenderTargetView.GetAddressOf()));
}

void Device::SetDepthStencilView()
{
	ComPtr<ID3D11Texture2D> pTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	td.Width = m_SwapChainDesc.BufferDesc.Width;
	td.Height = m_SwapChainDesc.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	ThrowifFailed(m_pd3dDevice->CreateTexture2D(&td, nullptr, pTexture.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	dsvd.Texture2D.MipSlice = 0;

	ThrowifFailed(m_pd3dDevice->CreateDepthStencilView(
		pTexture.Get(),
		&dsvd,
		m_pDepthStencilView.GetAddressOf()));
}

void Device::SetViewport()
{
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = Casting(float, m_SwapChainDesc.BufferDesc.Width);
	m_Viewport.Height = Casting(float, m_SwapChainDesc.BufferDesc.Height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
}

void Device::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* pAdapter = nullptr;
	vector<IDXGIAdapter*> AdapterList;
	while (m_pdxgiFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		pAdapter->GetDesc(&desc);

		tstring text = L"***Adapter: " + tstring(desc.Description) + L"\n";

		OutputDebugString(text.c_str());

		AdapterList.push_back(pAdapter);

		++i;
	}

	for (size_t i = 0; i < AdapterList.size(); ++i)
	{
		LogAdapterOutput(AdapterList[i]);
		ReleaseCom(AdapterList[i]);
	}

}
	 
void Device::LogAdapterOutput(IDXGIAdapter* pAdapter)
{
	UINT i = 0;
	IDXGIOutput* pOutput = nullptr;
	
	while (pAdapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		pOutput->GetDesc(&desc);

		tstring text = L"***Output: " + tstring(desc.DeviceName) + L"\n";

		OutputDebugString(text.c_str());

		LogOutputDisplayModes(pOutput, DXGI_FORMAT_R8G8B8A8_UNORM);

		ReleaseCom(pOutput);

		++i;
	}
}
	 
void Device::LogOutputDisplayModes(IDXGIOutput* pOutput, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	pOutput->GetDisplayModeList(format, flags, &count, nullptr);

	vector<DXGI_MODE_DESC> modeList(count);
	pOutput->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto & x : modeList)
	{
		UINT RefreshRate = x.RefreshRate.Numerator / x.RefreshRate.Denominator;

		tstring text = L"Width = " + to_tstring(x.Width) + L" " + L"Height = " + to_tstring(x.Height) + L" " +
			L"RefreshRate = " + to_wstring(RefreshRate) + L"\n";

		::OutputDebugString(text.c_str());
	}
}