#pragma once
#include "Core.h"
#include "Timer.h"

class Sample : public Core
{
public:
	Sample();
public:
	bool		Init() override;
	bool		Frame() override;
	bool		Render() override;
	bool		Release() override;
private:
	Timer		m_Timer;
};