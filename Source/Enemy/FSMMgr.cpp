#include "FSMMgr.h"

bool FSMMgr::Release()
{
	for (auto& iter : m_FSMMapList)
	{
		delete iter.second;
	}
	m_FSMMapList.clear();
	return true;
}
void FSMMgr::InitFSM(const std::tstring& Filepath)
{
	std::tifstream fp(Filepath.c_str());

	if (!fp.is_open())
	{
		MessageBox(nullptr, L"FILE OPEN", L"FSM FILE OPEN ERROR", MB_OK);
	}

	std::tstring buffer;
	std::tstring Name;
	Name.assign(Filepath, Filepath.find_last_of('/') + 1, Filepath.find_last_of('.') - Filepath.find_last_of('/') - 1);

	std::tstring s1;
	std::tstring type;
	std::tstring s2;
	E_EVENT eventtype;

	FinateStateMachine * nFSM = new FinateStateMachine;

	int Total;

	getline(fp, buffer);
	getline(fp, buffer);
	Total = std::stoi(buffer);

	for (int iIndex = 0; iIndex < Total; ++iIndex)
	{
		fp >> s1 >> type >> s2;
		if (type == L"FINDTARGET")
		{
			eventtype = E_EVENT::FINDTARGET;
		}
		else if (type == L"INATTACKRANGE")
		{
			eventtype = E_EVENT::INATTACKRANGE;
		}
		else if (type == L"BEATTACKED")
		{
			eventtype = E_EVENT::BEATTACKED;
		}
		else if (type == L"LOSTTARGET")
		{
			eventtype = E_EVENT::LOSTTARGET;
		}
		else if (type == L"OUTATTACKRANGE")
		{
			eventtype = E_EVENT::OUTATTACKRANGE;
		}
		else if (type == L"BEARROWATTACKED")
		{
			eventtype = E_EVENT::BEARROWATTACKED;
		}
		else if (type == L"NOHIT")
		{
			eventtype = E_EVENT::NOHIT;
		}
		else if (type == L"ARRIVE")
		{
			eventtype = E_EVENT::ARRIVE;
		}
		nFSM->AddStateTransition(s1, eventtype, s2);
	}
	m_FSMMapList.insert(std::make_pair(Name, nFSM));
}
FinateStateMachine*	FSMMgr::LoadFSM(const std::tstring& Name)
{
	FSMList::iterator iter;
	iter = m_FSMMapList.find(Name);
	if (iter == m_FSMMapList.end())
	{
		return nullptr;
	}
	return iter->second;
}