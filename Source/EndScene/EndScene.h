#pragma once
#include "Scene.h"

class EndScene : public Scene
{
public:
	bool	Init() override;
	bool	Frame() override;
	bool	Render() override;
	bool	Release() override;
private:
	bool	isEnd;
};