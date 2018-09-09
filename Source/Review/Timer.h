#pragma once
#include "Define.h"

class Timer
{
public:
	Timer();
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
public:
	std::tstring	getTimeInfo();
private:
	bool			FrameShow;
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_CurrentTick;
	LARGE_INTEGER	m_BeforeTick;
	LARGE_INTEGER	m_OneSecTick;
	FLOAT			m_fSecPerFrame;
	FLOAT			m_fGameTime;
	DWORD			m_FPS;
	std::tstring	TimeInfo;
};