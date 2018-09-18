#include "EffectObj.h"

bool EffectObj::Init()
{
	return true;
}
bool EffectObj::Frame()
{
	if (!m_pEffectSprite->Frame())
	{
		m_pEffectSprite->setIndex(0);
		return false;
	}
	m_rtDraw = m_pEffectSprite->getSpriteRt();
	Object::Frame();
	return true;
}
void EffectObj::setSpeed(const FLOAT& pt)
{
	m_fSpeed = pt;
}
void EffectObj::setDrawPos(const FloatPoint& pt)
{
	m_CenterPos.x = pt.x;
	m_CenterPos.y = pt.y;
}
void EffectObj::setIndex(const int& index)
{
	m_pEffectSprite->setIndex(index);
}