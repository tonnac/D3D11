#include "State.h"
#include "CharacterObject.h"
#include "EffectObj.h"

State::State(CharacterObject* pObject) : m_pEffectObj(nullptr), m_fTimer(0.0f)
{}
void State::setSprite(const std::tstring& Name, const std::tstring Sprite)
{
	m_pSprite = S_Sprite.LoadSprite(Name, Sprite);
}
RECT State::getEffectRT()
{
	if (m_pEffectObj)
	{
		return *m_pEffectObj->getCollisionRt();
	}
	return RECT();
}
bool State::Frame()
{
	return true;
}
bool State::Render()
{
	return true;
}
bool State::Release()
{
	return true;
}
bool State::Process(Player* pPlayer)
{
	return true;
}