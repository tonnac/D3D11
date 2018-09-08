#pragma once
#include "Define.h"

class Device
{
public:
	Device();
public:
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain(UINT iWidth, UINT iHeight);
	HRESULT SetRenderTargetView();
	void SetViewPort();
	bool CleanupDevice();
public:
	ID3D11DeviceContext *	getImmediContext();
	ID3D11RenderTargetView* getRenderTargetView();
	IDXGISwapChain*			getSwapChain();
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
};