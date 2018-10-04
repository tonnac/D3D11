#pragma once
#include "Sprite.h"
#include "Character.h"
#include "DirectInput.h"

class EffectObj;

class State
{
public:
	State();
public:
	virtual bool		Init		();
	virtual bool		Frame		() = 0;
public:
	void				setSprite	(const std::tstring& name, const std::tstring& Spritename);
protected:
	INT					m_iResetindex;
	Sprite *			m_pSprite;
	RECT*				m_rtDraw;
	FLOAT				m_fTimer;
};