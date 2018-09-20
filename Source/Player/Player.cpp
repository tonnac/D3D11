#include "Player.h"
#include "PlayerAttack.h"
#include "PlayerBasicState.h"

float	g_fSpeed = 0.0f;
INT		g_HP = 0;

Player::Player()
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
	state = new PlayerHurt(this);
	state = new PlayerDeath(this);
	
	state = new PlayerLadderEnter(this);
	state = new PlayerLadderLeave(this);
	state = new PlayerLadderUp(this);
	state = new PlayerLadderDown(this);
	g_fSpeed = m_fSpeed;
	m_HP = g_HP = 100;
	m_Damage = 24;
	m_fZoom = 2.8f;
}
void Player::setHP(const INT& hp)
{
	m_HP -= hp;
	g_HP = m_HP;
}

//bool Player::Init()
//{
//	for (auto it : m_pStateList)
//	{
//		it.second->Init();
//	}
//	g_fSpeed = m_fSpeed;
//	m_pCurrentState = m_pStateList["Fall"];
//	Object::Init();
//	return true;
//}

//void Player::setState(T_STR szStateName)
//{
//	std::string cstate(szStateName.begin(), szStateName.end());
//	m_pCurrentState = m_pStateList[cstate];
//}

//void Player::setDir(const INT& dir)
//{
//	m_iCurrentDir *= dir;
//}
//INT	Player::getDir()
//{
//	return m_iCurrentDir;
//}

//bool Player::isFallState()
//{
//	auto it = m_pStateList.find("Fall");
//	if (it->second == m_pCurrentState)
//	{
//		return true;
//	}
//	return false;
//}
//void Player::addState(std::string Name, State* state)
//{
//	m_pStateList.insert(std::make_pair(Name, state));
//}