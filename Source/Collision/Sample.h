#pragma once
#include "SceneMgr.h"
#include "Player.h"
//#include "Effect.h"
#include "KahoAttack.h"

class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	Player				m_Character;
	KahoBowAttack*		m_Bow;
};