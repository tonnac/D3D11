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

using EnemyOutputMap = std::map<E_EVENT, std::string>;
using EnemyMapState = std::map<std::string, FinateState*>;
using EnemyIterator = EnemyMapState::iterator;

class FinateState
{
public:
	FinateState();
public:
	void AddTransition			(const E_EVENT&, const std::string&);
	std::string GetState		(const E_EVENT&);
	void * operator new			(size_t sz, const char* FileName, int iLine);
	void operator delete		(void * p);
private:
	EnemyOutputMap m_mapOutput;
};

class FinateStateMachine
{
public:
	FinateStateMachine();
	~FinateStateMachine();
public:
	void AddStateTransition		(const std::string&, const E_EVENT&, const std::string&);
	std::string StateTransition	(const std::string&, const E_EVENT&);
	void * operator new			(size_t sz, const char* FileName, int iLine);
	void operator delete		(void * p);
private:
	EnemyMapState m_mapState;
};