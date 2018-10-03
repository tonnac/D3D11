#pragma once
#include "SceneMgr.h"
#include "Inventory.h"
#include "GameUI.h"

class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	GameUI				m_ui;
};