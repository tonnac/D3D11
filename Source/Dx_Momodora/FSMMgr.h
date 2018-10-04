#pragma once
#include "FinateStateMachine.h"

using FSMList = std::map<std::tstring, FinateStateMachine*>;

class FSMMgr : public Singleton<FSMMgr>
{
	friend class Singleton<FSMMgr>;
private:
	FSMMgr() = default;
public:
	bool				Release();
public:
	void				InitFSM(const std::tstring& Filepath);
	FinateStateMachine*	LoadFSM(const std::tstring& Name);
private:
	FSMList				m_FSMMapList;
};

#define S_FSMMgr FSMMgr::getInst()