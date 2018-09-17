#pragma once
#include "Std.h"
#include "Object.h"

class Rendering
{
public:
	Rendering(Object*);
	virtual ~Rendering() {};
public:
	virtual bool		Init		();
	virtual bool		Frame		();
	virtual bool		Render		();
	virtual bool		Release		();
	virtual	void		setInverse	(const INVERSE&);
	virtual	void		setFade		(const FLOAT&, const FLOAT&);
	virtual void		setFix		(const FLOAT&, const FLOAT&);
public:
	void * operator new(size_t sz, const char* FileName, int iLine);
	void operator delete(void * p);
protected:
	FloatPoint*			m_CenterPos;
	FloatPoint*			m_DrawPos;
	RECT*				m_rtCollision;
	RECT*				m_rtDraw;
	HDC					m_hColorDC;
	HDC					m_hMaskDC;
};
class InversionRendering : public Rendering
{
public:
	InversionRendering(Object*, const FLOAT&, const INVERSE&);
public:
	bool				Frame() override;
	bool				Render() override;
	void				setInverse(const INVERSE&) override;
private:
	INVERSE				m_sInverse;
	FLOAT				m_fZoom;
};
class RotateRendering : public Rendering
{
public:
	RotateRendering(Object*, const FLOAT&);
public:
	void				getRotateBitmap(HBITMAP, HDC);
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	HDC					m_hRotationDC;
	HDC					m_hRColorDC;
	HDC					m_hRMaskDC;
	HBITMAP				m_hbColorBitmap;
	HBITMAP				m_hbMaskBitmap;

	FLOAT				m_fAngle;
	FLOAT				m_fMaxLength;
};
class AlphaRendering : public Rendering
{
public:
	AlphaRendering(Object*, const INVERSE& = INVERSE::DEFAULT, const FLOAT& = 1.0f);
public:
	bool				Init		() override;
	bool				Frame		() override;
	bool				Render		() override;
	void				setFade		(const FLOAT&, const FLOAT& = 30.0f);
public:
	bool AlphaBlend(HDC dcDest, int x, int y, int cx, int cy,
		HDC dcSrc,
		HDC dcMaskSrc, int sx, int sy, int scx, int scy, int alpha, 
		DWORD opMode = SRCCOPY, COLORREF rgbMask = RGB(0, 0, 0));
private:
	INVERSE				m_sInverse;
	BLENDFUNCTION		m_bBlend;
	FLOAT				m_fZoom;
	FLOAT				m_fAlpha;
	FLOAT				m_fFadeSpeed;
};