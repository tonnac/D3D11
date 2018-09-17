#include "FinateStateMachine.h"

FinateState::FinateState()
{
	m_mapOutput.clear();
}

void FinateState::AddTransition(const E_EVENT& eventtype, const std::string& statetype)
{
	m_mapOutput[eventtype] = statetype;
}
std::string FinateState::GetState(const E_EVENT& eventtype)
{
	return m_mapOutput[eventtype];
}

void * FinateState::operator new(size_t sz, const char* FileName, int iLine)
{
	std::string ad = FileName;
	MEMINFO mem;
	void* pfs = new char[sz];
	mem.addr = pfs;
	mem.filename = ad.substr(ad.find_last_of('\\') + 1, ad.length() - (ad.find_last_of(".cpp") + 4));
	mem.line = iLine;
	mem.dwAllocateTime = timeGetTime();
	MemoryMap.insert(std::make_pair(pfs, mem));
	++::g_inewCount;
	return pfs;
}
void FinateState::operator delete(void * p)
{
	std::map<void*, MEMINFO>::iterator it;
	it = MemoryMap.find(p);
	MemoryMap.erase(it);
	--::g_inewCount;
	delete p;
}
FinateStateMachine::FinateStateMachine()
{
	m_mapState.clear();
}
FinateStateMachine::~FinateStateMachine()
{
}
void FinateStateMachine::AddStateTransition(const std::string& inputState,
						const E_EVENT& eventtype, const std::string& outputState)
{
	FinateState * nstate = nullptr;
	EnemyIterator iter = m_mapState.find(inputState);
	if (iter == m_mapState.end())
	{
		nstate = new FinateState;
		m_mapState[inputState] = nstate;
	}
	else
	{
		nstate = iter->second;
	}
	nstate->AddTransition(eventtype, outputState);
}
std::string FinateStateMachine::StateTransition(const std::string& currentstate, const E_EVENT& eventtype)
{
	return m_mapState[currentstate]->GetState(eventtype);
}
void * FinateStateMachine::operator new(size_t sz, const char* FileName, int iLine)
{
	std::string ad = FileName;
	MEMINFO mem;
	void* pfs = new char[sz];
	mem.addr = pfs;
	mem.filename = ad.substr(ad.find_last_of('\\') + 1, ad.length() - (ad.find_last_of(".cpp") + 4));
	mem.line = iLine;
	mem.dwAllocateTime = timeGetTime();
	MemoryMap.insert(std::make_pair(pfs, mem));
	++::g_inewCount;
	return pfs;
}
void FinateStateMachine::operator delete(void * p)
{
	std::map<void*, MEMINFO>::iterator it;
	it = MemoryMap.find(p);
	MemoryMap.erase(it);
	--::g_inewCount;
	delete p;
}