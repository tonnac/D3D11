#pragma once
#include "Object.h"

class FadeObject : public Object
{
public:
	FadeObject();
	~FadeObject();
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	bool				SetAlpha(const FLOAT&);
	bool				Decrease(const FLOAT&);
private:
	BLENDFUNCTION		m_bBlend;
	FLOAT				m_fAlpha;
	FLOAT				m_fDecrement;
	HDC					m_hMemDC;
	HBRUSH				m_bkBrush;
	HBITMAP				m_hOffBitmap;
};