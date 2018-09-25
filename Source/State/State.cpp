#include "State.h"

//#include "EffectObj.h"

State::State(Character* pCharacter) : m_fTimer(0.0f), m_pCharacter(pCharacter)
//m_pEffectObj(nullptr)
{}
void State::setSprite(const std::tstring& name, const std::tstring& Spritename)
{
	S_Sprite.SpriteSet(L"Kaho.txt");
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
		m_pSprite->setIndex(0);
	}
	return true;
}