#pragma once
#include "Window.h"

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
private:
	bool			GameFrame();
	bool			GameRender();
	bool			GamePreRender();
	bool			GamePostRender();
private:

};