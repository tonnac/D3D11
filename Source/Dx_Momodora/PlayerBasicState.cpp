#include "PlayerBasicState.h"
#include "ObjectMgr.h"
#include "SceneMgr.h"

PlayerState::PlayerState(Player * pPlayer) : State(pPlayer)
{
	bowdel = [](PlayerEffect* pEffect)
	{
		if (pEffect->getEnd() == false)
		{
			delete pEffect;
		}
	};
}

void PlayerState::AttackFrame(PlayerEffectPtr aptr)
{
	D3DXVECTOR2 Center = m_pCharacter->getCenterPos();
	if (aptr == g_Attack1 || aptr == g_Attack3)
	{
		if (m_pCharacter->getDir() == 1)
		{
			Center.x += 100.0f;
			aptr->setDir(1);
		}
		else
		{
			Center.x -= 100.0f;
			aptr->setDir(-1);
		}
	}
	else if (aptr == g_Attack2)
	{
		if (m_pCharacter->getDir() == 1)
		{
			Center.x += 70.0f;
			aptr->setDir(1);
		}
		else
		{
			Center.x -= 70.0f;
			aptr->setDir(-1);
		}
	}
	else
	{
		if (m_pCharacter->getDir() == 1)
		{
			Center.x += 60.0f;
			Center.y += 10.0f;
			aptr->setDir(1);
		}
		else
		{
			Center.x -= 60.0f;
			Center.y += 10.0f;
			aptr->setDir(-1);
		}
	}
	aptr->SetCenterPos(Center);
}

bool PlayerState::AirAttack()
{
	if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		S_Sound.Play(Effect_Snd::ATTACK1);
		m_pCharacter->setState(L"AirAttack");
		D3DXVECTOR2 Center = m_pCharacter->getCenterPos();
		if (m_pCharacter->getDir() == 1)
		{
			Center.x += 60.0f;
			Center.y += 20.0f;
			g_Attack1->setDir(1);
		}
		else
		{
			Center.x -= 60.0f;
			Center.y += 20.0f;
			g_Attack1->setDir(-1);
		}
		g_AirAttack->SetCenterPos(Center);
		S_Object.AddPlayerEffect(g_AirAttack);
		return true;
	}
	return false;
}
bool PlayerState::BowAttack()
{
	if (S_Input.getKeyState(DIK_D) == Input::KEYSTATE::KEY_PUSH)
	{
		std::shared_ptr<KahoBowAttack> Arrow(new KahoBowAttack, bowdel);
		D3DXVECTOR2 Cen = m_pCharacter->getCenterPos();

		if (m_pCharacter->getDir() == -1)
		{
			Arrow->SetCenterPos({ Cen.x - 20, Cen.y - 13 });
			Arrow->reverseDir();
		}
		else
		{
			Arrow->SetCenterPos({ Cen.x + 20, Cen.y - 13 });
		}
		S_Sound.Play(Effect_Snd::ARROW);
		S_Scene.InitArrow(Arrow);
		S_Object.AddPlayerEffect(Arrow);
		return true;
	}
	return false;
}
bool PlayerState::Attack()
{
	if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect_Snd::ATTACK1);
		m_pCharacter->setState(L"Attack1");
		AttackFrame(g_Attack1);
		S_Object.AddPlayerEffect(g_Attack1);
		return true;
	}
	return false;
}
bool PlayerState::Jump()
{
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect_Snd::JUMP);
		m_pSprite->setIndex(0);
		m_pCharacter->setLanding(false);
		m_pCharacter->setState(L"Jump");
		return true;
	}
	return false;
}
bool PlayerState::Roll()
{
	if (S_Input.getKeyState(DIK_Q) == Input::KEYSTATE::KEY_PUSH)
	{
		S_Sound.Play(Effect_Snd::ROLL);
		m_pCharacter->setState(L"Roll");
		return true;
	}
	return false;
}
bool PlayerState::Fall()
{
	if (m_pCharacter->isLanding() == false)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Fall");
		return true;
	}
	return false;
}
void PlayerState::AirMove()
{
	INT DIR = m_pCharacter->getDir();
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		if (DIR == -1)
		{
			m_pCharacter->reverseDir();
		}
		else
			m_pCharacter->MoveCenterPos({ DIR * g_fSecPerFrame * g_fSpeed, 0.0f });
	}
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)
	{
		if (DIR == 1)
		{
			m_pCharacter->reverseDir();
		}
		else
			m_pCharacter->MoveCenterPos({ DIR * g_fSecPerFrame * g_fSpeed, 0.0f });
	}
}

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
	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * 80.0f });

	if (BowAttack() == true)
	{
		m_pCharacter->setState(L"Bow");
		return true;
	}
	if (Attack() == true)
	{
		return true;
	}
	if (Jump() == true)
	{
		return true;
	}
	if (Roll() == true)
	{
		return true;
	}
	if (Fall() == true)
	{
		return true;
	}

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
	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * 30.0f });

	if (Attack() == true)
	{
		return true;
	}
	if (Jump() == true)
	{
		return true;
	}
	if (Roll() == true)
	{
		return true;
	}
	if (Fall() == true)
	{
		return true;
	}
	if (BowAttack() == true)
	{
		m_pCharacter->setState(L"Bow");
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
		m_pCharacter->MoveCenterPos({ Dir * g_fSecPerFrame * g_fSpeed, 0.0f });
	}
	else if ((S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_UP || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_FREE) ||
		(S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_UP || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_FREE))
	{
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Brake");
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
	if (Attack() == true)
	{
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

PlayerJump::PlayerJump(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(400.0f)
{
	m_pCharacter->AddState(std::tstring(L"Jump"), this);
}
bool PlayerJump::Init()
{
	setSprite(L"Kaho", L"Jump");
	m_pSprite->setDivideTime(0.5f);
	m_iResetindex = 2;
	return true;
}
bool PlayerJump::Frame()
{
	m_fTimer += g_fSecPerFrame;
	m_pCharacter->MoveCenterPos({ 0.0f,  -g_fSecPerFrame * m_fJumpSpeed });

	if (AirAttack() == true)
	{
		return true;
	}
	if (m_fTimer >= 0.3f)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Fall");
		return true;
	}
	if (BowAttack() == true)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"AirBow");
		return true;
	}
	AirMove();

	if (m_fTimer >= 0.1f && Player::getJumpNum() == 0 && (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH))
	{
		S_Sound.Play(Effect_Snd::JUMP);
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		Player::setJumpNum(1);
		m_pCharacter->setState(L"Jump2");
		return true;
	}
	return State::Frame();
}

