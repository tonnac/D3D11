#pragma once

#ifndef TIMER_H
#define TIMER

class Timer
{
public:
	Timer();

	float TotalTime() const;
	float DeltaTime() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();

private:
	double m_SecondsPerCount = 0.0;
	double m_DeltaTime = -1.0;

	__int64 m_BaseTime = 0;
	__int64 m_PausedTime = 0;
	__int64 m_StopTime = 0;
	__int64 m_PrevTime = 0;
	__int64 m_CurrTime = 0;

	bool m_Stopped = false;
};

#endif // TIMER_H