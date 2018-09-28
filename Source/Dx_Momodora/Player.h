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
	void			setDown(const bool& down);
public:
	static INT		getJumpNum();
	bool			getDown() const;
private:
	FLOAT			m_fTimer;
	bool			m_bInvincible;
	static INT		m_iJumpNum;
	bool			m_Down;
};