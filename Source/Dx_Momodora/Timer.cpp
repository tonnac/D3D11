#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"

FLOAT g_fSecPerFrame = 0.0f;
FLOAT g_fGameTime = 0.0f;


Timer::Timer()
{
	ZeroMemory(&m_Frequency, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurrentTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeforeTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OneSecTick, sizeof(LARGE_INTEGER));
}

bool Timer::Init()
{
	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_BeforeTick);
	m_OneSecTick = m_BeforeTick;
	return true;
}
bool Timer::Frame()
{
	QueryPerformanceCounter(&m_CurrentTick);
	g_fSecPerFrame = (m_CurrentTick.LowPart - m_BeforeTick.LowPart) / CASTING(FLOAT, m_Frequency.LowPart);
	g_fGameTime += g_fSecPerFrame;

	if ((m_CurrentTick.LowPart - m_OneSecTick.LowPart) / CASTING(FLOAT, m_Frequency.LowPart) > 1.0f)
	{
		m_TimerInfo = L"FPS: " + std::to_tstring(m_dwFPS) + L"   Time: " + std::to_tstring(CASTING(int, g_fGameTime)) + L"   SPF: " + std::to_tstring(g_fSecPerFrame) + L"\n";
		m_dwFPS = 0;
		m_OneSecTick = m_CurrentTick;
	}

	if (S_Input.getKeyState(DIK_LCONTROL) == Input::KEYSTATE::KEY_HOLD && S_Input.getKeyState(DIK_F) == Input::KEYSTATE::KEY_PUSH)
	{
		FrameInfo = !FrameInfo;
	}

	++m_dwFPS;

	m_BeforeTick = m_CurrentTick;
	return true;
}
bool Timer::Render()
{
	if (FrameInfo == true)
	{
		S_Write.DrawText({ 0,0,800,600 }, m_TimerInfo, D2D1::ColorF::Black);
	}
	return true;
}
bool Timer::Release()
{
	return true;
}