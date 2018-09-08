#include "Timer.h"

FLOAT g_fSecPerFrame = 0.0f;

Timer::Timer() : m_fSecPerFrame(0.0f), m_fGameTime(0.0f), m_FPS(0)
{
	ZeroMemory(&m_CurrentTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeforeTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OneSecTick, sizeof(LARGE_INTEGER));
	QueryPerformanceFrequency(&m_Frequency);
}

bool Timer::Init()
{
	QueryPerformanceCounter(&m_BeforeTick);

	if (m_BeforeTick.QuadPart == 0)
	{
		return false;
	}
	return true;
	m_OneSecTick = m_BeforeTick;
}
bool Timer::Frame()
{
	QueryPerformanceCounter(&m_CurrentTick);

	m_fSecPerFrame = g_fSecPerFrame = (m_CurrentTick.LowPart - m_BeforeTick.LowPart) / CASTING(FLOAT,m_Frequency.LowPart);
	m_fGameTime += m_fSecPerFrame;

	if ((m_CurrentTick.LowPart - m_OneSecTick.LowPart) / m_Frequency.LowPart >= 1)
	{
		TimeInfo = L"FPS : " + std::to_tstring(m_FPS) + L" Time : " + std::to_tstring(CASTING(int, m_fGameTime)) + L" SPF : " + std::to_tstring(m_fSecPerFrame);
		m_FPS = 0;
		m_OneSecTick = m_CurrentTick;
	}
	++m_FPS;

	m_BeforeTick = m_CurrentTick;
	return true;
}
bool Timer::Render()
{
	return true;
}
bool Timer::Release()
{
	return true;
}
std::tstring Timer::getTimeInfo()
{
	return TimeInfo;
}