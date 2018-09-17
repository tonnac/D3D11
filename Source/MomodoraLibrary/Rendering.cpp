#include "Rendering.h"

Rendering::Rendering(Object* ob) : m_DrawPos(ob->getDrawPos()), m_rtDraw(ob->getrtDraw()),
									m_hColorDC(ob->getColorDC()), m_hMaskDC(ob->getMaskDC()),
								m_CenterPos(ob->getCenterPos()), m_rtCollision(ob->getCollisionRt())
{}
bool Rendering::Init()
{
	return true;
}
bool Rendering::Frame()
{
	m_CenterPos->x = m_DrawPos->x + (m_rtDraw->right / 2);
	m_CenterPos->y = m_DrawPos->y + (m_rtDraw->bottom / 2);

	m_rtCollision->left = static_cast<LONG>(m_DrawPos->x);
	m_rtCollision->top = static_cast<LONG>(m_DrawPos->y);
	m_rtCollision->right = static_cast<LONG>(m_DrawPos->x + m_rtDraw->right);
	m_rtCollision->bottom = static_cast<LONG>(m_DrawPos->y + m_rtDraw->bottom);
	return true;
}
bool Rendering::Render()
{
	if (m_hMaskDC == nullptr)
	{
		BitBlt(g_hOffScreenDC,
			static_cast<int>(m_DrawPos->x),
			static_cast<int>(m_DrawPos->y),
			m_rtDraw->right,
			m_rtDraw->bottom,
			m_hColorDC,
			m_rtDraw->left,
			m_rtDraw->top,
			SRCCOPY);
		return true;
	}
	BitBlt(g_hOffScreenDC,
		static_cast<int>(m_DrawPos->x),
		static_cast<int>(m_DrawPos->y),
		m_rtDraw->right,
		m_rtDraw->bottom,
		m_hMaskDC,
		m_rtDraw->left,
		m_rtDraw->top, SRCAND);
	BitBlt(g_hOffScreenDC,
		static_cast<int>(m_DrawPos->x),
		static_cast<int>(m_DrawPos->y),
		m_rtDraw->right,
		m_rtDraw->bottom,
		m_hColorDC,
		m_rtDraw->left,
		m_rtDraw->top,
		SRCINVERT);
	BitBlt(g_hOffScreenDC,
		static_cast<int>(m_DrawPos->x),
		static_cast<int>(m_DrawPos->y),
		m_rtDraw->right,
		m_rtDraw->bottom,
		m_hMaskDC,
		m_rtDraw->left,
		m_rtDraw->top,
		SRCINVERT);
	return true;
}
bool Rendering::Release()
{
	return true;
}
void Rendering::setInverse(const INVERSE& type)
{
	return;
}
void Rendering::setFade(const FLOAT& alpha, const FLOAT& speed)
{
	return;
}
void Rendering::setFix(const FLOAT& alpha, const FLOAT& speed)
{
	return;
}
void * Rendering::operator new(size_t sz, const char* FileName, int iLine)
{
	std::string ad = FileName;
	MEMINFO mem;
	void* pfs = new char[sz];
	mem.addr = pfs;
	mem.filename = ad.substr(ad.find_last_of('\\') + 1, ad.length() - (ad.find_last_of(".cpp") + 4));
	mem.line = iLine;
	mem.dwAllocateTime = timeGetTime();
	MemoryMap.insert(std::make_pair(pfs, mem));
	++::g_inewCount;
	return pfs;
}
void Rendering::operator delete(void * p)
{
	std::map<void*, MEMINFO>::iterator it;
	it = MemoryMap.find(p);
	MemoryMap.erase(it);
	--::g_inewCount;
	delete p;
}

