#include "EffectObj.h"

bool EffectObj::Init()
{
	return true;
}
bool EffectObj::Frame()
{
	if (!m_pEffectSprite->Frame())
	{
		m_pEffectSprite->setIndex(0);
		return false;
	}
	m_rtDraw = m_pEffectSprite->getSpriteRt();
	Object::Frame();
	return true;
}
void EffectObj::setSpeed(const FLOAT& pt)
{
	m_fSpeed = pt;
}
void EffectObj::setDrawPos(const FloatPoint& pt)
{
	m_CenterPos.x = pt.x;
	m_CenterPos.y = pt.y;
}
void EffectObj::setIndex(const int& index)
{
	m_pEffectSprite->setIndex(index);
}

void * EffectObj::operator new(size_t sz, const char* FileName, int iLine)
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
void EffectObj::operator delete(void * p)
{
	std::map<void*, MEMINFO>::iterator it;
	it = MemoryMap.find(p);
	MemoryMap.erase(it);
	--::g_inewCount;
	delete p;
}