#pragma once
#include "Std.h"

class Bitmap
{
public:
	bool				Init();
	bool				Frame();
	bool				Render();
	bool				Release();
	bool				LoadFile(T_STR);
	HBITMAP				getHBITMAP();
	HDC					getMemDC();
	BITMAP				getBitmapInfo();
public:
	void * operator new(size_t sz, const char* FileName, int iLine);
	void operator delete(void * p);
private:
	BITMAP				m_BitmapInfo;
	HBITMAP				m_hBitmap;
	HDC					m_hMemDC;
};