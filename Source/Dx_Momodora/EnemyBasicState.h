#pragma once
#include "State.h"
#include "FinateStateMachine.h"

class EffectObj;
class Enemy;
class Player;

class EnemyBasicState : public State
{
public:
	EnemyBasicState(Enemy*);
public:
	virtual bool		Init() = 0;
	virtual bool		Frame();
	bool				Release();
	bool				Collision(D2D1_RECT_F* Rect);
protected:
	Enemy *				m_pEnemy;
	D2D1_RECT_F*		m_rtArea;
	D2D1_RECT_F*		m_rtSight;
	D2D1_RECT_F*		m_rtAttackRange;
};

class EnemyMoveState : public EnemyBasicState
{
public:
	EnemyMoveState(Enemy*);
public:
	bool				Init	() override;
	bool				Frame	() override;
};
class EnemyChaseState : public EnemyBasicState
{
public:
	EnemyChaseState(Enemy*);
public:
	bool				Init() override;
	bool				Frame() override;
};
class EnemyAttackState : public EnemyBasicState
{
public:
	EnemyAttackState(Enemy*);
public:
	bool				Init() override;
	bool				Frame() override;
};

class EnemyHitState : public EnemyBasicState
{
public:
	EnemyHitState(Enemy*);
public:
	bool				Init() override;
	bool				Frame() override;
};

class EnemyAngryState : public EnemyBasicState
{
public:
	EnemyAngryState(Enemy*);
public:
	bool				Init() override;
	bool				Frame() override;
};

class EnemyMoveLocationState : public EnemyBasicState
{
public:
	EnemyMoveLocationState(Enemy*);
public:
	bool				Init() override;
	bool				Frame() override;
};