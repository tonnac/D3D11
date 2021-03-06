#pragma once
#include "wClass.h"
#include "DeviceInfo.h"
#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"
#include "mSound.h"

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
public:
	void			ResizeDevice(const LONG& Width, const LONG& Height) override;
private:
	bool			DeviceInit();
	bool			PreInit();
	bool			GameFrame();
	bool			PreRender();
	bool			GameRender();
	bool			PostRender();
	bool			MatrixInit();
private:
	Timer			m_Timer;
};