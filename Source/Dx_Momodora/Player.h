#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
public:
	static void		setJumpNum(const INT& iNum);
public:
	static INT		getJumpNum();
private:
	static INT		m_iJumpNum;
};