#pragma once
#include "FinateStateMachine.h"
#include "EffectObj.h"


class State;
using StateMap = std::map<std::string, State*>;
using EffectVector = std::vector<EffectObj*>;
using EffectIter = EffectVector::iterator;

class CharacterObject : public Object
{
public:
	CharacterObject();
public:
	virtual bool 		Init			() override;
	virtual bool 		Frame			() override;
	virtual bool 		Render			() override;
	virtual bool 		Release			() override;
public:
	void				setState		(T_STR);
	void				setDir			(const INT&);
	void				setLadder		(const bool&);
	void				setJumpSpeed	(const FLOAT&, const FLOAT& = 0.0f);
	void				setJumpNum		(const INT&);
	void				setDownable		(const bool&);
	virtual void		setHP			(const INT&);
	void				setInvincible	(const bool&);
	void				setDead			(const bool&);
	void				setFSM			(FinateStateMachine* );
	void				setDamage		(const INT&);
public:
	bool				isInvincible	();
	bool				isDead			();
	INT					getDamage		();
	RECT				getEffectObj	();
	std::string			getCurrentState	();
	bool				getDownable		();
	INT					getDir			();
	FLOAT&				getJumpSpeed	(const INT& = 0);
	bool				getLadder		();
	INT					getJumpNum		();
	FLOAT				getZoom			();
	INT					getHP			();
	virtual bool		isFallState		();
public:
	void				addState		(std::string, State*);
	void				addEffect		(EffectObj *);
public:
	bool				hasNext();
	void				deleteEffect	(EffectIter&);
	EffectIter			getEffectIter	();
public:
	std::string			setTransition	(E_EVENT Event);
protected:
	FLOAT				m_fZoom;
	bool				m_bDead;
	INT					m_HP;
	INT					m_Damage;
	INT					m_iJumpNumber;
	bool				m_bInvincible;
	bool				m_bLadder;
	bool				m_bDownable;
	FLOAT				m_fInvincibleTime;
	const FLOAT			m_finvencible;
	FLOAT				m_fJumpSpeed;
	FLOAT				m_fJumpSpeed2;
	INT					m_iCurrentDir;
	std::string			m_sCurrestState;
	State *				m_pCurrentState;
	StateMap			m_pStateList;
	EffectVector		m_pEffectVector;
	EffectIter			m_EffectIter;
	FinateStateMachine*	m_fms;
};