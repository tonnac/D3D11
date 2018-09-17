#include "PlayerBasicState.h"
#include "KahoAttack.h"
#include "TerrainObject.h"
#include "mSound.h"

PlayerState::PlayerState(Player * pPlayer) : State(pPlayer), m_pCharObj(pPlayer)
{
	m_CenterPos = pPlayer->getCenterPos();
	m_rtDraw = pPlayer->getrtDraw();
}
bool PlayerState::Render()
{
	return true;
}
bool PlayerState::Release()
{
	if (m_pEffectObj)
	{
		m_pEffectObj->Release();
		delete m_pEffectObj;
	}
	return true;
}

PlayerIdle::PlayerIdle(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Idle"), this);
}
bool PlayerIdle::Init()
{
	setSprite(L"Kaho", L"Idle");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerIdle::Frame()
{
	m_CenterPos->y += g_fSecPerFrame * 30.0f;
	if (S_Input.GetKey('A') == KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect::JUMP);
		m_pCharObj->setState(L"Jump");
		return true;
	}
	if (S_Input.GetKey('S') == KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect::ATTACK1);
		m_pCharObj->setState(L"Attack1");
		return true;
	}
	if (S_Input.GetKey('Q') == KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect::ROLL);
		m_pCharObj->setState(L"Roll");
		return true;
	}
	if (S_Input.GetKey('D') == KEYSTATE::KEY_PUSH)
	{
		EffectObj * Arrow = new KahoBowAttack;
		Arrow->Init();
		if (m_pCharObj->getDir() == -1)
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x - 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
			Arrow->setSpeed(-200.0f);
		}
		else
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x + 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setSpeed(200.0f);
		}
		m_pCharObj->addEffect(Arrow);
		S_Sound.Play(Effect::ARROW);
		m_pCharObj->setState(L"Bow");
		return true;
	}
	if (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_HOLD)
	{
		if (m_pCharObj->getDir() == -1)		// 방향 같으면
		{
			m_pCharObj->setState(L"Run");
			return true;
		}
		else								// 방향 다르면
		{
			m_pCharObj->setState(L"Turn");
			return true;
		}
	}
	if (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_HOLD)
	{
		if (m_pCharObj->getDir() == 1)
		{
			m_pCharObj->setState(L"Run");
			return true;
		}
		else
		{
			m_pCharObj->setState(L"Turn");
			return true;
		}
	}
	if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH && m_pCharObj->getLadder())
	{
		m_pCharObj->setState(L"LadderEnter");
		return true;
	}
	if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH && m_pCharObj->getLadder())
	{
		m_pCharObj->setState(L"LadderUp");
		return true;
	}
	if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_HOLD)
	{
		m_pCharObj->setState(L"Crouch");
		return true;
	}

	if (!m_pSprite->Frame())				// 한바퀴 다돌면
	{
		m_pSprite->setIndex(0);
	}
	*(m_rtDraw) = m_pSprite->getSpriteRt();
	return true;
}

PlayerRun::PlayerRun(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Run"), this);
}
bool PlayerRun::Init()
{
	setSprite(L"Kaho", L"Run");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool PlayerRun::Frame()
{
	FLOAT fSpeed = m_pCharObj->getSpeed();
	m_CenterPos->y += g_fSecPerFrame * 2.0f;
	if (!m_pCharObj->getLanding())
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Fall");
		return true;
	}

	if (S_Input.GetKey('S') == KEYSTATE::KEY_PUSH)
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Attack1");
		return true;
	}
	if (S_Input.GetKey('A') == KEYSTATE::KEY_PUSH || S_Input.GetKey('A') == KEYSTATE::KEY_HOLD)
	{
		S_Sound.Play(Effect::JUMP);
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Jump");
		return true;
	}
	if ((m_pCharObj->getDir() == 1 && (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_HOLD))
		|| (m_pCharObj->getDir() == -1 && (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_HOLD)))
	{
		FLOAT fSpeed = m_pCharObj->getSpeed();
		m_CenterPos->x += m_pCharObj->getDir() * g_fSecPerFrame * fSpeed;
		*m_rtDraw = m_pSprite->getSpriteRt();
	}
	else if ((S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_UP || S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_FREE) ||
		(S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_UP || S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_FREE))
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Brake");
		return true;
	}
	if (S_Input.GetKey('Q') == KEYSTATE::KEY_PUSH)
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Roll");
		return true;
	}
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(2);
	}

	return true;
}

