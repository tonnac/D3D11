#pragma once
#include "Character.h"
#include "MiscSet.h"

class Player : public Character
{
public:
	Player();
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	bool			Frame() override;
	bool			Render(ID3D11DeviceContext* pContext) override;
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
	void			setHP(const FLOAT& iVal) override;
	void			setBuff(const bool& flag);
	void			setDead(const bool& flag);
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
	MiscSet			m_Misc;
};

using PlayerPTR = std::shared_ptr<Player>;