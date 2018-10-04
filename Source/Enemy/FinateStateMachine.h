#pragma once
#include "EnemyBasicState.h"

enum class E_EVENT : unsigned char
{
	FINDTARGET,
	LOSTTARGET,
	BEATTACKED,
	BEARROWATTACKED,
	NOHIT,
	INATTACKRANGE,
	OUTATTACKRANGE,
	ARRIVE
};

class FinateState;

using EnemyOutputMap = std::map<E_EVENT, std::tstring>;
using EnemyMapState = std::map<std::tstring, FinateState*>;
using EnemyIterator = EnemyMapState::iterator;

class FinateState
{
public:
	FinateState();
public:
	void AddTransition			(const E_EVENT&, const std::tstring&);
	std::tstring GetState		(const E_EVENT&);
private:
	EnemyOutputMap m_mapOutput;
};

class FinateStateMachine
{
public:
	FinateStateMachine();
	~FinateStateMachine();
public:
	void AddStateTransition		(const std::tstring&, const E_EVENT&, const std::tstring&);
	std::tstring StateTransition	(const std::tstring&, const E_EVENT&);
private:
	EnemyMapState m_mapState;
};