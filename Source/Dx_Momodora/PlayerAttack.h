#pragma once
#include "PlayerBasicState.h"
#include "Effect.h"

class MeleeAttack : public PlayerState
{
public:
	MeleeAttack(Player*);
public:
	virtual bool		Init() = 0;
	virtual bool		Frame() = 0;
};

class PlayerAttack : public MeleeAttack
{
public:
	PlayerAttack(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerAttack2 : public MeleeAttack
{
public:
	PlayerAttack2(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class PlayerAttack3 : public MeleeAttack
{
public:
	PlayerAttack3(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class AirAttack : public MeleeAttack
{
public:
	AirAttack(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
private:
	FLOAT		m_fAcceleration;
};
class BowAttack : public PlayerState
{
public:
	BowAttack(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};
class AirBowAttack : public PlayerState
{
public:
	AirBowAttack(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
private:
	FLOAT		m_fAcceleration;
};
class CrouchBowAttack : public PlayerState
{
public:
	CrouchBowAttack(Player*);
public:
	bool		Init() override;
	bool		Frame() override;
};