InversionRendering::InversionRendering(Object* ob, const FLOAT& zoom, const INVERSE& inverse) : Rendering(ob),
m_sInverse(inverse), m_fZoom(zoom)
{}
bool InversionRendering::Frame()
{
	m_DrawPos->x = m_CenterPos->x - (m_rtDraw->right * m_fZoom / 2);
	m_DrawPos->y = m_CenterPos->y - (m_rtDraw->bottom * m_fZoom / 2);

	FLOAT d = m_rtDraw->right * m_fZoom / 2;
	FLOAT e = m_rtDraw->bottom * m_fZoom / 2;

	m_rtCollision->left = static_cast<LONG>(m_CenterPos->x - d);
	m_rtCollision->top = static_cast<LONG>(m_CenterPos->y - e);
	m_rtCollision->right = static_cast<LONG>(m_CenterPos->x + d);
	m_rtCollision->bottom = static_cast<LONG>(m_CenterPos->y + e);
	return true;
}
bool InversionRendering::Render()
{
	if (m_hMaskDC == nullptr)
	{
		switch (m_sInverse)
		{
		case INVERSE::LR_ROTATION:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(-m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCCOPY);
			break;
		case INVERSE::TB_ROTATION:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				static_cast<int>(m_rtDraw->right),
				static_cast<int>(-m_rtDraw->bottom),
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCCOPY);
			break;
		case INVERSE::LRTB_ROTATION:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				static_cast<int>(-m_rtDraw->right),
				static_cast<int>(-m_rtDraw->bottom),
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCCOPY);
			break;
		default:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCCOPY);
		}
	}
	else
	{
		switch (m_sInverse)
		{
		case INVERSE::LR_ROTATION:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right * m_fZoom),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(-m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCAND);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right * m_fZoom),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(-m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right * m_fZoom),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(-m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
			break;
		case INVERSE::TB_ROTATION:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				m_rtDraw->right,
				-m_rtDraw->bottom,
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCAND);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				m_rtDraw->right, -m_rtDraw->bottom,
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				m_rtDraw->right, -m_rtDraw->bottom,
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
			break;
		case INVERSE::LRTB_ROTATION:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				-m_rtDraw->right, -m_rtDraw->bottom,
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCAND);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				-m_rtDraw->right, -m_rtDraw->bottom,
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right),
				static_cast<int>(m_DrawPos->y + m_rtDraw->bottom),
				-m_rtDraw->right, -m_rtDraw->bottom,
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
			break;
		default:
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCAND);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hColorDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
			StretchBlt(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hMaskDC,
				m_rtDraw->left, m_rtDraw->top,
				m_rtDraw->right, m_rtDraw->bottom,
				SRCINVERT);
		}
	}
	return true;
}
void InversionRendering::setInverse(const INVERSE& Inverse)
{
	m_sInverse = Inverse;
}

