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
	KahoAttack1			m_Effect;
	KahoAttack2			m_Effect1;
	KahoAttack3			m_Effect2;
	KahoAirAttack		m_Effect3;
	Player				m_Character;
	Sprite*				m_pSprite;
};