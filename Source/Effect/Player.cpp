#include "Player.h"
#include "PlayerBasicState.h"

FLOAT g_fSpeed = 0.0f;

Player::Player()
{
	State * state = new PlayerIdle(this);
	state = new PlayerRun(this);
	state = new PlayerBrake(this);
	state = new PlayerTurn(this);
	//state = new PlayerAttack(this);
	//state = new PlayerAttack2(this);
	//state = new PlayerAttack3(this);
	//state = new PlayerJump(this);
	//state = new PlayerJump2(this);
	//state = new PlayerFall(this);
	//state = new AirAttack(this);
	//state = new PlayerRise(this);
	//state = new PlayerCrouch(this);
	//state = new PlayerRoll(this);
	//state = new BowAttack(this);
	//state = new AirBowAttack(this);
	//state = new CrouchBowAttack(this);
	//state = new PlayerHurt(this);
	//state = new PlayerDeath(this);

	//state = new PlayerLadderEnter(this);
	//state = new PlayerLadderLeave(this);
	//state = new PlayerLadderUp(this);
	//state = new PlayerLadderDown(this);
	m_fSpeed = g_fSpeed = 200.0f;
	m_pCurrentState = m_StateList[L"Idle"];
}