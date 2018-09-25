#pragma once
#include "Core.h"
#include "SceneMgr.h"
#include "Player.h"
#include "Sprite.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	Player				m_Character;
	Sprite*				m_pSprite;
};