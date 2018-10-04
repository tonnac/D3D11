#include "PlayerBasicState.h"
#include "ObjectMgr.h"
#include "SceneMgr.h"
#include "Inventory.h"

EffectDel PlayerState::bowdel = [](PlayerEffect* pEffect)
{
	if (pEffect->getEnd() == false)
	{
		delete pEffect;
	}
};

PlayerState::PlayerState(Player * pPlayer) : m_pCharacter(pPlayer)
{
}


bool PlayerState::Frame()
{
	m_pCharacter->SetTexPos(m_pSprite->getSpriteVt());
	if (m_pSprite->Frame() == false)
	{
		m_pSprite->setIndex(m_iResetindex);
		return false;
	}
	return true;
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
		S_Sound.PlayEffect(Effect_Snd::ATTACK1);
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
		S_Sound.PlayEffect(Effect_Snd::ARROW);
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
		S_Sound.PlayEffect(Effect_Snd::ATTACK1);
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
		S_Sound.PlayEffect(Effect_Snd::JUMP);
		m_pSprite->setIndex(0);
		m_pCharacter->setLanding(false);
		m_pCharacter->setState(L"Jump");
		m_pCharacter->MoveCenterPos({ 0.0f, -g_fSecPerFrame * 900.0f });
		return true;
	}
	return false;
}
bool PlayerState::Roll()
{
	if (S_Input.getKeyState(DIK_Q) == Input::KEYSTATE::KEY_PUSH)
	{
		S_Sound.PlayEffect(Effect_Snd::ROLL);
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
	if (S_Input.getKeyState(DIK_W) == Input::KEYSTATE::KEY_PUSH)
	{
		QuickSlot Q = g_Inven->getQuickSlot();
		ItemPtr item = Q.getItem(m_pCharacter->getConsumableNum());
		if (item != nullptr && item->getCount() > 0)
		{
			m_pCharacter->setState(L"Item");
			S_Sound.PlayEffect(Effect_Snd::ITEM);
		}
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
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH && m_pCharacter->getLadder())
	{
		m_pCharacter->setState(L"LadderEnter");
		return true;
	}
	if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_PUSH && m_pCharacter->getLadder())
	{
		m_pCharacter->setState(L"LadderUp");
		return true;
	}
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pCharacter->setState(L"Crouch");
		return true;
	}

	return PlayerState::Frame();
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

	return PlayerState::Frame();
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
	if (PlayerState::Frame() == false)
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
	m_pSprite->setDivideTime(0.25f);
	return true;
}
bool PlayerTurn::Frame()
{
	if (Jump() == true)
	{
		return true;
	}
	if (PlayerState::Frame() == false)
	{
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

PlayerJump::PlayerJump(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(500.0f), m_fOffkeyTime(0.0f), m_fAcceleration(0.0f)
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
	m_fAcceleration += g_fSecPerFrame * 800.0f;
	m_fTimer += g_fSecPerFrame;
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_FREE || S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_UP)
	{
		m_fOffkeyTime += g_fSecPerFrame;
	}

	if (AirAttack() == true)
	{
		return true;
	}
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_HOLD && m_fTimer <= 0.3f)
	{
		m_pCharacter->MoveCenterPos({ 0.0f,  -g_fSecPerFrame * (m_fJumpSpeed - m_fAcceleration) });
	}
	else if (m_fOffkeyTime <= 0.2f && (m_fJumpSpeed - m_fJumpSpeed) < 0.0f)
	{
		m_pCharacter->MoveCenterPos({ 0.0f,  -g_fSecPerFrame * (m_fJumpSpeed - m_fAcceleration) });
	}
	else
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		m_fAcceleration = 0.0f;
		m_fOffkeyTime = 0.0f;
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
		m_fJumpSpeed = 0.0f;
		m_fOffkeyTime = 0.0f;
		S_Sound.PlayEffect(Effect_Snd::JUMP);
		m_fTimer = 0.0f;
		m_pSprite->setIndex(0);
		Player::setJumpNum(1);
		m_pCharacter->setState(L"Jump2");
		return true;
	}
	return PlayerState::Frame();
}

PlayerJump2::PlayerJump2(Player * pPlayer) : PlayerState(pPlayer), m_fJumpSpeed(500.0f)
{
	m_pCharacter->AddState(std::tstring(L"Jump2"), this);
}
bool PlayerJump2::Init()
{
	setSprite(L"Kaho", L"Jump");
	m_pSprite->setDivideTime(1.0f);
	m_iResetindex = 2;
	return true;
}
bool PlayerJump2::Frame()
{
	m_fTimer += g_fSecPerFrame;
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_HOLD && m_fTimer <= 0.20f)
	{
		m_pCharacter->MoveCenterPos({ 0.0f,  -g_fSecPerFrame * m_fJumpSpeed });
	}
	else
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(2);
		m_pCharacter->setState(L"Fall");
		return true;
	}
	if (AirAttack() == true)
	{
		return true;
	}
	if (BowAttack() == true)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(2);
		m_pCharacter->setState(L"AirBow");
		return true;
	}
	if (m_fTimer >= 0.15f)
	{
		m_fTimer = 0.0f;
		m_pSprite->setIndex(2);
		m_pCharacter->setState(L"Fall");
		return true;
	}
	AirMove();

	return PlayerState::Frame();
}

