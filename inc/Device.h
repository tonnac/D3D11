#pragma once
#include "Define.h"

class Device
{
public:
	Device();
public:
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain(const UINT& iWidth, const UINT& iHeight);
	HRESULT SetRenderTargetView();
	void SetViewPort();
	bool CleanupDevice();
public:
	HRESULT ResizeDevice(const UINT& iWidth, const UINT& iHeight);
	//Direct Write ¼Ò¸ê & »ý¼º
	virtual void			DeleteDeviceResources();
	virtual HRESULT			CreateDeviceResources();
public:
	ID3D11DeviceContext *	getImmediContext() const;
	ID3D11RenderTargetView* getRenderTargetView() const;
	IDXGISwapChain*			getSwapChain() const;
	IDXGIFactory*			getGIFactory() const;
	BOOL					getFullScreenMode() const;
public:
	BOOL					setFullScreenMode(const BOOL& isFullScreen);
private:
	ID3D11Device *			m_pd3dDevice;
	ID3D11DeviceContext*	m_pImmediateContext;

	IDXGIFactory*			m_pGIFactory;

	IDXGISwapChain*			m_pSwapChain;

	ID3D11RenderTargetView*	m_pRenderTargetView;
	D3D_FEATURE_LEVEL		m_feautreLevel;
	D3D_DRIVER_TYPE			m_DriverType;
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;
	D3D11_VIEWPORT			m_ViewPort;

	BOOL					m_bFullScreenMode;
};