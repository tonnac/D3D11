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
	RECT				getEffectRT	();
protected:
	Sprite *			m_pSprite;
	POINT*				m_CenterPos;
	EffectObj *			m_pEffectObj;
	FLOAT				m_fTimer;
};
