#include "Core.h"

bool Core::GameInit()
{
	Init();
	return true;
}
bool Core::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}
bool Core::GameRelease()
{
	Release();
	return true;
}

bool Core::Init()
{
	return true;
}
bool Core::Frame()
{
	return true;
}
bool Core::Render()
{
	return true;
}
bool Core::Release()
{
	return true;
}
bool Core::GameFrame()
{
	Frame();
	return true;
}
bool Core::GameRender()
{
	GamePreRender();
	Render();
	GamePostRender();
	return true;
}
bool Core::GamePreRender()
{
	return true;
}
bool Core::GamePostRender()
{
	return true;
}