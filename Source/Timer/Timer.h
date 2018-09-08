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
	void			Reset();
	void			Start();
	void			Stop();
	bool			IsStarted() const;
	FLOAT			getElapsedTime();
public:
	INT				getFPS() const;
	FLOAT			getSPF() const;
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_Current;
	LARGE_INTEGER	m_Before;
	LARGE_INTEGER	m_FPSElapsed;
	LARGE_INTEGER	m_OneSecTime;
	FLOAT			m_fSecPerFrame;
	FLOAT			m_fGameTime;
	INT				m_iFrameCount;
private:
	LARGE_INTEGER	m_StartEvent;
	LARGE_INTEGER	m_Elapsed;
	bool			m_bStarted;
	FLOAT			m_fEventTime;
};