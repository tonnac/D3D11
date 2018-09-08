#pragma once
#include "Window.h"
#include "Timer.h"
#include "DirectWrite.h"

//#define DEVICE_INFO

class Core : public Window
{
public:
	bool			GameInit		() override;
	bool			GameRun			() override;
	bool			GameRelease		() override;
public:
	virtual bool	Init			();
	virtual bool	Frame			();
	virtual bool	Render			();
	virtual bool	Release			();
public:
	virtual void	DeleteDeviceResources() override;
	virtual HRESULT	CreateDeviceResources() override;
private:
	bool			GameFrame();
	bool			GameRender();
	bool			GamePreRender();
	bool			GamePostRender();
private:
	Timer			m_Timer;
	DirectWrite		m_DirectWrite;
};