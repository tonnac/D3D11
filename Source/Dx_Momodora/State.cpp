#include "State.h"

//#include "EffectObj.h"

State::State(Character* pCharacter) : m_fTimer(0.0f), m_pCharacter(pCharacter), m_iResetindex(0)
//m_pEffectObj(nullptr)
{}
void State::setSprite(const std::tstring& name, const std::tstring& Spritename)
{
	m_pSprite = S_Sprite.LoadSprite(name, Spritename);
}
//RECT State::getEffectRT()
//{
//	if (m_pEffectObj)
//	{
//		return *m_pEffectObj->getCollisionRt();
//	}
//	return RECT();
//}
bool State::Init()
{
	return true;
}
bool State::Frame()
{
	m_pCharacter->SetTexPos(m_pSprite->getSpriteVt());
	if (m_pSprite->Frame() == false)
	{
		m_pSprite->setIndex(m_iResetindex);
		return false;
	}
	return true;
}
bool State::Release()
{
	return true;
}