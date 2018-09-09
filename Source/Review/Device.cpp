#include "Device.h"

Device::Device() : m_pd11Device(nullptr), m_pDXGIFactory(nullptr), m_pSwapChain(nullptr), m_pImmediateContext(nullptr),
m_pRenderTargetView(nullptr)
{
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
}

HRESULT	Device::CreateDevice()
{
	HRESULT hr;

	UINT Flags = 0;

	Flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	int driverNum = sizeof(driverType) / sizeof(D3D_DRIVER_TYPE);

	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	UINT featureNum = sizeof(featureLevel) / sizeof(D3D_FEATURE_LEVEL);

	for (int idriver = 0; idriver < driverNum; ++idriver)
	{
		D3D_DRIVER_TYPE dType = driverType[idriver];
		hr = D3D11CreateDevice(nullptr, 
			dType, nullptr, 
			Flags, featureLevel, 
			featureNum, D3D11_SDK_VERSION, &m_pd11Device, &m_FeatureLevel, 
			&m_pImmediateContext);
		if (SUCCEEDED(hr))
		{
			if (m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
			{
				if (m_pd11Device) m_pd11Device->Release();
				if (m_pImmediateContext) m_pImmediateContext->Release();
				continue;
			}
			break;
		}
	}
	return hr;
}
HRESULT	Device::CreateFactory()
{
	if (m_pd11Device == nullptr) return E_FAIL;
	HRESULT hr;

	IDXGIDevice * pDXGIDevice = nullptr;
	hr = m_pd11Device->QueryInterface(__uuidof(IDXGIDevice), VOIDPTR(pDXGIDevice));
	     
	IDXGIAdapter * pDXGIAdapter = nullptr;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), VOIDPTR(pDXGIAdapter));

	hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), VOIDPTR(m_pDXGIFactory));
	
	pDXGIDevice->Release();
	pDXGIAdapter->Release();

	return hr;
}
HRESULT	Device::CreateSwapChain()
{
	HRESULT hr;

	DXGI_MODE_DESC ModeDesc;
	ZeroMemory(&ModeDesc, sizeof(DXGI_MODE_DESC));
	ModeDesc.Width = g_rtClient.right - g_rtClient.left;
	ModeDesc.Height = g_rtClient.bottom - g_rtClient.top;
	ModeDesc.RefreshRate.Numerator = 60;
	ModeDesc.RefreshRate.Denominator = 1;
	ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_SwapChainDesc.BufferDesc = ModeDesc;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.OutputWindow = g_hWnd;
	m_SwapChainDesc.Windowed = TRUE;
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXFAIL(m_pDXGIFactory->CreateSwapChain(m_pd11Device, &m_SwapChainDesc, &m_pSwapChain));
	return hr;
}
HRESULT	Device::SetRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer = nullptr;

	DXFAIL(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), VOIDPTR(pBackBuffer)));
	DXFAIL(m_pd11Device->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView));
	pBackBuffer->Release();

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	return hr;
}
void Device::SetViewPort()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = CASTING(FLOAT, m_SwapChainDesc.BufferDesc.Width);
	m_ViewPort.Height = CASTING(FLOAT, m_SwapChainDesc.BufferDesc.Height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
}
bool Device::CleanupDevice()
{
	m_pImmediateContext->ClearState();
	RELEASE(m_pRenderTargetView);
	RELEASE(m_pSwapChain);
	RELEASE(m_pImmediateContext);
	RELEASE(m_pDXGIFactory);
	RELEASE(m_pd11Device);
	return true;
}

HRESULT Device::ResizeDevice(const UINT& iWidth, const UINT& iHeight)
{
	if (m_pd11Device == nullptr) return E_FAIL;
	HRESULT hr;

	m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	//DirectWrite
	ResizeDiscard();

	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	
	m_SwapChainDesc.BufferDesc.Height = iHeight;
	m_SwapChainDesc.BufferDesc.Width = iWidth;
	m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, iWidth, iHeight, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);

	GetClientRect(g_hWnd, &g_rtClient);

	DXFAIL(SetRenderTargetView());
	SetViewPort();

	//DirectWrite
	ResizeCreate();
	return hr;
}
void Device::ResizeDiscard()
{
	return;
}
void Device::ResizeCreate()
{
	return;
}
ID3D11Device *	Device::getDevice()
{
	GETPTR(m_pd11Device);
}
ID3D11DeviceContext* Device::getContext()
{
	GETPTR(m_pImmediateContext);
}
IDXGISwapChain*	Device::getSwapChain()
{
	GETPTR(m_pSwapChain);
}
IDXGISwapChain** Device::getSwapChainPtr()
{
	return &m_pSwapChain;
}
ID3D11RenderTargetView*	Device::getRenderTargetView()
{
	GETPTR(m_pRenderTargetView);
}
IDXGIFactory* Device::getDXGIFactory()
{
	GETPTR(m_pDXGIFactory);
}
DXGI_SWAP_CHAIN_DESC Device::getSwapChainDesc()
{
	return m_SwapChainDesc;
}