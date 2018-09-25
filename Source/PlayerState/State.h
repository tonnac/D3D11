#pragma once
#include "Sprite.h"
#include "Character.h"
#include "DirectInput.h"

class EffectObj;
class Player;

class State
{
public:
	State(Character* pCharacter);
public:
	virtual bool		Init		();
	virtual bool		Frame		();
public:
	void				setSprite	(const std::tstring& name, const std::tstring& Spritename);
public:
//	RECT				getEffectRT	();
protected:
	INT					m_iResetindex;
	Character *			m_pCharacter;
	Sprite *			m_pSprite;
	RECT*				m_rtDraw;
//	EffectObj *			m_pEffectObj;
	FLOAT				m_fTimer;
};