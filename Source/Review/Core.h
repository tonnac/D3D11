#pragma once
#include "WClass.h"
#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"

class Core : public WClass
{
public:
	Core();
public:
	bool			GameInit() override;
	bool			GameRun() override;
	bool			GameRelease() override;
public:
	virtual	bool	Init();
	virtual	bool	Frame();
	virtual	bool	Render();
	virtual	bool	Release();
public:
	void			AddText(const TCHAR* Key, const std::tstring& Text, const D2D1_RECT_F& rt, const D2D1::ColorF& Color, const TCHAR* FontName,
							const FLOAT& fontSize, const TCHAR* localeName = L"en-us",
							const DWRITE_FONT_WEIGHT& FontWeight = DWRITE_FONT_WEIGHT_NORMAL, const DWRITE_FONT_STYLE& FontStyle = DWRITE_FONT_STYLE_NORMAL,
							const DWRITE_FONT_STRETCH& FontStretch = DWRITE_FONT_STRETCH_NORMAL);
private:
	bool			GameFrame();
	bool			GameRender();
	bool			PreInit();
	bool			PreRender();
	bool			PostRender();
private:
	virtual void	ResizeDiscard() override;
	virtual void	ResizeCreate() override;
private:
	Timer			m_Timer;
};