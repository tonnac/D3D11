#pragma once
#include "Character.h"
#include "EnemyBasicState.h"


class Player;

class Enemy : public Character
{
public:
	Enemy();
	~Enemy();
public:
	bool			Init		() override;
	bool			Frame		() override;
public:
	D2D1_RECT_F*	getArea();
	D2D1_RECT_F*	getSight();
	D2D1_RECT_F*	getAttackRange();
public:
	void			setArea(const D2D1_RECT_F&);
	void 			setSight(const D2D1_RECT_F&);
	void 			setAttackRange(const D2D1_RECT_F&);
	void			setSpeed(const FLOAT& fSpeed);
	std::tstring	setTransition(E_EVENT Event);
	void			setDir(const INT& iDir);
	void			setFSM(FinateStateMachine* pFSM);
public:
	COL				Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize) override;
	COL				Collision();
//	bool			MoveScrollObj(const LONG&) override;
private:
	std::tstring	m_CurState;
	INT				m_Damage;
	D2D1_RECT_F		m_rtArea;
	D2D1_RECT_F		m_rtSight;
	D2D1_RECT_F		m_rtAttackRange;
	FinateStateMachine*	m_fms;
};