#include "DirectWrite.h"

DirectWrite::DirectWrite() : m_pD2DFactory(nullptr), m_pWriteFactory(nullptr), m_pRenderTarget(nullptr)
{}
bool DirectWrite::Begin()
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->BeginDraw();
	}
	return true;
}
bool DirectWrite::End()
{
	if (m_pRenderTarget && FAILED(m_pRenderTarget->EndDraw()))
	{
		return false;
	}
	return true;
}
bool DirectWrite::Init(IDXGISwapChain** pSwapChain)
{
	this->m_pSwapChain = pSwapChain;
	CreateFactory();
	CreateRenderTarget();
	return true;
}
bool DirectWrite::Render()
{
	for (auto& iter : m_TextMap)
	{
		if (iter.second._pTextLayout == nullptr)
		{
			m_pRenderTarget->DrawText(iter.second._Text.c_str(), CASTING(UINT, iter.second._Text.length()), iter.second._pTextFormat, iter.second._TextPos, iter.second._pColorBrush);
		}
		else
		{
			m_pRenderTarget->DrawTextLayout({ iter.second._pTextLayout->GetMaxWidth(), iter.second._pTextLayout->GetMaxHeight() }, iter.second._pTextLayout, iter.second._pColorBrush);
		}
	}
	return true;
}
bool DirectWrite::Release()
{
	for (auto& iter : m_TextMap)
	{
		iter.second.Release();
	}
	m_TextMap.clear();
	DiscardRenderTarget();
	DiscardFactory();
	return true;
}
void DirectWrite::AddText(const TCHAR* Key, const std::tstring& Text, const D2D1_RECT_F& rt, const D2D1::ColorF& Color, const TCHAR* FontName,
	const FLOAT& fontSize, const TCHAR* localeName,	const DWRITE_FONT_WEIGHT& FontWeight, const DWRITE_FONT_STYLE& FontStyle, const DWRITE_FONT_STRETCH& FontStretch)
{
	HRESULT hr;
	if (m_TextMap.find(Key) != m_TextMap.end()) return;
	TextStruct AddText;
	AddText._Text = Text;
	hr = m_pWriteFactory->CreateTextFormat(FontName, nullptr, FontWeight, FontStyle, FontStretch, fontSize, localeName, &AddText._pTextFormat);
	if (FAILED(hr)) return;
	hr = m_pRenderTarget->CreateSolidColorBrush(Color, &AddText._pColorBrush);
	if (FAILED(hr)) return;
	AddText._TextPos = rt;
	m_TextMap.insert(std::make_pair(Key, AddText));
}
void DirectWrite::AddLayout(const TCHAR* Key, const D2D1_POINT_2F& Point)
{
	HRESULT hr;
	hr = m_pWriteFactory->CreateTextLayout(m_TextMap[Key]._Text.c_str(), m_TextMap[Key]._Text.length(), m_TextMap[Key]._pTextFormat, Point.x, Point.y, &m_TextMap[Key]._pTextLayout);
	if (FAILED(hr)) return;
	IDWriteTypography* pTypography = nullptr;
	hr = m_pWriteFactory->CreateTypography(&pTypography);
	if (FAILED(hr)) return;
	hr = pTypography->AddFontFeature({ DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7, 1 });
	if (FAILED(hr)) return;
	hr = m_TextMap[Key]._pTextLayout->SetTypography(pTypography, { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) });
	if (FAILED(hr)) return;
	RELEASE(pTypography);
}
void DirectWrite::ChangeText(const TCHAR* Key, const std::tstring& Text)
{
	if (m_TextMap.find(Key) == m_TextMap.end()) return;
	m_TextMap[Key]._Text = Text;
}
HRESULT	DirectWrite::SetFont(const TCHAR* Key, const TCHAR *fontFamily)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };

	DXFAIL(m_TextMap[Key]._pTextLayout->SetFontFamilyName(fontFamily, TextRange));
	return hr;
}
HRESULT	DirectWrite::SetFontSize(const TCHAR* Key, const float& size)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };

	DXFAIL(m_TextMap[Key]._pTextLayout->SetFontSize(size, TextRange));
	return hr;
}
HRESULT	DirectWrite::SetWeight(const TCHAR* Key, const bool& bold)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };
	if (bold == true)
	{
		DXFAIL(m_TextMap[Key]._pTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, TextRange));
	}
	else
	{
		DXFAIL(m_TextMap[Key]._pTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_NORMAL, TextRange));
	}
	return hr;
}
HRESULT	DirectWrite::SetStyle(const TCHAR* Key, const bool& italic)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };
	if (italic == true)
	{
		DXFAIL(m_TextMap[Key]._pTextLayout->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, TextRange));
	}
	else
	{
		DXFAIL(m_TextMap[Key]._pTextLayout->SetFontStyle(DWRITE_FONT_STYLE_NORMAL, TextRange));
	}
	return hr;
}
HRESULT	DirectWrite::SetUnderline(const TCHAR* Key, const bool& underline)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };
	if (underline == true)
	{
		DXFAIL(m_TextMap[Key]._pTextLayout->SetUnderline(underline, TextRange));
	}
	else
	{
		DXFAIL(m_TextMap[Key]._pTextLayout->SetUnderline(underline, TextRange));
	}
	return hr;
}
void DirectWrite::SetColor(const TCHAR* Key, const D2D1::ColorF& Color)
{
	if (m_TextMap.find(Key) == m_TextMap.end()) return;
	m_TextMap[Key]._pColorBrush->SetColor(Color);
}
void DirectWrite::SetAlignment(const TCHAR* Key, const DWRITE_TEXT_ALIGNMENT& Text, const DWRITE_PARAGRAPH_ALIGNMENT& Paragraph)
{
	if (m_TextMap.find(Key) == m_TextMap.end()) return;
	m_TextMap[Key]._pTextFormat->SetTextAlignment(Text);
	m_TextMap[Key]._pTextFormat->SetParagraphAlignment(Paragraph);
}
HRESULT DirectWrite::CreateFactory()
{
	HRESULT hr;
	DXFAIL(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory));
	m_pD2DFactory->GetDesktopDpi(&m_fDpiX, &m_fDpiY);
	DXFAIL(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory)));
	return hr;
}
HRESULT DirectWrite::CreateRenderTarget()
{
	HRESULT hr;
	IDXGISurface* pSurface = nullptr;
	(*m_pSwapChain)->GetBuffer(0, __uuidof(IDXGISurface), VOIDPTR(pSurface));
	D2D1_RENDER_TARGET_PROPERTIES Property;
	Property.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	Property.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Property.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	Property.dpiX = m_fDpiX;
	Property.dpiY = m_fDpiY;
	Property.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	Property.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	DXFAIL(m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface, &Property, &m_pRenderTarget));
	pSurface->Release();
	return hr;
}
void DirectWrite::DiscardFactory()
{
	RELEASE(m_pD2DFactory);
	RELEASE(m_pWriteFactory);
}
void DirectWrite::DiscardRenderTarget()
{
	RELEASE(m_pRenderTarget);
}
void DirectWrite::OnResize()
{
	return;
}