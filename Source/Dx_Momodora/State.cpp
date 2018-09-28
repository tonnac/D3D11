#include "State.h"

State::State() : m_fTimer(0.0f), m_iResetindex(0)
{}
void State::setSprite(const std::tstring& name, const std::tstring& Spritename)
{
	m_pSprite = S_Sprite.LoadSprite(name, Spritename);
}
bool State::Init()
{
	return true;
}