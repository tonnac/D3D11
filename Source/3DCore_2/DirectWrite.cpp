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
	ScaleTransFunc = [this](const TCHAR* Key, const FLOAT& fScale, const D2D1_POINT_2F& Pos = { CASTING(FLOAT,g_rtClient.right), CASTING(FLOAT,g_rtClient.bottom) }) 
	{Transform(Key, 0.0f, fScale, Pos);};
	RotateTransFunc = [this](const TCHAR* Key, const FLOAT& fAngle, const D2D1_POINT_2F& Pos = { CASTING(FLOAT,g_rtClient.right), CASTING(FLOAT,g_rtClient.bottom) }) 
	{Transform(Key, fAngle, 1.0f, Pos);};
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
			m_pRenderTarget->DrawTextLayout({ iter.second._TextPos.left, iter.second._TextPos.top }, iter.second._pTextLayout, iter.second._pColorBrush);
		}
	}
	return true;
}
bool DirectWrite::RenderText(const TCHAR* Key)
{
	if (m_TextMap[Key]._pTextLayout == nullptr)
	{
		m_pRenderTarget->SetTransform(m_TextMap[Key]._World);
		m_pRenderTarget->DrawText(m_TextMap[Key]._Text.c_str(), CASTING(UINT, m_TextMap[Key]._Text.length()), m_TextMap[Key]._pTextFormat, m_TextMap[Key]._TextPos, m_TextMap[Key]._pColorBrush);
	}
	else
	{
		m_pRenderTarget->SetTransform(m_TextMap[Key]._World);
		m_pRenderTarget->DrawTextLayout({ m_TextMap[Key]._TextPos.left, m_TextMap[Key]._TextPos.top }, m_TextMap[Key]._pTextLayout, m_TextMap[Key]._pColorBrush);
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
HRESULT DirectWrite::AddText(const TCHAR* Key, const std::tstring& Text, const D2D1_RECT_F& rt, const D2D1::ColorF& Color, const TCHAR* FontName,
	const FLOAT& fontSize, const TCHAR* localeName,	const DWRITE_FONT_WEIGHT& FontWeight, const DWRITE_FONT_STYLE& FontStyle, const DWRITE_FONT_STRETCH& FontStretch)
{
	HRESULT hr;
	if (m_TextMap.find(Key) != m_TextMap.end()) return E_FAIL;
	TextStruct AddText;
	AddText._Text = Text;
	DXFAIL(m_pWriteFactory->CreateTextFormat(FontName, nullptr, FontWeight, FontStyle, FontStretch, fontSize, localeName, &AddText._pTextFormat));
	DXFAIL(m_pRenderTarget->CreateSolidColorBrush(Color, &AddText._pColorBrush));
	AddText._Color = Color;
	AddText._TextPos = rt;
	m_TextMap.insert(std::make_pair(Key, AddText));
	return hr;
}
HRESULT DirectWrite::AddLayout(const TCHAR* Key, const D2D1_POINT_2F& Point, const DWRITE_FONT_FEATURE& fontFeature)
{
	HRESULT hr;
	if (m_TextMap.find(Key) == m_TextMap.end()) return E_FAIL;
	RELEASE(m_TextMap[Key]._pTextLayout);
	DXFAIL(m_pWriteFactory->CreateTextLayout(m_TextMap[Key]._Text.c_str(), CASTING(UINT32,m_TextMap[Key]._Text.length()), m_TextMap[Key]._pTextFormat, Point.x, Point.y, &m_TextMap[Key]._pTextLayout));
	IDWriteTypography* pTypography = nullptr;
	DXFAIL(m_pWriteFactory->CreateTypography(&pTypography));
	DXFAIL(pTypography->AddFontFeature(fontFeature));
	m_TextMap[Key]._FontFeature = fontFeature;
	DXFAIL(m_TextMap[Key]._pTextLayout->SetTypography(pTypography, { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) }));
	RELEASE(pTypography);
	return hr;
}
void DirectWrite::SetText(const TCHAR* Key, const std::tstring& Text)
{
	if (m_TextMap.find(Key) == m_TextMap.end()) return;
	m_TextMap[Key]._Text = Text;
	if (m_TextMap[Key]._pTextLayout != nullptr)
	{
		HRESULT hr;
		FLOAT x, y;
		x = m_TextMap[Key]._pTextLayout->GetMaxWidth();
		y = m_TextMap[Key]._pTextLayout->GetMaxHeight();

		RELEASE(m_TextMap[Key]._pTextLayout);
		hr = m_pWriteFactory->CreateTextLayout(Text.c_str(), CASTING(UINT32, Text.length()), m_TextMap[Key]._pTextFormat, x, y, &m_TextMap[Key]._pTextLayout);
		if (FAILED(hr)) return;
		IDWriteTypography * pTypography = nullptr;
		hr = m_pWriteFactory->CreateTypography(&pTypography);
		if (FAILED(hr)) return;

		hr = pTypography->AddFontFeature(m_TextMap[Key]._FontFeature);
		if (FAILED(hr)) return;

		DWRITE_TEXT_RANGE textRange = { 0, CASTING(UINT32, Text.length()) };
		hr = m_TextMap[Key]._pTextLayout->SetTypography(pTypography, textRange);
		if (FAILED(hr)) return;

		hr = SetFont(Key, m_TextMap[Key]._FontFamily.c_str());
		if (FAILED(hr)) return;

		hr = SetFontSize(Key, m_TextMap[Key]._FontSize);
		if (FAILED(hr)) return;

		hr = SetStyle(Key, m_TextMap[Key]._FontStyle);
		if (FAILED(hr)) return;

		hr = SetWeight(Key, m_TextMap[Key]._FontWeight);
		if (FAILED(hr)) return;

		RELEASE(pTypography);
	}
}
HRESULT	DirectWrite::SetFont(const TCHAR* Key, const TCHAR *fontFamily)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };

	m_TextMap[Key]._FontFamily = fontFamily;
	DXFAIL(m_TextMap[Key]._pTextLayout->SetFontFamilyName(fontFamily, TextRange));
	return hr;
}
HRESULT	DirectWrite::SetFontSize(const TCHAR* Key, const FLOAT& size)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };
	m_TextMap[Key]._FontSize = size;
	DXFAIL(m_TextMap[Key]._pTextLayout->SetFontSize(size, TextRange));
	return hr;
}
HRESULT	DirectWrite::SetWeight(const TCHAR* Key, const DWRITE_FONT_WEIGHT& fontWeight)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };
	m_TextMap[Key]._FontWeight = fontWeight;
	DXFAIL(m_TextMap[Key]._pTextLayout->SetFontWeight(fontWeight, TextRange));
	return hr;
}
HRESULT	DirectWrite::SetStyle(const TCHAR* Key, const DWRITE_FONT_STYLE& fontStyle)
{
	HRESULT hr;
	DWRITE_TEXT_RANGE TextRange = { 0, CASTING(UINT32,m_TextMap[Key]._Text.length()) };
	m_TextMap[Key]._FontStyle = fontStyle;
	DXFAIL(m_TextMap[Key]._pTextLayout->SetFontStyle(fontStyle, TextRange));
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
	m_TextMap[Key]._Color = Color;
}
void DirectWrite::SetAlignment(const TCHAR* Key, const DWRITE_TEXT_ALIGNMENT& Text, const DWRITE_PARAGRAPH_ALIGNMENT& Paragraph)
{
	if (m_TextMap.find(Key) == m_TextMap.end()) return;
	m_TextMap[Key]._pTextFormat->SetTextAlignment(Text);
	m_TextMap[Key]._pTextFormat->SetParagraphAlignment(Paragraph);
}
void DirectWrite::SetTextPos(const TCHAR* Key, const D2D1_RECT_F& rt)
{
	if (m_TextMap.find(Key) == m_TextMap.end()) return;
	m_TextMap[Key]._TextPos = rt;
}
void DirectWrite::SetTypography(const TCHAR* Key, const DWRITE_FONT_FEATURE& fontFeature)
{
	HRESULT hr;
	FLOAT x, y;
	x = m_TextMap[Key]._pTextLayout->GetMaxWidth();
	y = m_TextMap[Key]._pTextLayout->GetMaxHeight();
	RELEASE(m_TextMap[Key]._pTextLayout);
	hr = m_pWriteFactory->CreateTextLayout(m_TextMap[Key]._Text.c_str(), CASTING(UINT32, m_TextMap[Key]._Text.length()), m_TextMap[Key]._pTextFormat, x, y, &m_TextMap[Key]._pTextLayout);
	if (FAILED(hr)) return;
	IDWriteTypography * pTypography = nullptr;
	hr = m_pWriteFactory->CreateTypography(&pTypography);
	if (FAILED(hr)) return;
	m_TextMap[Key]._FontFeature = fontFeature;
	hr = pTypography->AddFontFeature(m_TextMap[Key]._FontFeature);
	if (FAILED(hr)) return;
	DWRITE_TEXT_RANGE textRange = { 0, CASTING(UINT32, m_TextMap[Key]._Text.length()) };
	hr = m_TextMap[Key]._pTextLayout->SetTypography(pTypography, textRange);
	if (FAILED(hr)) return;
	RELEASE(pTypography);
}
void DirectWrite::SetRotate(const TCHAR* Key, const FLOAT& fAngle, const D2D1_POINT_2F& CenterPos)
{
	RotateTransFunc(Key, fAngle, CenterPos);
}
void DirectWrite::SetScale(const TCHAR* Key, const FLOAT& fScale, const D2D1_POINT_2F& CenterPos)
{
	ScaleTransFunc(Key, fScale, CenterPos);
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
void DirectWrite::ResizeDiscard()
{
	DiscardRenderTarget();
	for (auto& iter : m_TextMap)
	{
		iter.second.BrushRelease();
	}
}
void DirectWrite::ResizeCreate()
{
	CreateRenderTarget();
	for (auto& iter : m_TextMap)
	{
		D2D1::ColorF color = iter.second._Color;
		m_pRenderTarget->CreateSolidColorBrush(color, &iter.second._pColorBrush);
		iter.second._pColorBrush->SetColor(color);
	}
}
void DirectWrite::Transform(const TCHAR* Key, const FLOAT& fAngle, const FLOAT& fScale, const D2D1_POINT_2F& CenterPos)
{
	D2D1::Matrix3x2F Rot = D2D1::Matrix3x2F::Identity().Rotation(fAngle, CenterPos);
	D2D1::Matrix3x2F Scale = D2D1::Matrix3x2F::Identity().Scale(fScale, fScale, CenterPos);
	m_TextMap[Key]._World = Rot * Scale;
}