#include "Device.h"
#include "DirectWrite.h"
void Device::InitDevice()
{
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	CreateDevice();
	CreateFactory();
	CreateSwapChain();
	SetRenderTargetView();
	SetViewPort();

	UINT Flags = DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES;
	m_pDXGIFactory->MakeWindowAssociation(g_hWnd, Flags);
}
void Device::CleanupDevice()
{
	m_pContext->ClearState();
	RELEASE(m_pContext);
	RELEASE(m_pRenderTargetView);
	RELEASE(m_pSwapChain);
	RELEASE(m_pDXGIFactory);
	RELEASE(m_pDevice);
}
void Device::ResizeDevice(const LONG& Width, const LONG& Height)
{
	if (m_pContext == nullptr) return;
	m_pContext->OMSetRenderTargets(0, nullptr, nullptr);
	
	S_Write.DiscardDP_Device();

	RELEASE(m_pRenderTargetView);

	m_SwapChainDesc.BufferDesc.Height = g_rtClient.bottom - g_rtClient.top;
	m_SwapChainDesc.BufferDesc.Width = g_rtClient.right - g_rtClient.left;

	m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, m_SwapChainDesc.BufferDesc.Width,
		m_SwapChainDesc.BufferDesc.Height, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);

	SetRenderTargetView();
	SetViewPort();
	S_Write.CreateDP_Device(m_pSwapChain);
}
void Device::CreateDevice()
{
	UINT DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; //Direct Write
#ifdef _DEBUG
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	int FeatureNum = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);
	D3D_DRIVER_TYPE driverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	int DriverNum = sizeof(driverType) / sizeof(D3D_DRIVER_TYPE);

	HRESULT hr;
	for (int i = 0; i < DriverNum; ++i)
	{
		D3D_DRIVER_TYPE dp = driverType[i];
		hr = D3D11CreateDevice(nullptr, dp, nullptr, DeviceFlags, featureLevels, FeatureNum, D3D11_SDK_VERSION
			, &m_pDevice, &m_FeatureLevel, &m_pContext);
		if (FAILED(hr))
		{
			RELEASE(m_pDevice);
			RELEASE(m_pContext);
			ThrowifFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, DeviceFlags, featureLevels, FeatureNum, D3D11_SDK_VERSION
				, &m_pDevice, &m_FeatureLevel, &m_pContext));
		}
	}
}
void Device::CreateFactory()
{
	if (m_pDevice == nullptr) return;
	IDXGIDevice * pDXGIDevice = nullptr;
	ThrowifFailed(m_pDevice->QueryInterface(IID_PPV_ARGS(&pDXGIDevice)));

	IDXGIAdapter * pDXGIAdapter = nullptr;
	ThrowifFailed(pDXGIDevice->GetParent(IID_PPV_ARGS(&pDXGIAdapter)));

	ThrowifFailed(pDXGIAdapter->GetParent(IID_PPV_ARGS(&m_pDXGIFactory)));
	RELEASE(pDXGIDevice);
	RELEASE(pDXGIAdapter);
}
void Device::CreateSwapChain()
{
	DXGI_MODE_DESC modedesc;
	ZeroMemory(&modedesc, sizeof(DXGI_MODE_DESC));
	modedesc.Width = g_rtClient.right - g_rtClient.left;
	modedesc.Height = g_rtClient.bottom - g_rtClient.top;
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

	ThrowifFailed(m_pDXGIFactory->CreateSwapChain(m_pDevice, &m_SwapChainDesc, &m_pSwapChain));
}
void Device::SetRenderTargetView()
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	ThrowifFailed(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));
	ThrowifFailed(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView));
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	RELEASE(pBackBuffer);
}
void Device::SetViewPort()
{
	m_ViewPort.Width = CASTING(FLOAT, m_SwapChainDesc.BufferDesc.Width);
	m_ViewPort.Height = CASTING(FLOAT, m_SwapChainDesc.BufferDesc.Height);
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pContext->RSSetViewports(1, &m_ViewPort);
}
