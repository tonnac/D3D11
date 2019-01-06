#pragma once

#include "d3dUtil.h"
#include "DxRT.h"

class Device
{
protected:
	Device();

protected:
	virtual bool InitDirect3D();

	virtual void OnResize();

	void CreateSwapChain();
	void SetRenderTargetView();

	void LogAdapters();
	void LogAdapterOutput(IDXGIAdapter* pAdapter);
	void LogOutputDisplayModes(IDXGIOutput* pOutput, DXGI_FORMAT format);

protected:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pImmediateContext;

	Microsoft::WRL::ComPtr<IDXGIFactory> m_pdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapchain;

	DxRT m_DxRT;

	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
	D3D_FEATURE_LEVEL m_Featurelevel;
};