#pragma once
#include "d3dUtil.h"

class Timer
{
public:
	Timer();
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_CurrentTick;
	LARGE_INTEGER	m_OneSecTick;
	LARGE_INTEGER	m_BeforeTick;

	DWORD			m_dwFPS;
	std::tstring	m_TimerInfo;

	bool			FrameInfo = true;
};