PlayerFall::PlayerFall(Player * pPlayer) : PlayerState(pPlayer), m_fAcceleration(0.0f), m_fAccelPlus(0.0f)
{
	m_pCharacter->AddState(std::tstring(L"Fall"), this);
}
bool PlayerFall::Init()
{
	setSprite(L"Kaho", L"Fall");
	m_pSprite->setDivideTime(0.4f);
	m_iResetindex = 2;
	return true;
}
bool PlayerFall::Frame()
{
	m_fTimer += g_fSecPerFrame;
	m_fAccelPlus += g_fSecPerFrame * 60.0f;
	m_fAcceleration += m_fAccelPlus;
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

	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * m_fAcceleration });

	if (m_pCharacter->isLanding() == true)
	{
		m_pSprite->setIndex(0);
		Player::setJumpNum(0);
		m_fAccelPlus = 0.0f;
		m_fAcceleration = 0.0f;
		m_pCharacter->setState(L"Rise");
		return true;
	}

	return PlayerState::Frame();
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
	if (PlayerState::Frame() == false)
	{
		m_pCharacter->setState(L"Idle");
	}
	if (Jump() == true)
	{
		return true;
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
	Player* pPlayer = dynamic_cast<Player*>(m_pCharacter);
	if (pPlayer->getDown() && S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
	{
		m_pCharacter->MoveCenterPos({ 0.0f,40.0f });
		pPlayer->setDown(false);
		m_pCharacter->setState(L"Fall");
		m_pSprite->setIndex(0);
		return true;
	}
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

	return PlayerState::Frame();
}

PlayerLadderEnter::PlayerLadderEnter(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"LadderEnter"), this);
}
bool PlayerLadderEnter::Init()
{
	setSprite(L"Kaho", L"LadderEnter");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerLadderEnter::Frame()
{
	if (PlayerState::Frame() == false)
	{
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"LadderDown");
		return true;
	}
	m_pCharacter->MoveCenterPos({ 0.0f,g_fSecPerFrame * 80.0f });
	return true;
}

PlayerLadderLeave::PlayerLadderLeave(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"LadderLeave"), this);
}
bool PlayerLadderLeave::Init()
{
	setSprite(L"Kaho", L"LadderLeave");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerLadderLeave::Frame()
{
	if (PlayerState::Frame() == false)
	{
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Rise");
		return true;
	}
	m_pCharacter->MoveCenterPos({ 0.0f,-g_fSecPerFrame * 65.0f });
	return true;
}

PlayerLadderUp::PlayerLadderUp(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"LadderUp"), this);
}
bool PlayerLadderUp::Init()
{
	setSprite(L"Kaho", L"LadderUp");
	m_pSprite->setDivideTime(1.2f);
	return true;
}
bool PlayerLadderUp::Frame()
{
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pCharacter->setState(L"LadderDown");
		return true;
	}
	if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pCharacter->MoveCenterPos({ 0.0f,-g_fSecPerFrame * g_fSpeed * 0.67f });
		if (PlayerState::Frame() == false)
		{
			m_pSprite->setIndex(0);
			return true;
		}
	}
	return true;
}

PlayerLadderDown::PlayerLadderDown(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"LadderDown"), this);
}
bool PlayerLadderDown::Init()
{
	setSprite(L"Kaho", L"LadderDown");
	m_pSprite->setDivideTime(0.8f);
	return true;
}
bool PlayerLadderDown::Frame()
{
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pCharacter->MoveCenterPos({ 0.0f, g_fSecPerFrame * g_fSpeed * 0.9f });
		if (PlayerState::Frame() == false)
		{
			m_pSprite->setIndex(0);
			return true;
		}
	}
	if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_PUSH || S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pCharacter->setState(L"LadderUp");
		return true;
	}
	//if (!m_pCharacter->isFallState())
	//{
	//	m_pSprite->setIndex(0);
	//	m_pCharacter->setState(L"LadderLeave");
	//	return true;
	//}
	PlayerState::Frame();
	return true;
}

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
	if (PlayerState::Frame() == false)
	{
		m_fTimer = 0.0f;
		m_pCharacter->setState(L"Idle");
	}
	return true;
}

PlayerHurt::PlayerHurt(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Hurt"), this);
}
bool PlayerHurt::Init()
{
	setSprite(L"Kaho", L"Hurt");
	m_pSprite->setDivideTime(0.4f);
	return true;
}
bool PlayerHurt::Frame()
{
	m_pCharacter->setInvincible(true);
	m_pCharacter->MoveCenterPos({ m_pCharacter->getDir() * -1 * g_fSecPerFrame * g_fSpeed * 0.25f, 0.0f });
	if (PlayerState::Frame() == false)
	{
		if (m_pCharacter->getHP() <= 0)
		{
			m_pCharacter->setInvincible(false);
			m_pCharacter->setState(L"Death");
			return true;
		}
		m_pSprite->setIndex(0);
		m_pCharacter->setState(L"Fall");
		return true;
	}
	return true;
}

PlayerDeath::PlayerDeath(Player * pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Death"), this);
}
bool PlayerDeath::Init()
{
	setSprite(L"Kaho", L"Death");
	m_pSprite->setDivideTime(3.0f);
	return true;
}
bool PlayerDeath::Frame()
{
	if (PlayerState::Frame() == false)
	{
		m_pCharacter->setDead(true);
	}
	return true;
}

PlayerItem::PlayerItem(Player* pPlayer) : PlayerState(pPlayer)
{
	m_pCharacter->AddState(std::tstring(L"Item"), this);
}
bool PlayerItem::Init()
{
	setSprite(L"Kaho", L"Item");
	m_pSprite->setDivideTime(1.0f);
	return true;
}
bool PlayerItem::Frame()
{
	if (PlayerState::Frame() == false)
	{
		QuickSlot Q = g_Inven->getQuickSlot();
		ItemPtr item = Q.getItem(m_pCharacter->getConsumableNum());
		m_pCharacter->setState(L"Idle");
		item->UseItem();
	}
	return true;
}