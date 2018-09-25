#include "PlayerBasicState.h"
//#include "KahoAttack.h"
//#include "TerrainObject.h"
//#include "mSound.h"

PlayerState::PlayerState(Player * pPlayer) : State(pPlayer)
{}
//bool PlayerState::Render()
//{
//	return true;
//}
//bool PlayerState::Release()
//{
//	if (m_pEffectObj)
//	{
//		m_pEffectObj->Release();
//		delete m_pEffectObj;
//	}
//	return true;
//}

PlayerIdle::PlayerIdle(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Idle"), this);
}
bool PlayerIdle::Init()
{
	setSprite(L"Kaho", L"Idle");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerIdle::Frame()
{
//	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * 30.0f });
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
	{
//		S_Sound.Play(Effect::JUMP);
		m_pCharacter->setState(L"Jump");
		return true;
	}
	if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
	{
//		S_Sound.Play(Effect::ATTACK1);
		m_pCharacter->setState(L"Attack1");
		return true;
	}
	if (S_Input.getKeyState(DIK_Q) == Input::KEYSTATE::KEY_PUSH)
	{
//		S_Sound.Play(Effect::ROLL);
		m_pCharacter->setState(L"Roll");
		return true;
	}
	//if (S_Input.getKeyState(DIK_D) == Input::KEYSTATE::KEY_PUSH)
	//{
	//	EffectObj * Arrow = new KahoBowAttack;
	//	Arrow->Init();
	//	if (m_pCharacter->getDir() == -1)
	//	{
	//		Arrow->Set(m_pCharacter->getCenterPos()->x - 20, m_pCharacter->getCenterPos()->y - 13);
	//		Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
	//		Arrow->setSpeed(-200.0f);
	//	}
	//	else
	//	{
	//		Arrow->Set(m_pCharacter->getCenterPos()->x + 20, m_pCharacter->getCenterPos()->y - 13);
	//		Arrow->setSpeed(200.0f);
	//	}
	//	m_pCharacter->addEffect(Arrow);
	//	S_Sound.Play(Effect::ARROW);
	//	m_pCharacter->setState(L"Bow");
	//	return true;
	//}
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)
	{
		m_fTimer += g_fSecPerFrame;
		if (m_pCharacter->getDir() == -1 && m_fTimer >= 0.1f)		// 방향 같으면
		{
			m_fTimer = 0.0f;
			m_pCharacter->setState(L"Run");
			return true;
		}
		else if (m_pCharacter->getDir() == 1)						// 방향 다르면
		{
			m_fTimer = 0.0f;
			m_pCharacter->reverseDir();
			return true;
		}
	}
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		m_fTimer += g_fSecPerFrame;
		if (m_pCharacter->getDir() == 1 && m_fTimer >= 0.1f)
		{
			m_fTimer = 0.0f;
			m_pCharacter->setState(L"Run");
			return true;
		}
		else if (m_pCharacter->getDir() == -1)
		{
			m_fTimer = 0.0f;
			m_pCharacter->reverseDir();
			return true;
		}
	}
	//if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH && m_pCharacter->getLadder())
	//{
	//	m_pCharacter->setState(L"LadderEnter");
	//	return true;
	//}
	//if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_PUSH && m_pCharacter->getLadder())
	//{
	//	m_pCharacter->setState(L"LadderUp");
	//	return true;
	//}
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pCharacter->setState(L"Crouch");
		return true;
	}

	return State::Frame();
}

PlayerRun::PlayerRun(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Run"), this);
}
bool PlayerRun::Init()
{
	setSprite(L"Kaho", L"Run");
	m_pSprite->setDivideTime(1.0f);
	m_iResetindex = 2;
	return true;
}
bool PlayerRun::Frame()
{
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	m_CenterPos->y += g_fSecPerFrame * 2.0f;
	//if (!m_pCharacter->getLanding())
	//{
	//	m_pSprite->setIndex(0);
	//	m_pCharacter->setState(L"Fall");
	//	return true;
	//}

	if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
	{
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Attack1");
		return true;
	}
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_HOLD)
	{
//		S_Sound.Play(Effect::JUMP);
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Jump");
		return true;
	}
	if ((m_pCharacter->getDir() == 1 && (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD))
		|| (m_pCharacter->getDir() == -1 && (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)))
	{
		m_pSprite->setIndex(0);
		m_pCharacter->reverseDir();
		m_pCharacter->setState(L"Turn");
		return true;
	}
	else if ((m_pCharacter->getDir() == 1 && (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD))
		|| (m_pCharacter->getDir() == -1 && (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)))
	{
		INT Dir = m_pCharacter->getDir();
		m_pCharacter->MoveCenterPos({ Dir * g_fSecPerFrame * 50.0f, 0.0f });
	}
	else if ((S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_UP || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_FREE) ||
		(S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_UP || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_FREE))
	{
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Brake");
		return true;
	}
	if (S_Input.getKeyState(DIK_Q) == Input::KEYSTATE::KEY_PUSH)
	{
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Roll");
		return true;
	}

	return State::Frame();
}

