#include "PlayerAttack.h"
#include "KahoAttack.h"
//#include "mSound.h"
#include "SceneMgr.h"

MeleeAttack::MeleeAttack(Player* pPlayer) : PlayerState(pPlayer)
{}

PlayerAttack::PlayerAttack(Player * pPlayer) : MeleeAttack(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Attack1"), this);
}
bool PlayerAttack::Init()
{
	setSprite(L"Kaho", L"Attack1");
	m_pSprite->setDivideTime(0.7f);
	return true;
}
bool PlayerAttack::Frame()
{
	m_pCharacter->MoveCenterPos({ m_pCharacter->getDir() * g_fSecPerFrame * 30.0f, 0.0f });

	m_fTimer += g_fSecPerFrame;

	if (m_fTimer >= 0.4f && S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
	{
		m_fTimer = 0.0f;
		S_Sound.Play(Effect_Snd::ATTACK2);
		m_pCharacter->setState(L"Attack2");
		m_pSprite->setIndex(0);
		D3DXVECTOR2 Center = m_pCharacter->getCenterPos();
		if (m_pCharacter->getDir() == 1)
		{
			Center.x += 70.0f;
			g_Attack2->setDir(1);
		}
		else
		{
			Center.x -= 70.0f;
			g_Attack2->setDir(-1);
		}
		g_Attack2->SetCenterPos(Center);
		S_Object.AddPlayerEffect(g_Attack2);
		return true;
	}

	if (State::Frame() == false)
	{
		m_fTimer = 0.0f;
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

PlayerAttack2::PlayerAttack2(Player * pPlayer) : MeleeAttack(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Attack2"), this);
}
bool PlayerAttack2::Init()
{
	setSprite(L"Kaho", L"Attack2");
	m_pSprite->setDivideTime(0.7f);
	return true;
}
bool PlayerAttack2::Frame()
{
	m_pCharacter->MoveCenterPos({ m_pCharacter->getDir() * g_fSecPerFrame * 30.0f, 0.0f });

	m_fTimer += g_fSecPerFrame;

	if (m_fTimer >= 0.4f && S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
	{
		m_fTimer = 0.0f;
		S_Sound.Play(Effect_Snd::ATTACK3);
		m_pCharacter->setState(L"Attack3");
		m_pSprite->setIndex(0);
		D3DXVECTOR2 Center = m_pCharacter->getCenterPos();
		if (m_pCharacter->getDir() == 1)
		{
			Center.x += 100.0f;
			g_Attack3->setDir(1);
		}
		else
		{
			Center.x -= 100.0f;
			g_Attack3->setDir(-1);
		}
		g_Attack3->SetCenterPos(Center);
		S_Object.AddPlayerEffect(g_Attack3);
		return true;
	}

	if (State::Frame() == false)
	{
		m_fTimer = 0.0f;
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

PlayerAttack3::PlayerAttack3(Player * pPlayer) : MeleeAttack(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Attack3"), this);
}
bool PlayerAttack3::Init()
{
	setSprite(L"Kaho", L"Attack3");
	m_pSprite->setDivideTime(0.7f);
	return true;
}
bool PlayerAttack3::Frame()
{
	m_pCharacter->MoveCenterPos({ m_pCharacter->getDir() * g_fSecPerFrame * 30.0f, 0.0f });

	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

AirAttack::AirAttack(Player * pPlayer) : MeleeAttack(pPlayer), m_fAcceleration(0.5f)
{
	m_pCharacter->AddState(std::tstring(L"AirAttack"), this);
}
bool AirAttack::Init()
{
	setSprite(L"Kaho", L"AirAttack");
	m_pSprite->setDivideTime(0.5f);
	return true;
}
bool AirAttack::Frame()
{
	m_pCharacter->MoveCenterPos({ m_pCharacter->getDir() * g_fSecPerFrame * 30.0f, 0.0f });

	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Fall");
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////

BowAttack::BowAttack(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Bow"), this);
}
bool BowAttack::Init()
{
	setSprite(L"Kaho", L"Bow");
	m_pSprite->setDivideTime(0.5f);
	return true;
}
bool BowAttack::Frame()
{
	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

AirBowAttack::AirBowAttack(Player * pPlayer) : PlayerState(pPlayer), m_fAcceleration(0.25f)
{
	m_pCharacter->AddState(std::tstring(L"AirBow"), this);
}
bool AirBowAttack::Init()
{
	setSprite(L"Kaho", L"AirBow");
	m_pSprite->setDivideTime(0.5f);
	return true;
}
bool AirBowAttack::Frame()
{
	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Fall");
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

CrouchBowAttack::CrouchBowAttack(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"CrouchBow"), this);
}
bool CrouchBowAttack::Init()
{
	setSprite(L"Kaho", L"CrouchBow");
	m_pSprite->setDivideTime(0.5f);
	return true;
}
bool CrouchBowAttack::Frame()
{
	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Crouch");
	}
	return true;
}