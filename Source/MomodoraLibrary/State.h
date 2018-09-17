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
	void				setSprite	(T_STR, T_STR);
public:
	RECT				getEffectRT	();
public:
	void * operator new(size_t sz, const char* FileName, int iLine);
	void operator delete(void * p);
protected:
//	CharacterObject *	m_pCharObj;
	Sprite *			m_pSprite;
	FloatPoint*			m_CenterPos;
	RECT*				m_rtDraw;
	EffectObj *			m_pEffectObj;
	FLOAT				m_fTimer;
};
