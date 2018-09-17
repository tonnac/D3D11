#pragma once
#include "Std.h"
class Timer
{
public:
	Timer();
public:
	bool					Init();
	bool					Frame();
	bool					Render();
	bool					Release();
private:
	TCHAR					m_csBuffer[256];
	float					m_fSecPerFrame;
	float					m_fTimer;
	float					m_fGameTime;
	DWORD					m_dwBeforeTick;
	DWORD					m_dwFrameCount;
	DWORD					m_dwInterval;
};