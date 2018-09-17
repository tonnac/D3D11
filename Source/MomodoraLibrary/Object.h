#pragma once
#include "BitmapMgr.h"
#include "Input.h"

class Rendering;

class Object
{
public:
	Object();
	virtual ~Object() {};
public:
	virtual bool		Init			();
	virtual bool		Frame			();
	virtual bool		Render			();
	virtual bool		Release			();
public:
	RECT*				getrtDraw		();
	RECT*				getCollisionRt	();
	FloatPoint*			getCenterPos	();
	FloatPoint*			getDrawPos		();
	HDC					getColorDC		() const;
	HDC					getMaskDC		() const;
	FLOAT				getSpeed		() const;
	bool				getDebugmode	() const;
	bool				getLanding		() const;
public:
	void				setRendering	();
	void				setRendering	(const FLOAT&, const INVERSE&);
	void				setRendering	(const FLOAT&);
	void				setRendering	(const INVERSE&);
	void				setFadeRender	(const FLOAT&, const FLOAT&);
	void				setFadeRender	(const FLOAT&, const INVERSE&);
	void				setDebugmode	(const bool&);
	void				setSpeed		(const FLOAT&);
	void				setDraw_x		(const FLOAT&);
	void				setCenterPos_x	(const FLOAT&);
	void				setCenterPos_y	(const FLOAT&);
	void				setLanding		(const bool&);
public:
	void				DebugMode();
public:
	virtual bool		LoadFile		(T_STR, T_STR, T_STR = std::basic_string<TCHAR>());
	virtual void		Set				(const FLOAT&, const FLOAT&,
										const DWORD& = 0, const DWORD& = 0, const DWORD& = 0, const DWORD& = 0);
	void				Set				(const FloatPoint&, const RECT&);
public:
	virtual bool		MoveScrollBk	(const LONG&);
	virtual bool		MoveScrollObj	(const LONG&);
public:
	virtual	bool		Collision		(Object *);
public:
	void * operator new(size_t sz, const char* FileName, int iLine);
	void operator delete(void * p);
protected:
	FLOAT				m_fScroll;
	FloatPoint			m_CenterPos;
	FloatPoint			m_DrawPos;
	FLOAT				m_fSpeed;
	RECT				m_rtDraw;
	RECT				m_rtCollision;
	Bitmap *			m_ColorBitmap;
	Bitmap *			m_MaskBitmap;
	bool				isDebugMode;
	bool				isLanding;
	Rendering*			m_pRendering;
};