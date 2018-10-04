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
	INT				getQuickNum() const;
	bool			isInvincible() const;
	bool			isBuff() const;
public:
	void			setBuff(const bool& flag);
	void			setDead() override;
	void			setInvincible(const bool& flag);
private:
	bool			m_bBuff;
	FLOAT			m_fTimer;
	FLOAT			m_fInvincibleTimer;
	bool			m_Ladder;
	bool			m_bInvincible;
	static INT		m_iJumpNum;
	bool			m_Down;
	INT				m_iEquipNumber;
	INT				m_iQuickSlotNum;
	FLOAT			m_fAttackScale;
};

using PlayerPTR = std::shared_ptr<Player>;