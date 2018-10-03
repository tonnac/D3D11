#include "Player.h"
#include "PlayerBasicState.h"
#include "PlayerAttack.h"
#include "DirectInput.h"

FLOAT g_fSpeed = 0.0f;
INT Player::m_iJumpNum = 0;

Player::Player() : m_bInvincible(false), m_fTimer(0.0f), m_Down(false), m_Ladder(false), m_iConsumableNumber(0), m_iEquipNumber(0)
{
	State * state = new PlayerIdle(this);
	state = new PlayerRun(this);
	state = new PlayerBrake(this);
	state = new PlayerTurn(this);
	state = new PlayerAttack(this);
	state = new PlayerAttack2(this);
	state = new PlayerAttack3(this);
	state = new PlayerJump(this);
	state = new PlayerJump2(this);
	state = new PlayerFall(this);
	state = new AirAttack(this);
	state = new PlayerRise(this);
	state = new PlayerCrouch(this);
	state = new PlayerRoll(this);
	state = new BowAttack(this);
	state = new AirBowAttack(this);
	state = new CrouchBowAttack(this);
	//state = new PlayerHurt(this);
	//state = new PlayerDeath(this);

	state = new PlayerLadderEnter(this);
	state = new PlayerLadderLeave(this);
	state = new PlayerLadderUp(this);
	state = new PlayerLadderDown(this);
	m_fSpeed = g_fSpeed = 300.0f;
	m_pCurrentState = m_StateList[L"Idle"];
}

bool Player::Frame()
{
	if (m_bInvincible == true)
	{
		m_fTimer += g_fSecPerFrame;
		if (m_ConstantData.Util.z >= 0.5f)
		{
			m_ConstantData.Util.z = 0.0f;
		}
		else
			m_ConstantData.Util.z = 1.0f;
		if (m_fTimer >= 5.0f)
		{
			m_fTimer = 0.0f;
			m_ConstantData.Util.z = 1.0f;
			m_bInvincible = false;
		}
	}

	if (S_Input.getKeyState(DIK_F8) == Input::KEYSTATE::KEY_PUSH)
	{
		m_bInvincible = true;
	}
	return Character::Frame();
}

void Player::setJumpNum(const INT& iNum)
{
	m_iJumpNum = iNum;
}
void Player::setDown(const bool& down)
{
	m_Down = down;
}
void Player::setLadder(const bool& ladder)
{
	m_Ladder = ladder;
}

INT Player::getJumpNum()
{
	return m_iJumpNum;
}
bool Player::getDown() const
{
	return m_Down;
}
bool Player::getLadder() const
{
	return m_Ladder;
}
INT Player::getConsumableNum() const
{
	return m_iConsumableNumber;
}
INT Player::getEquipNum() const
{
	return m_iEquipNumber;
}