#pragma once
#include "State.h"
#include "Collision.h"

class EffectObj;
class Enemy;
class Player;

class EnemyBasicState : public State
{
public:
	EnemyBasicState(Enemy*);
public:
	virtual bool		Init() = 0;
	virtual bool		Process(Player *) = 0;
	virtual bool		Render() override;
	bool				Release();
protected:
	Enemy *				m_pEnemy;
	FloatRect*			m_rtArea;
	FloatRect*			m_rtSight;
	FloatRect*			m_rtAttackRange;
};

class EnemyMoveState : public EnemyBasicState
{
public:
	EnemyMoveState(Enemy*);
public:
	bool				Init	() override;
	bool				Process	(Player *) override;
};
class EnemyChaseState : public EnemyBasicState
{
public:
	EnemyChaseState(Enemy*);
public:
	bool				Init() override;
	bool				Process(Player *) override;
};
class EnemyAttackState : public EnemyBasicState
{
public:
	EnemyAttackState(Enemy*);
public:
	bool				Init() override;
	bool				Process(Player *) override;
};

class EnemyHitState : public EnemyBasicState
{
public:
	EnemyHitState(Enemy*);
public:
	bool				Init() override;
	bool				Process(Player *) override;
};

class EnemyAngryState : public EnemyBasicState
{
public:
	EnemyAngryState(Enemy*);
public:
	bool				Init() override;
	bool				Process(Player *) override;
};

class EnemyMoveLocationState : public EnemyBasicState
{
public:
	EnemyMoveLocationState(Enemy*);
public:
	bool				Init() override;
	bool				Process(Player *) override;
};