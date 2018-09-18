#pragma once
#include "Object.h"
#include "SpriteMgr.h"

class EffectObj : public Object
{
public:
	virtual bool		Init		();
	virtual bool		Frame		();
	void				setSpeed	(const FLOAT&);
	void				setDrawPos	(const FloatPoint&);
	void				setIndex	(const int&);
protected:
	Sprite *			m_pEffectSprite;
};