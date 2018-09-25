#include "State.h"

//#include "EffectObj.h"

State::State(Character* pCharacter) : m_fTimer(0.0f), m_pCharacter(pCharacter)
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
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pCharacter->MoveCenterPos({ g_fSecPerFrame * 200.0f, 0.0f });
	}
	m_pCharacter->SetTexPos(m_pSprite->getSpriteVt());
	if (m_pSprite->Frame() == false)
	{
		m_pSprite->setIndex(0);
	}
	return true;
}
