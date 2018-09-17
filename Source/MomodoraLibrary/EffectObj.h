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
public:
	void * operator new(size_t sz, const char* FileName, int iLine);
	void operator delete(void * p);
protected:
	Sprite *			m_pEffectSprite;
};