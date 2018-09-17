#include "KahoAttack.h"

bool KahoAttack1::Init()
{
	m_pEffectSprite = S_SpriteMgr.LoadSprite(L"Kaho", L"Leaf1");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	Object::Init();
	return true;
}
bool KahoAttack1::Frame()
{
	EffectObj::Frame();
	if (m_pEffectSprite->getIndex() < 4)
	{
		m_rtCollision.left = 0;
		m_rtCollision.right = 0;
		m_rtCollision.bottom = 0;
		m_rtCollision.top = 0;
	}
	return true;
}

bool KahoAttack2::Init()
{
	m_pEffectSprite = S_SpriteMgr.LoadSprite(L"Kaho", L"Leaf2");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	Object::Init();
	return true;
}
bool KahoAttack2::Frame()
{
	EffectObj::Frame();
	if (m_pEffectSprite->getIndex() < 4)
	{
		m_rtCollision.left = 0;
		m_rtCollision.right = 0;
		m_rtCollision.bottom = 0;
		m_rtCollision.top = 0;
	}
	return true;
}

bool KahoAttack3::Init()
{
	m_pEffectSprite = S_SpriteMgr.LoadSprite(L"Kaho", L"Leaf3");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.7f);
	Object::Init();
	return true;
}
bool KahoAttack3::Frame()
{
	EffectObj::Frame();
	if (m_pEffectSprite->getIndex() < 5)
	{
		m_rtCollision.left = 0;
		m_rtCollision.right = 0;
		m_rtCollision.bottom = 0;
		m_rtCollision.top = 0;
	}
	return true;
}

bool KahoAirAttack::Init()
{
	m_pEffectSprite = S_SpriteMgr.LoadSprite(L"Kaho", L"AirLeaf");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.5f);
	Object::Init();
	return true;
}
bool KahoAirAttack::Frame()
{
	EffectObj::Frame();
	if (m_pEffectSprite->getIndex() < 2)
	{
		m_rtCollision.left = 0;
		m_rtCollision.right = 0;
		m_rtCollision.bottom = 0;
		m_rtCollision.top = 0;
	}
	return true;
}

bool KahoBowAttack::Init()
{
	LoadFile(L"PLAYER", L"../../../../data/bmp/KahoColor.bmp", L"../../../../data/bmp/KahoMask.bmp");
	m_pEffectSprite = S_SpriteMgr.LoadSprite(L"Kaho", L"ArrowS");
	m_pEffectSprite->setIndex(0);
	m_pEffectSprite->setDivideTime(0.5f);
	setRendering(2.8f, INVERSE::DEFAULT);
	return true;
}
bool KahoBowAttack::Frame()
{
	m_CenterPos.x += g_fSecPerFrame * m_fSpeed * 7.0f;
	EffectObj::Frame();
	return true;
}