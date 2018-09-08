#include "Timer.h"
#include <sstream>
FLOAT	g_fSecPerFrame = 0.0f;

Timer::Timer() : m_bStarted(false), m_iFrameCount(0), m_fSecPerFrame(0.0f), m_fEventTime(0.0f), m_fGameTime(0.0f)
{
	ZeroMemory(&m_Frequency, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Current, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Before, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FPSElapsed, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_StartEvent, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Elapsed, sizeof(LARGE_INTEGER));
	QueryPerformanceFrequency(&m_Frequency);
}

bool Timer::Init()
{
	QueryPerformanceCounter(&m_Before);

	if (m_Before.QuadPart == 0) // 타이머 지원 안하는경우
	{
		return false;
	}
	m_FPSElapsed = m_Before;
	return true;
}
bool Timer::Frame()
{
	QueryPerformanceCounter(&m_Current);

	m_fSecPerFrame = g_fSecPerFrame = static_cast<FLOAT>(m_Current.QuadPart - m_Before.QuadPart) / static_cast<FLOAT>(m_Frequency.QuadPart);
	m_fGameTime += m_fSecPerFrame;
	if ((m_Current.LowPart - m_FPSElapsed.LowPart) / m_Frequency.LowPart >= 1)
	{
		std::tsstream is;
		is << std::fixed << std::setprecision(3) << L"FPS : " << m_iFrameCount << " Timer : " << m_fGameTime << " SPF : " << std::setprecision(5) << m_fSecPerFrame;
		m_Buffer = is.str();
		m_iFrameCount = 0;
		m_FPSElapsed = m_Current;
	}
	++m_iFrameCount;
	m_Before = m_Current;
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
void Timer::Reset()
{
	m_bStarted = false;
	m_fEventTime = 0.0f;
	ZeroMemory(&m_StartEvent, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Elapsed, sizeof(LARGE_INTEGER));
}
void Timer::Start()
{
	m_bStarted = true;
	if (m_fEventTime > 0.0f)
	{
		return;
	}
	QueryPerformanceCounter(&m_StartEvent);
}
void Timer::Stop()
{
	getElapsedTime();
	m_bStarted = false;
}
bool Timer::IsStarted() const
{
	return m_bStarted;
}
FLOAT Timer::getElapsedTime()
{
	if (m_bStarted == true)
	{
		QueryPerformanceCounter(&m_Elapsed);
		m_fEventTime = static_cast<FLOAT>(m_Elapsed.LowPart - m_StartEvent.LowPart);
	}
	return m_fEventTime;
}
INT Timer::getFPS() const
{
	return m_iFrameCount;
}
FLOAT Timer::getSPF() const
{
	return m_fSecPerFrame;
}
std::tstring Timer::getBuffer() const
{
	return m_Buffer;
}