PlayerJump2::PlayerJump2(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(600.0f)
{
	m_pCharacter->AddState(std::tstring(L"Jump2"), this);
}
bool PlayerJump2::Init()
{
	setSprite(L"Kaho", L"Jump");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool PlayerJump2::Frame()
{
	m_fTimer += g_fSecPerFrame;

	m_pCharacter->MoveCenterPos({ 0.0f,  -g_fSecPerFrame * m_fJumpSpeed });

	if (AirAttack() == true)
	{
		return true;
	}
	if (BowAttack() == true)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"AirBow");
		return true;
	}
	if (m_fTimer >= 0.15f)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Fall");
		return true;
	}
	AirMove();

	return State::Frame();
}

PlayerFall::PlayerFall(Player * pPlayer) : PlayerState(pPlayer), m_fAcceleration(1.0f)
{
	m_pCharacter->AddState(std::tstring(L"Fall"), this);
}
bool PlayerFall::Init()
{
	setSprite(L"Kaho", L"Fall");
	m_pSprite->setDivideTime(1.0f);
	m_iResetindex = 2;
	return true;
}
bool PlayerFall::Frame()
{
	m_fTimer += g_fSecPerFrame;

	if (AirAttack() == true)
	{
		return true;
	}
	AirMove();
	if (BowAttack() == true)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"AirBow");
		return true;
	}

	if (m_fTimer >= 0.1f && Player::getJumpNum() == 0 &&
		(S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH))
	{
		Player::setJumpNum(1);
		m_pSprite->setIndex(0);
		m_fTimer = 0.0f;
		m_pCharacter->setState(L"Jump2");
		return true;
	}

	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * 400.0f });

	if (m_pCharacter->isLanding() == true)
	{
		m_pSprite->setIndex(0);
		Player::setJumpNum(0);
		m_pCharacter->setState(L"Rise");
		return true;
	}

	return State::Frame();
}

PlayerRise::PlayerRise(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Rise"), this);
}
bool PlayerRise::Init()
{
	setSprite(L"Kaho", L"Rise");
	m_pSprite->setDivideTime(0.3f);
	return true;
}
bool PlayerRise::Frame()
{
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)
	{
		if (m_pCharacter->getDir() == -1)		// 방향 같으면
		{
			m_pCharacter->setState(L"Run");
			return true;
		}
	}
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		if (m_pCharacter->getDir() == 1)
		{
			m_pCharacter->setState(L"Run");
			return true;
		}
	}
	if (State::Frame() == false)
	{
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

PlayerCrouch::PlayerCrouch(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Crouch"), this);
}
bool PlayerCrouch::Init()
{
	setSprite(L"Kaho", L"Crouch");
	m_pSprite->setDivideTime(0.8f);
	m_iResetindex = 2;
	return true;
}
bool PlayerCrouch::Frame()
{
	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * 200.0f });
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_UP || S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_FREE)
	{
		m_pCharacter->setState(L"Rise");
	}
	/*if (m_pCharacter->getDownable() && S_Input.getKeyState('A') == Input::KEYSTATE::KEY_PUSH)
	{
		m_CenterPos->y += 40.0f;
		m_pCharacter->setDownable(false);
		m_pCharacter->setState(L"Fall");
		m_pSprite->setIndex(0);
		return true;
	}*/
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH)
	{
		if (m_pCharacter->getDir() == 1)						// 방향 다르면
		{
			m_fTimer = 0.0f;
			m_pCharacter->reverseDir();
		}
	}
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH)
	{
		if (m_pCharacter->getDir() == -1)
		{
			m_fTimer = 0.0f;
			m_pCharacter->reverseDir();
		}
	}
	if (BowAttack() == true)
	{
		m_pCharacter->setState(L"CrouchBow");
		return true;
	}

	return State::Frame();
}
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
PlayerRoll::PlayerRoll(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Roll"), this);
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
	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * 30.0f });
	if (m_pCharacter->isLanding() == false && m_fTimer >= 0.5f)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Fall");
		return true;
	}
	m_pCharacter->MoveCenterPos({ m_pCharacter->getDir() * g_fSecPerFrame * g_fSpeed * 1.5f , 0.0f });
	if (State::Frame() == false)
	{
		m_fTimer = 0.0f;
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

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
