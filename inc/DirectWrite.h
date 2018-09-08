#pragma once
#include "Define.h"
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")


class DirectWrite
{
public:
	DirectWrite();
public:
	bool					Set(IDXGISurface* pSurface);
	bool					Init();
	bool					Release();
public:
	bool					Begin();
	bool					End();
	HRESULT					DrawText(const D2D1_RECT_F& rt, const TCHAR* pText, D2D1::ColorF Color);
public:
	HRESULT					CreateDeviceIndependentResources();
	void					DiscardDeviceIndependentResources();
	HRESULT					CreateDeviceResources(IDXGISurface* pSurface);
	void					DiscardDeviceResources();
public:
	void					OnResize(IDXGISurface* pSurface);
private:
	ID2D1Factory *			m_pD2DFactory;
	ID2D1RenderTarget*		m_pRenderTarget;
	IDWriteFactory*			m_pDWriteFactory;
	IDWriteTextFormat*		m_pTextFormat;
	IDWriteTextLayout*		m_pTextLayout;
	ID2D1SolidColorBrush*	m_pColorBrush;
private:
	FLOAT					m_fDpiX;
	FLOAT					m_fDpiY;
};