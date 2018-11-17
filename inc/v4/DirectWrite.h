#pragma once
#include "d3dUtil.h"

class DirectWrite : public Singleton<DirectWrite>
{
	friend class Singleton<DirectWrite>;
private:
	DirectWrite() = default;

public:
	bool					Initialize();
	bool					Begin();
	bool					End();

	void					DrawText(const D2D1_RECT_F& rt, const std::tstring& Text, DirectX::FXMVECTOR Color);

	void					OnResize(IDXGISwapChain* pSwapChain);
	void					Reset();

private:
	Microsoft::WRL::ComPtr<ID2D1RenderTarget>		m_pRenderTarget = nullptr;
	Microsoft::WRL::ComPtr<ID2D1Factory>			m_pD2DFactory = nullptr;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_pColorBrush = nullptr;
	Microsoft::WRL::ComPtr<IDWriteFactory>			m_pWriteFactory = nullptr;
	Microsoft::WRL::ComPtr<IDWriteTextFormat>		m_pTextFormat = nullptr;

	FLOAT					m_fdpiX = 0.0f;
	FLOAT					m_fdpiY = 0.0f;
};

#define S_Write DirectWrite::getInst()