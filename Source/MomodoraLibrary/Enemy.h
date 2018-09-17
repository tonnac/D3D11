#pragma once
#include "CharacterObject.h"
#include "EnemyBasicState.h"


class Player;

class Enemy : public CharacterObject
{
public:
	Enemy();
	~Enemy();
public:
	bool			Init		() override;
	bool			Frame		() override;
	bool			Render		() override;
	bool			isFallState	() override;
	bool			Process		(Player * pObject);
public:
	FloatRect*		getArea();
	FloatRect*		getSight();
	FloatRect*		getAttackRange();
public:
	void			setArea(const FloatRect&);
	void 			setSight(const FloatRect&);
	void 			setAttackRange(const FloatRect&);
	void			setJumpSpeed(const FLOAT&);
public:
	bool			MoveScrollObj(const LONG&) override;
private:
	FloatRect		m_rtArea;
	FloatRect		m_rtSight;
	FloatRect		m_rtAttackRange;
};