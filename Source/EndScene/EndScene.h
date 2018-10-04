#pragma once
#include "Scene.h"
#include "EmptyScreen.h"
#include "Bar.h"

class EndScene : public Scene
{
public:
	bool		Init() override;
	bool		Frame() override;
	bool		Render() override;
	bool		Release() override;
private:
	bool		bFlag;
	FLOAT		m_fTimer;
	FLOAT		m_ftTimer;
	bool		isEnd;
	Bar			m_EndScene;
	EmptyScreen m_Empty;
};