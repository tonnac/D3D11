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
	void			setLadder(const bool& ladder);
public:
	static INT		getJumpNum();
	bool			getDown() const;
	bool			getLadder() const;
	INT				getConsumableNum() const;
	INT				getEquipNum() const;
private:
	FLOAT			m_fTimer;
	bool			m_Ladder;
	bool			m_bInvincible;
	static INT		m_iJumpNum;
	bool			m_Down;
	INT				m_iConsumableNumber;
	INT				m_iEquipNumber;
};

using PlayerPTR = std::shared_ptr<Player>;