#include "FSMMgr.h"

FSMMgr::FSMMgr()
{

}

FSMMgr::~FSMMgr()
{
	Release();
}

bool FSMMgr::Init()
{
	return true;
}
bool FSMMgr::Frame()
{
	return true;
}
bool FSMMgr::Render()
{
	return true;
}
bool FSMMgr::Release()
{
	for (auto it : m_FSMMapList)
	{
		delete it.second;
	}
	m_FSMMapList.clear();
	return true;
}
T_STR FSMMgr::LoadFile(T_STR szName, T_STR szFile)
{
	std::ifstream fp(szFile.c_str());

	if (!fp.is_open())
	{
		MessageBox(nullptr, L"FILE OPEN", L"FSM FILE OPEN ERROR", MB_OK);
	}
		
	if (getFSMPtr(szName))
	{
		return szName;
	}

	std::string buffer;

	std::string s1;
	std::string type;
	std::string s2;
	E_EVENT eventtype;

	FinateStateMachine * nFSM = new FinateStateMachine;

	int Total;

	getline(fp, buffer);
	getline(fp, buffer);
	Total = std::stoi(buffer);

	for (int iIndex = 0; iIndex < Total; ++iIndex)
	{
		fp >> s1 >> type >> s2;
		if (type == "FINDTARGET")
		{
			eventtype = E_EVENT::FINDTARGET;
		}
		else if (type == "INATTACKRANGE")
		{
			eventtype = E_EVENT::INATTACKRANGE;
		}
		else if (type == "BEATTACKED")
		{
			eventtype = E_EVENT::BEATTACKED;
		}
		else if (type == "LOSTTARGET")
		{
			eventtype = E_EVENT::LOSTTARGET;
		}
		else if (type == "OUTATTACKRANGE")
		{
			eventtype = E_EVENT::OUTATTACKRANGE;
		}
		else if (type == "BEARROWATTACKED")
		{
			eventtype = E_EVENT::BEARROWATTACKED;
		}
		else if (type == "NOHIT")
		{
			eventtype = E_EVENT::NOHIT;
		}
		else if (type == "ARRIVE")
		{
			eventtype = E_EVENT::ARRIVE;
		}
		nFSM->AddStateTransition(s1, eventtype, s2);
	}

	m_FSMMapList.insert(std::make_pair(szName, nFSM));
	return szName;
}

FinateStateMachine* FSMMgr::getFSMPtr(T_STR szName)
{
	for (auto it2 : m_FSMMapList)
	{
		if (it2.first == szName)
			return it2.second;
	}
	return nullptr;
}
