#include "Device.h"

Device::Device() : m_pd3dDevice(nullptr), m_pSwapChain(nullptr), m_pRenderTargetView(nullptr),
m_pImmediateContext(nullptr), m_pGIFactory(nullptr), m_bFullScreenMode(FALSE)
{
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
}
HRESULT Device::CreateDevice()
{
	HRESULT hr;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // DirectWrite와 연동
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT driverNum = sizeof(driverTypes) / sizeof(D3D_DRIVER_TYPE);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT featureNum = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);

	for (UINT iDriver = 0; iDriver < driverNum; ++iDriver)
	{
		m_DriverType = driverTypes[iDriver];
		if (FAILED(hr = D3D11CreateDevice(nullptr, m_DriverType, nullptr, createDeviceFlags,
			featureLevels, featureNum, D3D11_SDK_VERSION, &m_pd3dDevice, &m_feautreLevel, &m_pImmediateContext)))
		{
			return hr;
		}
		if (m_feautreLevel < D3D_FEATURE_LEVEL_11_0)
		{
			if (m_pd3dDevice) m_pd3dDevice->Release();
			if (m_pImmediateContext) m_pImmediateContext->Release();
			continue;
		}
		break;
	}
	return S_OK;
}
HRESULT Device::CreateGIFactory()
{
	if (m_pd3dDevice == NULL) return E_FAIL;
	HRESULT hr;// = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory) );
	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);

	pDXGIDevice->Release();
	pDXGIAdapter->Release();
	return hr;
}
HRESULT Device::CreateSwapChain(const UINT& iWidth, const UINT& iHeight)
{
	HRESULT hr;
	DXGI_MODE_DESC bdesc;
	ZeroMemory(&bdesc, sizeof(DXGI_MODE_DESC));
	bdesc.Height = iHeight;
	bdesc.Width = iWidth;
	bdesc.RefreshRate.Denominator = 1;
	bdesc.RefreshRate.Numerator = 60;
	bdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	m_SwapChainDesc.BufferDesc = bdesc;
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.OutputWindow = g_hWnd;
	m_SwapChainDesc.Windowed = TRUE;
	m_bFullScreenMode = !m_SwapChainDesc.Windowed;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.SampleDesc.Quality = 0;
	
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &m_SwapChainDesc, &m_pSwapChain)))
	{
		return hr;
	}
	return hr;
}
HRESULT Device::SetRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		return hr;
	}
	if (FAILED(hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView)))
	{
		return hr;
	}
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	pBackBuffer->Release();
	return hr;
}
void Device::SetViewPort()
{
	m_ViewPort.Height = static_cast<FLOAT>(m_SwapChainDesc.BufferDesc.Height);
	m_ViewPort.Width = static_cast<FLOAT>(m_SwapChainDesc.BufferDesc.Width);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
}
bool Device::CleanupDevice()
{
	m_pSwapChain->SetFullscreenState(FALSE, nullptr);
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pGIFactory) m_pGIFactory->Release();

	m_pGIFactory = nullptr;
	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pImmediateContext = nullptr;
	return true;
}
HRESULT Device::ResizeDevice(const UINT& iWidth, const UINT& iHeight)
{
	if (m_pd3dDevice == nullptr) return S_FALSE;
	HRESULT hr;

	m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	if (m_pRenderTargetView) m_pRenderTargetView->Release();

	// DirectWrite 장치 제거
	DeleteDeviceResources();

	m_SwapChainDesc.BufferDesc.Height = iHeight;
	m_SwapChainDesc.BufferDesc.Width = iWidth;

	m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, iWidth, iHeight,
		m_SwapChainDesc.BufferDesc.Format, 0);

	GetClientRect(g_hWnd, &g_rtClient);

	if (FAILED(hr = SetRenderTargetView()))
	{
		return hr;
	}
	SetViewPort();

	// DirectWrite 장치 재생성
	CreateDeviceResources();
	return S_OK;
}
void Device::DeleteDeviceResources() { return; }
HRESULT	Device::CreateDeviceResources() { return S_OK; }
ID3D11DeviceContext * Device::getImmediContext() const
{
	if (m_pImmediateContext)
		return m_pImmediateContext;
	return nullptr;
}
ID3D11RenderTargetView* Device::getRenderTargetView() const
{
	if (m_pRenderTargetView)
		return m_pRenderTargetView;
	return nullptr;
}
IDXGISwapChain*	Device::getSwapChain() const
{
	if (m_pSwapChain)
		return m_pSwapChain;
	return nullptr;
}
IDXGIFactory* Device::getGIFactory() const
{
	if (m_pGIFactory)
		return m_pGIFactory;
	return nullptr;
}
BOOL Device::getFullScreenMode() const
{
	return m_bFullScreenMode;
}
BOOL Device::setFullScreenMode(const BOOL& isFullScreen)
{
	m_bFullScreenMode = isFullScreen;
	return m_bFullScreenMode;
}