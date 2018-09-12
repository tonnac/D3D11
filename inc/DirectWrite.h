#pragma once
#include "Define.h"
#include "D2D1.h"
#include "DWrite.h"

using ScaleTrans = std::function<void(const TCHAR*, const FLOAT&, const D2D1_POINT_2F&) > ;
using RotateTrans = std::function<void(const TCHAR*, const FLOAT&, const D2D1_POINT_2F&)>;

struct TextStruct
{
	TextStruct() : _World(D2D1::Matrix3x2F::Identity()), _pTextFormat(nullptr), _pTextLayout(nullptr), _pColorBrush(nullptr), _Text(std::tstring()), _Color(0.0f,0.0f,0.0f,0.0f)
	{}
	D2D1::Matrix3x2F		_World;
	std::tstring			_Text;
	IDWriteTextFormat*		_pTextFormat;
	IDWriteTextLayout*		_pTextLayout;
	ID2D1SolidColorBrush*	_pColorBrush;
	D2D1_RECT_F				_TextPos;
	D2D1::ColorF			_Color;
	DWRITE_FONT_FEATURE		_FontFeature;
	DWRITE_FONT_STYLE		_FontStyle;
	DWRITE_FONT_WEIGHT		_FontWeight;
	FLOAT					_FontSize;
	std::tstring			_FontFamily;
	inline bool Release()
	{
		RELEASE(_pTextLayout);
		BrushRelease();
		RELEASE(_pTextFormat);
		return true;
	}
	inline bool BrushRelease()
	{
		RELEASE(_pColorBrush);
		return true;
	}
};

using TextMap = std::map<const TCHAR*, TextStruct>;

class DirectWrite : public Singleton<DirectWrite>
{
	friend class Singleton<DirectWrite>;
private:
	DirectWrite();
public:
	bool					Begin();
	bool					End();
public:
	bool					Init(IDXGISwapChain** pSwapChain);
	bool					Render();
	bool					RenderText(const TCHAR* Key);
	bool					Release();
public:
	void					AddText(const TCHAR* Key, const std::tstring& Text, const D2D1_RECT_F& rt, const D2D1::ColorF& Color, const TCHAR* FontName,
									const FLOAT& fontSize, const TCHAR* localeName = L"en-us",
									const DWRITE_FONT_WEIGHT& FontWeight = DWRITE_FONT_WEIGHT_NORMAL, const DWRITE_FONT_STYLE& FontStyle = DWRITE_FONT_STYLE_NORMAL,
									const DWRITE_FONT_STRETCH& FontStretch = DWRITE_FONT_STRETCH_NORMAL);
	void					AddLayout(const TCHAR* Key, const D2D1_POINT_2F& Point, const DWRITE_FONT_FEATURE& fontFeature = { DWRITE_FONT_FEATURE_TAG_DEFAULT, 1 });
public:
	void					CreateFactory();
	void					CreateRenderTarget();
	void					DiscardFactory();
	void					DiscardRenderTarget();
public:
	void					ResizeDiscard();
	void					ResizeCreate();
public:
	void					Transform(const TCHAR* Key, const FLOAT& fAngle, const FLOAT& fScale, const D2D1_POINT_2F& CenterPos = { CASTING(FLOAT,g_rtClient.right / 2), CASTING(FLOAT,g_rtClient.bottom / 2) });
public:
	void					SetText(const TCHAR* Key, const std::tstring& Text);
	void					SetFont(const TCHAR* Key,const TCHAR *fontFamily);
	void					SetFontSize(const TCHAR* Key,const FLOAT& size);
	void					SetWeight(const TCHAR* Key,const DWRITE_FONT_WEIGHT& fontWeight = DWRITE_FONT_WEIGHT_NORMAL);
	void					SetStyle(const TCHAR* Key,const DWRITE_FONT_STYLE& fontStyle = DWRITE_FONT_STYLE_NORMAL);
	void					SetUnderline(const TCHAR* Key,const bool& underline);
	void					SetColor(const TCHAR* Key, const D2D1::ColorF& Color);
	void					SetAlignment(const TCHAR* Key, const DWRITE_TEXT_ALIGNMENT& Text, const DWRITE_PARAGRAPH_ALIGNMENT& Paragraph);
	void					SetTextPos(const TCHAR* Key, const D2D1_RECT_F& rt);
	void					SetTypography(const TCHAR* Key, const DWRITE_FONT_FEATURE& fontFeature);
	void					SetRotate(const TCHAR* Key, const FLOAT& fAngle, const D2D1_POINT_2F& CenterPos = { CASTING(FLOAT,g_rtClient.right / 2), CASTING(FLOAT,g_rtClient.bottom / 2) });
	void					SetScale(const TCHAR* Key, const FLOAT& fScale, const D2D1_POINT_2F& CenterPos = { CASTING(FLOAT,g_rtClient.right / 2), CASTING(FLOAT,g_rtClient.bottom / 2) });
private:
	ScaleTrans				ScaleTransFunc;
	RotateTrans				RotateTransFunc;
	ID2D1Factory *			m_pD2DFactory;
	IDWriteFactory*			m_pWriteFactory;
	ID2D1RenderTarget*		m_pRenderTarget;
	FLOAT					m_fDpiX;
	FLOAT					m_fDpiY;
	TextMap					m_TextMap;
private:
	IDXGISwapChain**		m_pSwapChain;
};

#define S_Write DirectWrite::getInst()