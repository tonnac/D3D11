#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
public:
	bool			Frame() override;

public:
	static void		setJumpNum(const INT& iNum);
public:
	static INT		getJumpNum();
private:
	FLOAT			m_fTimer;
	bool			m_bInvincible;
	static INT		m_iJumpNum;
};