#pragma once
#include "State.h"
#include "Player.h"

class EffectObj;

class PlayerState : public State
{
public:
	PlayerState(Player*);
public:
	virtual bool		Init() =0;
	virtual bool		Frame() =0;
	virtual	bool		Render();
	virtual	bool		Release();
protected:
	Player *			m_pCharObj;
};
class PlayerIdle : public PlayerState
{
public:
	PlayerIdle(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerRun : public PlayerState
{
public:
	PlayerRun(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerBrake : public PlayerState
{
public:
	PlayerBrake(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerTurn : public PlayerState
{
public:
	PlayerTurn(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerJump : public PlayerState
{
public:
	PlayerJump(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
private:
	FLOAT		m_fAcceleration;
	FLOAT&		m_fJumpSpeed;
};
class PlayerJump2 : public PlayerState
{
public:
	PlayerJump2(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
private:
	FLOAT		m_fAcceleration;
	FLOAT&		m_fJumpSpeed;
};
class PlayerFall : public PlayerState
{
public:
	PlayerFall(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
private:
	FLOAT		m_fAcceleration;
};
class PlayerRise : public PlayerState
{
public:
	PlayerRise(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerCrouch : public PlayerState
{
public:
	PlayerCrouch(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerLadderEnter : public PlayerState
{
public:
	PlayerLadderEnter(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerLadderLeave : public PlayerState
{
public:
	PlayerLadderLeave(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerLadderUp : public PlayerState
{
public:
	PlayerLadderUp(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerLadderDown : public PlayerState
{
public:
	PlayerLadderDown(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerRoll : public PlayerState
{
public:
	PlayerRoll(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerHurt : public PlayerState
{
public:
	PlayerHurt(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerDeath : public PlayerState
{
public:
	PlayerDeath(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
