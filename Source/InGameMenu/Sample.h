#pragma once
#include "SceneMgr.h"
#include "InGameMenu.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	InGameMenu			m_Menu;
};