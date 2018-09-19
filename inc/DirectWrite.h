#pragma once
#include "d3dUtil.h"

class DirectWrite : public Singleton<DirectWrite>
{
	friend class Singleton<DirectWrite>;
private:
	DirectWrite() = default;
public:
	bool					Set(IDXGISwapChain* pSwapChain);
	bool					Init();
	bool					Begin();
	bool					End();
	bool					Release();
public:
	void					DrawText(const D2D1_RECT_F& rt, const std::tstring& Text, D2D1::ColorF Color);
public:
	void					OnResize(IDXGISwapChain * pSwapChain);
public:
	void					CreateINDP_Device();
	void					CreateDP_Device(IDXGISwapChain* pSwapChain);
	void					DiscardINDP_Device();
	void					DiscardDP_Device();
private:
	ID2D1RenderTarget *		m_pRenderTarget = nullptr;
	ID2D1Factory*			m_pD2DFactory = nullptr;
	ID2D1SolidColorBrush*	m_pColorBrush = nullptr;
	IDWriteFactory*			m_pWriteFactory = nullptr;
	IDWriteTextFormat*		m_pTextFormat = nullptr;
	FLOAT					m_fdpiX = 0.0f;
	FLOAT					m_fdpiY = 0.0f;
};

#define S_Write DirectWrite::getInst()