PlayerBrake::PlayerBrake(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Brake"), this);
}
bool PlayerBrake::Init()
{
	setSprite(L"Kaho", L"Brake");
	m_pSprite->setDivideTime(0.35f);
	return true;
}
bool PlayerBrake::Frame()
{
	if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
	{
		m_pCharacter->setState(L"Attack1");
		return true;
	}
	if ((m_pCharacter->getDir() == 1 && (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)) ||
		(m_pCharacter->getDir() == -1 && (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)))
	{
		m_pCharacter->setState(L"Turn");
		m_pCharacter->reverseDir();
		m_pSprite->setIndex(0);
		return true;
	}
	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

PlayerTurn::PlayerTurn(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Turn"), this);
}
bool PlayerTurn::Init()
{
	setSprite(L"Kaho", L"Turn");
	m_pSprite->setDivideTime(0.35f);
	return true;
}
bool PlayerTurn::Frame()
{
	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

//PlayerJump::PlayerJump(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(m_pCharacter->getJumpSpeed()), m_fAcceleration(-120.0f)
//{
//	m_pCharacter->AddState(std::tstring("Jump"), this);
//}
//bool PlayerJump::Init()
//{
//	setSprite(L"Kaho", L"Jump");
//	m_pSprite->setDivideTime(0.5f);
//	return true;
//}
//bool PlayerJump::Frame()
//{
//	m_pCharacter->setLanding(false);
//	m_fTimer += g_fSecPerFrame;
//	m_fJumpSpeed += g_fSecPerFrame * 9.8 * m_fAcceleration / 2;
//	INT iJumpNumber = m_pCharacter->getJumpNum();
//	m_CenterPos->y -= g_fSecPerFrame * m_fJumpSpeed;
//	if (S_Input.getKeyState('S') == Input::KEYSTATE::KEY_PUSH)
//	{
//		S_Sound.Play(Effect::AIRATTACK);
//		m_fTimer = 0.0f;
//		m_fAcceleration = -120.0f;
//		m_pCharacter->setState(L"AirAttack");
//		return true;
//	}
//	if (m_fJumpSpeed < 0.0f)
//	{
//		m_fTimer = 0.0f;
//		m_fAcceleration = -120.0f;
//		m_pCharacter->setState(L"Fall");
//		return true;
//	}
//	if (iJumpNumber == 0 && (S_Input.getKeyState('A') == Input::KEYSTATE::KEY_PUSH))
//	{
//		S_Sound.Play(Effect::JUMP);
//		m_fTimer = 0.0f;
//		m_fAcceleration = -120.0f;
//		m_pSprite->setIndex(0);
//		m_pCharacter->setJumpNum(1);
//		m_pCharacter->setState(L"Jump2");
//		return true;
//	}
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	if (S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		if (m_pCharacter->getDir() == -1)
//		{
//			m_pCharacter->setRendering(m_pCharacter->getZoom(), INVERSE::DEFAULT);
//			m_pCharacter->setDir(-1);
//		}
//		else
//			m_CenterPos->x += g_fSecPerFrame * fSpeed;
//	}
//	if (S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		if (m_pCharacter->getDir() == 1)
//		{
//			m_pCharacter->setRendering(m_pCharacter->getZoom(), INVERSE::LR_ROTATION);
//			m_pCharacter->setDir(-1);
//		}
//		else
//			m_CenterPos->x -= g_fSecPerFrame * fSpeed;
//	}
//	if (S_Input.getKeyState('D') == Input::KEYSTATE::KEY_PUSH)
//	{
//		S_Sound.Play(Effect::ARROW);
//		EffectObj * Arrow = new KahoBowAttack;
//		Arrow->Init();
//		if (m_pCharacter->getDir() == -1)
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x - 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
//			Arrow->setSpeed(-200.0f);
//		}
//		else
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x + 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setSpeed(200.0f);
//		}
//		m_pCharacter->addEffect(Arrow);
//		m_pCharacter->setState(L"AirBow");
//		return true;
//	}
//	if (!m_pSprite->Frame())
//	{
//		m_pSprite->setIndex(0);
//	}
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerJump2::PlayerJump2(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(m_pCharacter->getJumpSpeed(1)), m_fAcceleration(-120.0f)
//{
//	m_pCharacter->AddState(std::tstring("Jump2"), this);
//}
//bool PlayerJump2::Init()
//{
//	setSprite(L"Kaho", L"Jump");
//	m_pSprite->setDivideTime(1.0f);
//	return true;
//}
//bool PlayerJump2::Frame()
//{
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	m_fTimer += g_fSecPerFrame;
//	m_fJumpSpeed += g_fSecPerFrame * 9.8 * m_fAcceleration / 2;
//	INT iJumpNumber = m_pCharacter->getJumpNum();
//	m_CenterPos->y -= g_fSecPerFrame * m_fJumpSpeed;
//
//	if (S_Input.getKeyState('S') == Input::KEYSTATE::KEY_PUSH)
//	{
//		S_Sound.Play(Effect::AIRATTACK);
//		m_fTimer = 0.0f;
//		m_fAcceleration = -120.0f;
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"AirAttack");
//		return true;
//	}
//	if (m_fJumpSpeed < 0.0f)
//	{
//		m_fTimer = 0.0f;
//		m_fAcceleration = -120.0f;
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"Fall");
//		return true;
//	}
//	if (S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_CenterPos->x += g_fSecPerFrame * fSpeed;
//	}
//	if (S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_CenterPos->x -= g_fSecPerFrame * fSpeed;
//	}
//	if (S_Input.getKeyState('D') == Input::KEYSTATE::KEY_PUSH)
//	{
//		EffectObj * Arrow = new KahoBowAttack;
//		Arrow->Init();
//		if (m_pCharacter->getDir() == -1)
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x - 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
//			Arrow->setSpeed(-200.0f);
//		}
//		else
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x + 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setSpeed(200.0f);
//		}
//		m_pCharacter->addEffect(Arrow);
//		S_Sound.Play(Effect::ARROW);
//		m_pCharacter->setState(L"AirBow");
//		return true;
//	}
//	if (!m_pSprite->Frame())
//	{
//		m_pSprite->setIndex(0);
//	}
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerFall::PlayerFall(Player * pPlayer) : PlayerState(pPlayer), m_fAcceleration(1.0f)
//{
//	m_pCharacter->AddState(std::tstring("Fall"), this);
//}
//bool PlayerFall::Init()
//{
//	setSprite(L"Kaho", L"Fall");
//	m_pSprite->setDivideTime(0.5f);
//	return true;
//}
//bool PlayerFall::Frame()
//{
//	m_fTimer += g_fSecPerFrame;
//	INT iJumpNum = m_pCharacter->getJumpNum();
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	if (S_Input.getKeyState('S') == Input::KEYSTATE::KEY_PUSH)
//	{
//		m_pCharacter->setState(L"AirAttack");
//		return true;
//	}
//	if (S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_CenterPos->x += g_fSecPerFrame * fSpeed;
//	}
//	if (S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_CenterPos->x -= g_fSecPerFrame * fSpeed;
//	}
//	if (S_Input.getKeyState('D') == Input::KEYSTATE::KEY_PUSH)
//	{
//		EffectObj * Arrow = new KahoBowAttack;
//		Arrow->Init();
//		if (m_pCharacter->getDir() == -1)
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x - 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
//			Arrow->setSpeed(-200.0f);
//		}
//		else
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x + 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setSpeed(200.0f);
//		}
//		m_pCharacter->addEffect(Arrow);
//		m_pCharacter->setState(L"AirBow");
//		return true;
//	}
//	if (m_fTimer >= 0.1f && iJumpNum == 0 &&
//		(S_Input.getKeyState('A') == Input::KEYSTATE::KEY_PUSH))
//	{
//		m_pCharacter->setJumpNum(1);
//		m_pSprite->setIndex(0);
//		m_fTimer = 0.0f;
//		m_pCharacter->setState(L"Jump");
//		return true;
//	}
//	m_CenterPos->y += g_fSecPerFrame * 400.0f;
//
//	if (!m_pSprite->Frame())
//	{
//		m_pSprite->setIndex(2);
//	}
//
//	if (m_pCharacter->getLanding())
//	{
//		m_pCharacter->setJumpNum(0);
//		m_pSprite->setIndex(0);
//		m_fTimer = 0.0f;
//		m_pCharacter->setJumpSpeed(400.0f, 300.0f);
//		m_pCharacter->setState(L"Rise");
//		return true;
//	}
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerRise::PlayerRise(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("Rise"), this);
//}
//bool PlayerRise::Init()
//{
//	setSprite(L"Kaho", L"Rise");
//	m_pSprite->setDivideTime(0.3f);
//	return true;
//}
//bool PlayerRise::Frame()
//{
//	if (!m_pSprite->Frame())
//	{
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"Idle");
//		return true;
//	}
//	if (S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_LEFT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		if (m_pCharacter->getDir() == -1)		// 방향 같으면
//		{
//			m_pCharacter->setState(L"Run");
//			return true;
//		}
//	}
//	if (S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
//	{
//		if (m_pCharacter->getDir() == 1)
//		{
//			m_pCharacter->setState(L"Run");
//			return true;
//		}
//	}
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerCrouch::PlayerCrouch(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("Crouch"), this);
//}
//bool PlayerCrouch::Init()
//{
//	setSprite(L"Kaho", L"Crouch");
//	m_pSprite->setDivideTime(0.8f);
//	return true;
//}
//bool PlayerCrouch::Frame()
//{
//	m_CenterPos->y += g_fSecPerFrame * 200.0f;
//	if (!m_pSprite->Frame())
//	{
//		m_pSprite->setIndex(2);
//		return true;
//	}
//	if (S_Input.getKeyState(VK_DOWN) == Input::KEYSTATE::KEY_UP || S_Input.getKeyState(VK_DOWN) == Input::KEYSTATE::KEY_FREE)
//	{
//		m_pCharacter->setState(L"Rise");
//	}
//	if (m_pCharacter->getDownable() && S_Input.getKeyState('A') == Input::KEYSTATE::KEY_PUSH)
//	{
//		m_CenterPos->y += 40.0f;
//		m_pCharacter->setDownable(false);
//		m_pCharacter->setState(L"Fall");
//		m_pSprite->setIndex(0);
//		return true;
//	}
//	if (S_Input.getKeyState('D') == Input::KEYSTATE::KEY_PUSH)
//	{
//		S_Sound.Play(Effect::ARROW);
//		EffectObj * Arrow = new KahoBowAttack;
//		Arrow->Init();
//		if (m_pCharacter->getDir() == -1)
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x - 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setRendering(2.8f, INVERSE::LR_ROTATION);
//			Arrow->setSpeed(-200.0f);
//		}
//		else
//		{
//			Arrow->Set(m_pCharacter->getCenterPos()->x + 20, m_pCharacter->getCenterPos()->y - 13);
//			Arrow->setSpeed(200.0f);
//		}
//		m_pCharacter->addEffect(Arrow);
//		m_pCharacter->setState(L"CrouchBow");
//		return true;
//	}
//
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerLadderEnter::PlayerLadderEnter(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("LadderEnter"), this);
//}
//bool PlayerLadderEnter::Init()
//{
//	setSprite(L"Kaho", L"LadderEnter");
//	m_pSprite->setDivideTime(0.8f);
//	return true;
//}
//bool PlayerLadderEnter::Frame()
//{
//	if (!m_pSprite->Frame())
//	{
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"LadderDown");
//		return true;
//	}
//	/*if (m_pSprite->getIndex() == 0)
//	{
//		m_CenterPos->y += 100.0f * g_fSecPerFrame;
//	}*/
//	m_CenterPos->y += g_fSecPerFrame * 80.0f;
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerLadderLeave::PlayerLadderLeave(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("LadderLeave"), this);
//}
//bool PlayerLadderLeave::Init()
//{
//	setSprite(L"Kaho", L"LadderLeave");
//	m_pSprite->setDivideTime(0.8f);
//	return true;
//}
//bool PlayerLadderLeave::Frame()
//{
//	if (!m_pSprite->Frame())
//	{
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"Rise");
//		return true;
//	}
//	m_CenterPos->y -= g_fSecPerFrame * 65.0f;
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerLadderUp::PlayerLadderUp(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("LadderUp"), this);
//}
//bool PlayerLadderUp::Init()
//{
//	setSprite(L"Kaho", L"LadderUp");
//	m_pSprite->setDivideTime(1.2f);
//	return true;
//}
//bool PlayerLadderUp::Frame()
//{
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	if (S_Input.getKeyState(VK_DOWN) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_DOWN) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_pCharacter->setState(L"LadderDown");
//		return true;
//	}
//	if (S_Input.getKeyState(VK_UP) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_UP) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_CenterPos->y -= g_fSecPerFrame * fSpeed * 0.67;
//		if (!m_pSprite->Frame())
//		{
//			m_pSprite->setIndex(0);
//		}
//	}
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerLadderDown::PlayerLadderDown(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("LadderDown"), this);
//}
//bool PlayerLadderDown::Init()
//{
//	setSprite(L"Kaho", L"LadderDown");
//	m_pSprite->setDivideTime(0.8f);
//	return true;
//}
//bool PlayerLadderDown::Frame()
//{
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	if (S_Input.getKeyState(VK_DOWN) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_DOWN) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_CenterPos->y += g_fSecPerFrame * fSpeed * 0.9;
//		if (!m_pSprite->Frame())
//		{
//			m_pSprite->setIndex(0);
//		}
//	}
//	if (S_Input.getKeyState(VK_UP) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(VK_UP) == Input::KEYSTATE::KEY_HOLD)
//	{
//		m_pCharacter->setState(L"LadderUp");
//		return true;
//	}
//	//if (!m_pCharacter->isFallState())
//	//{
//	//	m_pSprite->setIndex(0);
//	//	m_pCharacter->setState(L"LadderLeave");
//	//	return true;
//	//}
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerRoll::PlayerRoll(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("Roll"), this);
//}
//bool PlayerRoll::Init()
//{
//	setSprite(L"Kaho", L"Roll");
//	m_pSprite->setDivideTime(0.6f);
//	return true;
//}
//bool PlayerRoll::Frame()
//{
//	m_fTimer += g_fSecPerFrame;
//	if (!m_pCharacter->getLanding() && m_fTimer>= 0.5f)
//	{
//		m_fTimer = 0.0f;
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"Fall");
//		return true;
//	}
//	m_CenterPos->y += g_fSecPerFrame * 30.0f;
//	if (!m_pSprite->Frame())
//	{
//		m_fTimer = 0.0f;
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"Idle");
//	}
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	m_CenterPos->x += m_pCharacter->getDir() * g_fSecPerFrame * fSpeed * 1.5f;
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerHurt::PlayerHurt(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("Hurt"), this);
//}
//bool PlayerHurt::Init()
//{
//	setSprite(L"Kaho", L"Hurt");
//	m_pSprite->setDivideTime(0.4f);
//	return true;
//}
//bool PlayerHurt::Frame()
//{
//	m_pCharacter->setInvincible(true);
//	m_CenterPos->y += g_fSecPerFrame * 30.0f;
//	if (!m_pSprite->Frame())
//	{
//		if (m_pCharacter->getHP() <= 0)
//		{
//			m_pCharacter->setInvincible(false);
//			m_pCharacter->setState(L"Death");
//			return true;
//		}
//		m_pSprite->setIndex(0);
//		m_pCharacter->setState(L"Fall");
//		return true;
//	}
//	FLOAT fSpeed = m_pCharacter->getSpeed();
//	m_CenterPos->x += m_pCharacter->getDir() * -1 * g_fSecPerFrame * fSpeed * 0.25f;
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
//
//PlayerDeath::PlayerDeath(Player * pPlayer) : PlayerState(pPlayer)
//{
//	m_pCharacter->AddState(std::tstring("Death"), this);
//}
//bool PlayerDeath::Init()
//{
//	setSprite(L"Kaho", L"Death");
//	m_pSprite->setDivideTime(3.0f);
//	return true;
//}
//bool PlayerDeath::Frame()
//{
//	if (!m_pSprite->Frame())
//	{
//		m_pCharacter->setDead(true);
//		m_pSprite->setIndex(0);
//	}
//	*m_rtDraw = m_pSprite->getSpriteRt();
//	return true;
//}
