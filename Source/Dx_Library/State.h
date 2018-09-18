#pragma once
#include "SpriteMgr.h"

class CharacterObject;
class EffectObj;
class Player;

class State
{
public:
	State(CharacterObject*);
public:
	virtual bool		Init		() = 0;
	virtual bool		Frame		();
	virtual bool		Render		();
	virtual bool		Release		();
	virtual bool		Process		(Player*);
public:
	void				setSprite	(const std::tstring& Name, const std::tstring Sprite);
public:
	D2D1_RECT_F			getEffectRT	();
protected:
	Sprite *			m_pSprite;
	CharacterObject*	m_pCharObj;
	EffectObj *			m_pEffectObj;
	FLOAT				m_fTimer;
};
