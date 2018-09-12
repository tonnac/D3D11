#pragma once
#include "WClass.h"
#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"
#include "Enumeration.h"

//#define DEVICE_INFO

class Core : public WClass
{
public:
	Core();
public:
	bool			GameInit() override;
	bool			GameRun() override;
	bool			GameRelease() override;
public:
	virtual	bool	Init();
	virtual	bool	Frame();
	virtual	bool	Render();
	virtual	bool	Release();
private:
	bool			GameFrame();
	bool			GameRender();
	bool			PreInit();
	bool			PreRender();
	bool			PostRender();
private:
	virtual void	ResizeDiscard() override;
	virtual void	ResizeCreate() override;
private:
	Timer			m_Timer;
};