RotateRendering::RotateRendering(Object*ob, const FLOAT& angle) : Rendering(ob), m_fAngle(angle), m_fMaxLength(0.0f)
{}
bool RotateRendering::Init()
{
	m_fMaxLength = static_cast<float>(sqrt((m_rtDraw->right) * (m_rtDraw->right) + (m_rtDraw->bottom) * (m_rtDraw->bottom)));

	m_hRotationDC = CreateCompatibleDC(g_hOffScreenDC);
	m_hRColorDC = CreateCompatibleDC(g_hOffScreenDC);
	m_hRMaskDC = CreateCompatibleDC(g_hOffScreenDC);

	m_hbColorBitmap = CreateCompatibleBitmap(g_hScreenDC, static_cast<int>(m_fMaxLength), static_cast<int>(m_fMaxLength));
	m_hbMaskBitmap = CreateCompatibleBitmap(g_hScreenDC, static_cast<int>(m_fMaxLength), static_cast<int>(m_fMaxLength));
	return true;
}
bool RotateRendering::Frame()
{
	m_fAngle += g_fSecPerFrame * 30.0f;
	getRotateBitmap(m_hbMaskBitmap, m_hMaskDC);
	getRotateBitmap(m_hbColorBitmap, m_hColorDC);
	return true;
}
bool RotateRendering::Render()
{
	HBITMAP oldMask = static_cast<HBITMAP>(SelectObject(m_hRMaskDC, m_hbMaskBitmap));
	HBITMAP oldColor = static_cast<HBITMAP>(SelectObject(m_hRColorDC, m_hbColorBitmap));

	StretchBlt(g_hOffScreenDC,
		static_cast<int>(m_CenterPos->x - (m_fMaxLength / 2)),
		static_cast<int>(m_CenterPos->y - (m_fMaxLength / 2)),
		static_cast<int>(m_fMaxLength),
		static_cast<int>(m_fMaxLength),
		m_hRMaskDC,
		0,
		0,
		static_cast<int>(m_fMaxLength),
		static_cast<int>(m_fMaxLength),
		SRCAND);
	StretchBlt(g_hOffScreenDC,
		static_cast<int>(m_CenterPos->x - (m_fMaxLength / 2)),
		static_cast<int>(m_CenterPos->y - (m_fMaxLength / 2)),
		static_cast<int>(m_fMaxLength),
		static_cast<int>(m_fMaxLength),
		m_hRColorDC,
		0,
		0,
		static_cast<int>(m_fMaxLength),
		static_cast<int>(m_fMaxLength),
		SRCINVERT);
	StretchBlt(g_hOffScreenDC,
		static_cast<int>(m_CenterPos->x - (m_fMaxLength / 2)),
		static_cast<int>(m_CenterPos->y - (m_fMaxLength / 2)),
		static_cast<int>(m_fMaxLength),
		static_cast<int>(m_fMaxLength),
		m_hRMaskDC,
		0,
		0,
		static_cast<int>(m_fMaxLength),
		static_cast<int>(m_fMaxLength),
		SRCINVERT);
	SelectObject(m_hRMaskDC, oldMask);
	SelectObject(m_hRColorDC, oldColor);
	return true;
}
bool RotateRendering::Release()
{
	DeleteDC(m_hRotationDC);
	DeleteDC(m_hColorDC);
	DeleteDC(m_hMaskDC);
	DeleteObject(m_hbColorBitmap);
	DeleteObject(m_hbMaskBitmap);
	return true;
}
void RotateRendering::getRotateBitmap(HBITMAP hbit, HDC MemDC)
{
	FLOAT fRadian = static_cast<FLOAT>(DegreeToRadian(m_fAngle));
	FLOAT Cosine = cos(fRadian);
	FLOAT Sine = sin(fRadian);

	HBRUSH bkBrush = CreateSolidBrush(RGB(255, 255, 255));

	HBITMAP oldBItmap = static_cast<HBITMAP>(SelectObject(m_hRotationDC, hbit));
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(m_hRotationDC, bkBrush));

	PatBlt(m_hRotationDC, 0, 0,
		static_cast<int>(m_fMaxLength),
		static_cast<int>(m_fMaxLength), PATCOPY);

	DeleteObject(SelectObject(m_hRotationDC, oldBrush));

	int prevGraphic = SetGraphicsMode(m_hRotationDC, GM_ADVANCED);

	XFORM xform;
	xform.eM11 = Cosine;
	xform.eM12 = -Sine;
	xform.eM21 = Sine;
	xform.eM22 = Cosine;
	xform.eDx = m_fMaxLength / 2;
	xform.eDy = m_fMaxLength / 2;

	SetWorldTransform(m_hRotationDC, &xform);

	BitBlt(m_hRotationDC,
		static_cast<int>(-m_rtDraw->right / 2),
		static_cast<int>(-m_rtDraw->bottom / 2),
		m_rtDraw->right,
		m_rtDraw->bottom,
		MemDC,
		m_rtDraw->left, m_rtDraw->top, SRCCOPY);

	xform.eM11 = 1;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = 1;
	xform.eDx = 0;
	xform.eDy = 0;

	SetWorldTransform(m_hRotationDC, &xform);

	SelectObject(m_hRotationDC, oldBItmap);
	SetGraphicsMode(m_hRotationDC, prevGraphic);
}

