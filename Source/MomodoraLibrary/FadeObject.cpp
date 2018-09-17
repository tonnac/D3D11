#include "FadeObject.h"

FadeObject::FadeObject() : m_fDecrement(150.5f)
{}

FadeObject::~FadeObject()
{
	Release();
}

bool FadeObject::Init()
{
	m_fAlpha = 255.0f;
	m_hMemDC = CreateCompatibleDC(g_hOffScreenDC);
	m_hOffBitmap = CreateCompatibleBitmap(g_hScreenDC, g_rtClient.right, g_rtClient.bottom);
	m_bkBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(m_hMemDC, m_hOffBitmap);
	SelectObject(m_hMemDC, m_bkBrush);


	m_bBlend.BlendOp = AC_SRC_OVER;
	m_bBlend.BlendFlags = 0;
	m_bBlend.SourceConstantAlpha = m_fAlpha;
	m_bBlend.AlphaFormat = AC_SRC_OVER;

	return true;
}
bool FadeObject::Frame()
{
	m_fAlpha -= g_fSecPerFrame * m_fDecrement;
	if (m_fAlpha < 0.0f || m_fAlpha > 255.0f)
	{
		return false;
	}
	m_bBlend.SourceConstantAlpha = m_fAlpha;
	return true;
}
bool FadeObject::Render()
{
	::AlphaBlend(g_hOffScreenDC,
		0,
		0,
		g_rtClient.right,
		g_rtClient.bottom,
		m_hMemDC,
		0,
		0,
		g_rtClient.right,
		g_rtClient.bottom,
		m_bBlend);
	return true;
}
bool FadeObject::Release()
{
	DeleteDC(m_hMemDC);
	DeleteObject(m_bkBrush);
	DeleteObject(m_hOffBitmap);
	return true;
}
bool FadeObject::SetAlpha(const FLOAT& alpha)
{
	m_fAlpha = alpha;
	m_bBlend.SourceConstantAlpha = m_fAlpha;
	return true;
}
bool FadeObject::Decrease(const FLOAT& decre)
{
	m_fDecrement = decre;
	return true;
}