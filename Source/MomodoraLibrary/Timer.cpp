#include "Timer.h"

float g_fSecPerFrame;

bool Timer::Init()
{
	m_dwBeforeTick = timeGetTime();
	ZeroMemory(m_csBuffer, sizeof(TCHAR) * 256);
	m_dwInterval = 1000.0f / 60;
	return true;
}
bool Timer::Frame()
{
	DWORD dwCurrentTick = timeGetTime();
	DWORD ElapsedTick = dwCurrentTick - m_dwBeforeTick;
	//int dwDelay = m_dwInterval - ElapsedTick;
	//if (dwDelay > 0)
	//{
	//	Sleep(dwDelay);
	//}
	m_dwBeforeTick = dwCurrentTick;
//	m_fSecPerFrame = g_fSecPerFrame = m_dwInterval / 1000.0f;
	m_fSecPerFrame = g_fSecPerFrame = ElapsedTick / 1000.0f;
	m_fGameTime += m_fSecPerFrame;
	m_fTimer += m_fSecPerFrame;
	if (m_fTimer >= 1.0f)
	{
		m_fTimer -= 1.0f;
		_stprintf_s(m_csBuffer, L"FPS : %d   Time : %5.3f   SPF : %3.4f", m_dwFrameCount, m_fGameTime, m_fSecPerFrame);
		m_dwFrameCount = 0;
	}
	m_dwFrameCount++;
	return true;
}
bool Timer::Render()
{
	SetTextColor(g_hOffScreenDC, RGB(255, 255, 255));
	SetBkColor(g_hOffScreenDC, RGB(0, 0, 0));
	TextOut(g_hOffScreenDC, 0, 0, m_csBuffer, static_cast<int>(wcslen(m_csBuffer)));
	return true;
}
bool Timer::Release()
{
	return true;
}

Timer::Timer()
{
	m_fSecPerFrame = 0.0f;
	m_fTimer = 0.0f;
	m_fGameTime = 0.0f;
	m_dwBeforeTick = 0;
	m_dwFrameCount = 0;
}