PlayerBrake::PlayerBrake(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Brake"), this);
}
bool PlayerBrake::Init()
{
	setSprite(L"Kaho", L"Brake");
	m_pSprite->setDivideTime(0.35f);
	return true;
}
bool PlayerBrake::Frame()
{
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Idle");
		return true;
	}
	if (S_Input.GetKey('S') == KEYSTATE::KEY_PUSH)
	{
		m_pCharObj->setState(L"Attack1");
		return true;
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerTurn::PlayerTurn(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Turn"), this);
}
bool PlayerTurn::Init()
{
	setSprite(L"Kaho", L"Turn");
	m_pSprite->setDivideTime(0.1f);
	return true;
}
bool PlayerTurn::Frame()
{
	if (!m_pSprite->Frame())
	{
		m_pCharObj->setDir(-1);
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Run");
		return true;
	}
	if (m_pCharObj->getDir() == 1)
	{
		m_pCharObj->setRendering(m_pCharObj->getZoom(),INVERSE::LR_ROTATION);
	}
	else
	{
		m_pCharObj->setRendering(m_pCharObj->getZoom(),INVERSE::DEFAULT);
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerJump::PlayerJump(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(m_pCharObj->getJumpSpeed()), m_fAcceleration(-120.0f)
{
	m_pCharObj->addState(std::string("Jump"), this);
}
bool PlayerJump::Init()
{
	setSprite(L"Kaho", L"Jump");
	m_pSprite->setDivideTime(0.5f);
	return true;
}
bool PlayerJump::Frame()
{
	m_pCharObj->setLanding(false);
	m_fTimer += g_fSecPerFrame;
	m_fJumpSpeed += g_fSecPerFrame * 9.8 * m_fAcceleration / 2;
	INT iJumpNumber = m_pCharObj->getJumpNum();
	m_CenterPos->y -= g_fSecPerFrame * m_fJumpSpeed;
	if (S_Input.GetKey('S') == KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect::AIRATTACK);
		m_fTimer = 0.0f;
		m_fAcceleration = -120.0f;
		m_pCharObj->setState(L"AirAttack");
		return true;
	}
	if (m_fJumpSpeed < 0.0f)
	{
		m_fTimer = 0.0f;
		m_fAcceleration = -120.0f;
		m_pCharObj->setState(L"Fall");
		return true;
	}
	if (iJumpNumber == 0 && (S_Input.GetKey('A') == KEYSTATE::KEY_PUSH))
	{
		S_Sound.Play(Effect::JUMP);
		m_fTimer = 0.0f;
		m_fAcceleration = -120.0f;
		m_pSprite->setIndex(0);
		m_pCharObj->setJumpNum(1);
		m_pCharObj->setState(L"Jump2");
		return true;
	}
	FLOAT fSpeed = m_pCharObj->getSpeed();
	if (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_HOLD)
	{
		if (m_pCharObj->getDir() == -1)
		{
			m_pCharObj->setRendering(m_pCharObj->getZoom(), INVERSE::DEFAULT);
			m_pCharObj->setDir(-1);
		}
		else
			m_CenterPos->x += g_fSecPerFrame * fSpeed;
	}
	if (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_HOLD)
	{
		if (m_pCharObj->getDir() == 1)
		{
			m_pCharObj->setRendering(m_pCharObj->getZoom(), INVERSE::LR_ROTATION);
			m_pCharObj->setDir(-1);
		}
		else
			m_CenterPos->x -= g_fSecPerFrame * fSpeed;
	}
	if (S_Input.GetKey('D') == KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect::ARROW);
		EffectObj * Arrow = new KahoBowAttack;
		Arrow->Init();
		if (m_pCharObj->getDir() == -1)
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x - 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
			Arrow->setSpeed(-200.0f);
		}
		else
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x + 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setSpeed(200.0f);
		}
		m_pCharObj->addEffect(Arrow);
		m_pCharObj->setState(L"AirBow");
		return true;
	}
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerJump2::PlayerJump2(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(m_pCharObj->getJumpSpeed(1)), m_fAcceleration(-120.0f)
{
	m_pCharObj->addState(std::string("Jump2"), this);
}
bool PlayerJump2::Init()
{
	setSprite(L"Kaho", L"Jump");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool PlayerJump2::Frame()
{
	FLOAT fSpeed = m_pCharObj->getSpeed();
	m_fTimer += g_fSecPerFrame;
	m_fJumpSpeed += g_fSecPerFrame * 9.8 * m_fAcceleration / 2;
	INT iJumpNumber = m_pCharObj->getJumpNum();
	m_CenterPos->y -= g_fSecPerFrame * m_fJumpSpeed;

	if (S_Input.GetKey('S') == KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect::AIRATTACK);
		m_fTimer = 0.0f;
		m_fAcceleration = -120.0f;
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"AirAttack");
		return true;
	}
	if (m_fJumpSpeed < 0.0f)
	{
		m_fTimer = 0.0f;
		m_fAcceleration = -120.0f;
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Fall");
		return true;
	}
	if (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_HOLD)
	{
		m_CenterPos->x += g_fSecPerFrame * fSpeed;
	}
	if (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_HOLD)
	{
		m_CenterPos->x -= g_fSecPerFrame * fSpeed;
	}
	if (S_Input.GetKey('D') == KEYSTATE::KEY_PUSH)
	{
		EffectObj * Arrow = new KahoBowAttack;
		Arrow->Init();
		if (m_pCharObj->getDir() == -1)
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x - 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
			Arrow->setSpeed(-200.0f);
		}
		else
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x + 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setSpeed(200.0f);
		}
		m_pCharObj->addEffect(Arrow);
		S_Sound.Play(Effect::ARROW);
		m_pCharObj->setState(L"AirBow");
		return true;
	}
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerFall::PlayerFall(Player * pPlayer) : PlayerState(pPlayer), m_fAcceleration(1.0f)
{
	m_pCharObj->addState(std::string("Fall"), this);
}
bool PlayerFall::Init()
{
	setSprite(L"Kaho", L"Fall");
	m_pSprite->setDivideTime(0.5f);
	return true;
}
bool PlayerFall::Frame()
{
	m_fTimer += g_fSecPerFrame;
	INT iJumpNum = m_pCharObj->getJumpNum();
	FLOAT fSpeed = m_pCharObj->getSpeed();
	if (S_Input.GetKey('S') == KEYSTATE::KEY_PUSH)
	{
		m_pCharObj->setState(L"AirAttack");
		return true;
	}
	if (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_HOLD)
	{
		m_CenterPos->x += g_fSecPerFrame * fSpeed;
	}
	if (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_HOLD)
	{
		m_CenterPos->x -= g_fSecPerFrame * fSpeed;
	}
	if (S_Input.GetKey('D') == KEYSTATE::KEY_PUSH)
	{
		EffectObj * Arrow = new KahoBowAttack;
		Arrow->Init();
		if (m_pCharObj->getDir() == -1)
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x - 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
			Arrow->setSpeed(-200.0f);
		}
		else
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x + 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setSpeed(200.0f);
		}
		m_pCharObj->addEffect(Arrow);
		m_pCharObj->setState(L"AirBow");
		return true;
	}
	if (m_fTimer >= 0.1f && iJumpNum == 0 &&
		(S_Input.GetKey('A') == KEYSTATE::KEY_PUSH))
	{
		m_pCharObj->setJumpNum(1);
		m_pSprite->setIndex(0);
		m_fTimer = 0.0f;
		m_pCharObj->setState(L"Jump");
		return true;
	}
	m_CenterPos->y += g_fSecPerFrame * 400.0f;

	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(2);
	}

	if (m_pCharObj->getLanding())
	{
		m_pCharObj->setJumpNum(0);
		m_pSprite->setIndex(0);
		m_fTimer = 0.0f;
		m_pCharObj->setJumpSpeed(400.0f, 300.0f);
		m_pCharObj->setState(L"Rise");
		return true;
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerRise::PlayerRise(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Rise"), this);
}
bool PlayerRise::Init()
{
	setSprite(L"Kaho", L"Rise");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool PlayerRise::Frame()
{
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Idle");
		return true;
	}
	if (S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_LEFT) == KEYSTATE::KEY_HOLD)
	{
		if (m_pCharObj->getDir() == -1)		// 방향 같으면
		{
			m_pCharObj->setState(L"Run");
			return true;
		}
	}
	if (S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_RIGHT) == KEYSTATE::KEY_HOLD)
	{
		if (m_pCharObj->getDir() == 1)
		{
			m_pCharObj->setState(L"Run");
			return true;
		}
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerCrouch::PlayerCrouch(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Crouch"), this);
}
bool PlayerCrouch::Init()
{
	setSprite(L"Kaho", L"Crouch");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerCrouch::Frame()
{
	m_CenterPos->y += g_fSecPerFrame * 200.0f;
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(2);
		return true;
	}
	if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_UP || S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_FREE)
	{
		m_pCharObj->setState(L"Rise");
	}
	if (m_pCharObj->getDownable() && S_Input.GetKey('A') == KEYSTATE::KEY_PUSH)
	{
		m_CenterPos->y += 40.0f;
		m_pCharObj->setDownable(false);
		m_pCharObj->setState(L"Fall");
		m_pSprite->setIndex(0);
		return true;
	}
	if (S_Input.GetKey('D') == KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect::ARROW);
		EffectObj * Arrow = new KahoBowAttack;
		Arrow->Init();
		if (m_pCharObj->getDir() == -1)
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x - 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
			Arrow->setSpeed(-200.0f);
		}
		else
		{
			Arrow->Set(m_pCharObj->getCenterPos()->x + 20, m_pCharObj->getCenterPos()->y - 13);
			Arrow->setSpeed(200.0f);
		}
		m_pCharObj->addEffect(Arrow);
		m_pCharObj->setState(L"CrouchBow");
		return true;
	}

	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerLadderEnter::PlayerLadderEnter(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("LadderEnter"), this);
}
bool PlayerLadderEnter::Init()
{
	setSprite(L"Kaho", L"LadderEnter");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerLadderEnter::Frame()
{
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"LadderDown");
		return true;
	}
	/*if (m_pSprite->getIndex() == 0)
	{
		m_CenterPos->y += 100.0f * g_fSecPerFrame;
	}*/
	m_CenterPos->y += g_fSecPerFrame * 80.0f;
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerLadderLeave::PlayerLadderLeave(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("LadderLeave"), this);
}
bool PlayerLadderLeave::Init()
{
	setSprite(L"Kaho", L"LadderLeave");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerLadderLeave::Frame()
{
	if (!m_pSprite->Frame())
	{
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Rise");
		return true;
	}
	m_CenterPos->y -= g_fSecPerFrame * 65.0f;
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerLadderUp::PlayerLadderUp(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("LadderUp"), this);
}
bool PlayerLadderUp::Init()
{
	setSprite(L"Kaho", L"LadderUp");
	m_pSprite->setDivideTime(1.2f);
	return true;
}
bool PlayerLadderUp::Frame()
{
	FLOAT fSpeed = m_pCharObj->getSpeed();
	if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_HOLD)
	{
		m_pCharObj->setState(L"LadderDown");
		return true;
	}
	if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_UP) == KEYSTATE::KEY_HOLD)
	{
		m_CenterPos->y -= g_fSecPerFrame * fSpeed * 0.67;
		if (!m_pSprite->Frame())
		{
			m_pSprite->setIndex(0);
		}
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerLadderDown::PlayerLadderDown(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("LadderDown"), this);
}
bool PlayerLadderDown::Init()
{
	setSprite(L"Kaho", L"LadderDown");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerLadderDown::Frame()
{
	FLOAT fSpeed = m_pCharObj->getSpeed();
	if (S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_DOWN) == KEYSTATE::KEY_HOLD)
	{
		m_CenterPos->y += g_fSecPerFrame * fSpeed * 0.9;
		if (!m_pSprite->Frame())
		{
			m_pSprite->setIndex(0);
		}
	}
	if (S_Input.GetKey(VK_UP) == KEYSTATE::KEY_PUSH || S_Input.GetKey(VK_UP) == KEYSTATE::KEY_HOLD)
	{
		m_pCharObj->setState(L"LadderUp");
		return true;
	}
	//if (!m_pCharObj->isFallState())
	//{
	//	m_pSprite->setIndex(0);
	//	m_pCharObj->setState(L"LadderLeave");
	//	return true;
	//}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerRoll::PlayerRoll(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Roll"), this);
}
bool PlayerRoll::Init()
{
	setSprite(L"Kaho", L"Roll");
	m_pSprite->setDivideTime(0.6f);
	return true;
}
bool PlayerRoll::Frame()
{
	m_fTimer += g_fSecPerFrame;
	if (!m_pCharObj->getLanding() && m_fTimer>= 0.5f)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Fall");
		return true;
	}
	m_CenterPos->y += g_fSecPerFrame * 30.0f;
	if (!m_pSprite->Frame())
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Idle");
	}
	FLOAT fSpeed = m_pCharObj->getSpeed();
	m_CenterPos->x += m_pCharObj->getDir() * g_fSecPerFrame * fSpeed * 1.5f;
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerHurt::PlayerHurt(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Hurt"), this);
}
bool PlayerHurt::Init()
{
	setSprite(L"Kaho", L"Hurt");
	m_pSprite->setDivideTime(0.4f);
	return true;
}
bool PlayerHurt::Frame()
{
	m_pCharObj->setInvincible(true);
	m_CenterPos->y += g_fSecPerFrame * 30.0f;
	if (!m_pSprite->Frame())
	{
		if (m_pCharObj->getHP() <= 0)
		{
			m_pCharObj->setInvincible(false);
			m_pCharObj->setState(L"Death");
			return true;
		}
		m_pSprite->setIndex(0);
		m_pCharObj->setState(L"Fall");
		return true;
	}
	FLOAT fSpeed = m_pCharObj->getSpeed();
	m_CenterPos->x += m_pCharObj->getDir() * -1 * g_fSecPerFrame * fSpeed * 0.25f;
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}

PlayerDeath::PlayerDeath(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharObj->addState(std::string("Death"), this);
}
bool PlayerDeath::Init()
{
	setSprite(L"Kaho", L"Death");
	m_pSprite->setDivideTime(3.0f);
	return true;
}
bool PlayerDeath::Frame()
{
	if (!m_pSprite->Frame())
	{
		m_pCharObj->setDead(true);
		m_pSprite->setIndex(0);
	}
	*m_rtDraw = m_pSprite->getSpriteRt();
	return true;
}
