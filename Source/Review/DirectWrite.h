#pragma once
#include "Define.h"
#include "D2D1.h"
#include "DWrite.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

struct TextStruct
{
	TextStruct() : _pTextFormat(nullptr), _pTextLayout(nullptr), _pColorBrush(nullptr), _Text(std::tstring())
	{}
	std::tstring			_Text;
	IDWriteTextFormat*		_pTextFormat;
	IDWriteTextLayout*		_pTextLayout;
	ID2D1SolidColorBrush*	_pColorBrush;
	D2D1_RECT_F				_TextPos;
	inline bool Release()
	{
		RELEASE(_pTextLayout);
		RELEASE(_pColorBrush);
		RELEASE(_pTextFormat);
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
	bool					Release();
public:
	void					AddText(const TCHAR* Key, const std::tstring& Text, const D2D1_RECT_F& rt, const D2D1::ColorF& Color, const TCHAR* FontName,
									const FLOAT& fontSize, const TCHAR* localeName = L"en-us",
									const DWRITE_FONT_WEIGHT& FontWeight = DWRITE_FONT_WEIGHT_NORMAL, const DWRITE_FONT_STYLE& FontStyle = DWRITE_FONT_STYLE_NORMAL,
									const DWRITE_FONT_STRETCH& FontStretch = DWRITE_FONT_STRETCH_NORMAL);
	void					AddLayout(const TCHAR* Key, const D2D1_POINT_2F& Point);
	void					ChangeText(const TCHAR* Key, const std::tstring& Text);
public:
	HRESULT					CreateFactory();
	HRESULT					CreateRenderTarget();
	void					DiscardFactory();
	void					DiscardRenderTarget();
public:
	void					OnResize();
public:
	HRESULT					SetFont(const TCHAR* Key,const TCHAR *fontFamily);
	HRESULT					SetFontSize(const TCHAR* Key,const float& size);
	HRESULT					SetWeight(const TCHAR* Key,const bool& bold);
	HRESULT					SetStyle(const TCHAR* Key,const bool& italic);
	HRESULT					SetUnderline(const TCHAR* Key,const bool& underline);
	void					SetColor(const TCHAR* Key, const D2D1::ColorF& Color);
	void					SetAlignment(const TCHAR* Key, const DWRITE_TEXT_ALIGNMENT& Text, const DWRITE_PARAGRAPH_ALIGNMENT& Paragraph);
private:
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