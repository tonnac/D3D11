#pragma once
#include "d3dUtil.h"

class Device
{
public:
	Device() = default;
public:
	void						InitDevice();
	void						CleanupDevice();
public:
	virtual void				ResizeDevice(const LONG& Width, const LONG& Height);
private:
	void						CreateDevice();
	void						CreateFactory();
	void						CreateSwapChain();
	void						SetRenderTargetView();
	void						SetViewPort();
protected:
	ID3D11Device *				m_pDevice = nullptr;

	IDXGIFactory *				m_pDXGIFactory = nullptr;

	IDXGISwapChain*				m_pSwapChain = nullptr;
	ID3D11DeviceContext *		m_pContext = nullptr;

	ID3D11RenderTargetView *	m_pRenderTargetView = nullptr;
	D3D11_VIEWPORT				m_ViewPort;

	D3D_FEATURE_LEVEL			m_FeatureLevel;
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;
};