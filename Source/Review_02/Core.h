#pragma once
#include "wClass.h"
#include "DeviceInfo.h"
#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"

//#define DEVICE_INFO

class Core : public wClass
{
public:
	bool			GameInit() override;
	bool			GameRun() override;
	bool			GameRelease() override;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
private:
	bool			GameFrame();
	bool			PreRender();
	bool			GameRender();
	bool			PostRender();
private:
	Timer			m_Timer;
};