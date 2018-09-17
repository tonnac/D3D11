#include "State.h"
#include "CharacterObject.h"
#include "EffectObj.h"

State::State(CharacterObject* pObject) : m_pEffectObj(nullptr), m_fTimer(0.0f)
{}
void State::setSprite(T_STR cName, T_STR szSprite)
{
	m_pSprite = S_SpriteMgr.LoadSprite(cName, szSprite);
}
RECT State::getEffectRT()
{
	if (m_pEffectObj)
	{
		return *m_pEffectObj->getCollisionRt();
	}
	return RECT();
}
bool State::Frame()
{
	return true;
}
bool State::Render()
{
	return true;
}
bool State::Release()
{
	return true;
}
bool State::Process(Player* pPlayer)
{
	return true;
}
void * State::operator new(size_t sz, const char* FileName, int iLine)
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
void State::operator delete(void * p)
{
	std::map<void*, MEMINFO>::iterator it;
	it = MemoryMap.find(p);
	MemoryMap.erase(it);
	--::g_inewCount;
	delete p;
}