#pragma once
#include "WClass.h"

class Core : public WClass
{
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

};