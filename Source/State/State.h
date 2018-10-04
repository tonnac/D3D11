#pragma once
#include "Sprite.h"
#include "Character.h"

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
	virtual void		setSprite	(const std::tstring& name, const std::tstring& Spritename);
public:
//	RECT				getEffectRT	();
protected:
	Character *			m_pCharacter;
	Sprite *			m_pSprite;
	D3DXVECTOR2*		m_CenterPos;
	RECT*				m_rtDraw;
//	EffectObj *			m_pEffectObj;
	FLOAT				m_fTimer;
};
