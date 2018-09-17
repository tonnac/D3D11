#pragma once
#include "FinateStateMachine.h"

using FSMList = std::map<T_STR, FinateStateMachine*>;

class FSMMgr : public Singleton<FSMMgr>
{
	friend class Singleton<FSMMgr>;
public:
	FSMMgr();
	~FSMMgr();
public:
	bool					Init			();
	bool					Frame			();
	bool					Render			();
	bool					Release			();
public:
	T_STR					LoadFile		(T_STR, T_STR);
	FinateStateMachine*		getFSMPtr		(T_STR);
private:
	FSMList					m_FSMMapList;
};

#define S_FSMMgr	FSMMgr::getInstance()