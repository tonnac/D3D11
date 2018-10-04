#pragma once
#include "SceneMgr.h"
#include "Inventory.h"
#include "FSMMgr.h"
#include "Enemy.h"

class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	Enemy				m_Enemy;
};