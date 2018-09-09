#pragma once
#include "Define.h"

class Device
{
public:
	Device();
public:
	HRESULT					CreateDevice();
	HRESULT					CreateFactory();
	HRESULT					CreateSwapChain();
	HRESULT					SetRenderTargetView();
	void					SetViewPort();
	bool					CleanupDevice();
public:
	HRESULT					ResizeDevice(const UINT& iWidth, const UINT& iHeight);
public:
	ID3D11Device *			getDevice();
	ID3D11DeviceContext*	getContext();
	IDXGISwapChain*			getSwapChain();
	IDXGISwapChain**		getSwapChainPtr();
	ID3D11RenderTargetView*	getRenderTargetView();
	IDXGIFactory*			getDXGIFactory();
	DXGI_SWAP_CHAIN_DESC	getSwapChainDesc();
private:
	//DirectWrite 장치 재생성
	virtual void			ResizeDiscard();
	virtual void			ResizeCreate();
private:
	ID3D11Device *			m_pd11Device;

	IDXGIFactory*			m_pDXGIFactory;

	IDXGISwapChain*			m_pSwapChain;
	ID3D11DeviceContext*	m_pImmediateContext;

	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;

	ID3D11RenderTargetView* m_pRenderTargetView;
	D3D11_VIEWPORT			m_ViewPort;

	D3D_FEATURE_LEVEL		m_FeatureLevel;
};