AlphaRendering::AlphaRendering(Object* ob, const INVERSE& type, const FLOAT& fZoom) : Rendering(ob), m_fFadeSpeed(0.0f), m_fZoom(fZoom), m_sInverse(type)
{}
bool AlphaRendering::Init()
{
	m_bBlend.BlendOp = AC_SRC_OVER;
	m_bBlend.BlendFlags = 0;
	m_bBlend.SourceConstantAlpha = m_fAlpha;
	m_bBlend.AlphaFormat = AC_SRC_OVER;
	return true;
}
bool AlphaRendering::Frame()
{
	m_fAlpha += g_fSecPerFrame * m_fFadeSpeed;
	if (m_fZoom > 1.0f)
	{
		if (m_fAlpha > 255)
		{
			m_fAlpha = 0;
			return false;
		}
		else if (m_fAlpha < 128)
		{
			m_fAlpha = 255;
			return false;
		}
	}
	else
	{
		if (m_fAlpha > 255)
		{
			m_fAlpha = 0;
			return false;
		}
		else if (m_fAlpha < 0)
		{
			m_fAlpha = 255;
			return false;
		}
	}
	m_bBlend.SourceConstantAlpha = m_fAlpha;
	return true;
}
bool AlphaRendering::Render()
{
	if (m_hMaskDC == nullptr)
	{
		::AlphaBlend(g_hOffScreenDC,
			m_DrawPos->x,
			m_DrawPos->y,
			m_rtDraw->right,
			m_rtDraw->bottom,
			m_hColorDC,
			m_rtDraw->left,
			m_rtDraw->top,
			m_rtDraw->right,
			m_rtDraw->bottom,
			m_bBlend);
	}
	else
	{
		switch (m_sInverse)
		{
		case INVERSE::LR_ROTATION:
			AlphaBlend(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x + m_rtDraw->right * m_fZoom),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(-m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hColorDC,
				m_hMaskDC,
				m_rtDraw->left,
				m_rtDraw->top,
				m_rtDraw->right,
				m_rtDraw->bottom,
				m_fAlpha);
		default:
			AlphaBlend(g_hOffScreenDC,
				static_cast<int>(m_DrawPos->x),
				static_cast<int>(m_DrawPos->y),
				static_cast<int>(m_rtDraw->right * m_fZoom),
				static_cast<int>(m_rtDraw->bottom * m_fZoom),
				m_hColorDC,
				m_hMaskDC,
				m_rtDraw->left,
				m_rtDraw->top,
				m_rtDraw->right,
				m_rtDraw->bottom,
				m_fAlpha);
		}
	}
	return true;
}
void AlphaRendering::setFade(const FLOAT& alpha, const FLOAT& speed)
{
	m_fFadeSpeed = speed;
	m_fAlpha = alpha;
	return;
}
bool AlphaRendering::AlphaBlend(HDC dcDest, int x, int y, int cx, int cy,
	HDC dcSrc,
	HDC dcMaskSrc, int sx, int sy, int scx, int scy, int alpha, DWORD opMode, COLORREF rgbMask)
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// Create DIB section in shared memory
	hbmSrc = CreateDIBSection(dcSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// Create DIB section in shared memory
	hbmDest = CreateDIBSection(dcDest, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);


	BYTE * pMaskBits;
	HBITMAP hbmMask;
	// Create DIB section in shared memory
	hbmMask = CreateDIBSection(dcMaskSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pMaskBits, 0, 0l);

	HDC dc = CreateCompatibleDC(NULL);

	HBITMAP dcOld = (HBITMAP)SelectObject(dc, hbmSrc);

	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	dcOld = (HBITMAP)SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;

	dcOld = (HBITMAP)SelectObject(dc, hbmMask);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcMaskSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);


	int red = GetRValue(rgbMask);
	int green = GetGValue(rgbMask);
	int blue = GetBValue(rgbMask);

	for (int j = 0; j < cy; ++j)
	{
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];
		LPBYTE pbMaskRGB = (LPBYTE)&((DWORD*)pMaskBits)[j * cx];

		for (int i = 0; i < cx; ++i)
		{
			if (pbMaskRGB[0] == blue &&
				pbMaskRGB[1] == green &&
				pbMaskRGB[2] == red)
			{
				pbSrcRGB[0] = (pbDestRGB[0] * (255 - alpha) + pbSrcRGB[0] * alpha) >> 8;
				pbSrcRGB[1] = (pbDestRGB[1] * (255 - alpha) + pbSrcRGB[1] * alpha) >> 8;
				pbSrcRGB[2] = (pbDestRGB[2] * (255 - alpha) + pbSrcRGB[2] * alpha) >> 8;

			}
			else
			{
				pbSrcRGB[0] = pbDestRGB[0];
				pbSrcRGB[1] = pbDestRGB[1];
				pbSrcRGB[2] = pbDestRGB[2];
			}
			pbSrcRGB += 4;
			pbDestRGB += 4;
			pbMaskRGB += 4;
		}
	}

	dc = CreateCompatibleDC(NULL);

	SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, opMode))
		return false;
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);
	DeleteObject(hbmMask);
	return true;
}