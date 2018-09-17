#include "Sprite.h"

Sprite::Sprite(const int& k)
{
	m_spritelist.resize(k);

	m_fSpriteTime = 0.0f;
	m_fDivideTime = 1.0f;
	m_iIndex = 0;
	m_fOffset = m_fDivideTime / m_spritelist.size();
}
Sprite::~Sprite()
{

}

bool Sprite::Init()
{
	return true;
}
bool Sprite::Frame()
{
	m_fSpriteTime += g_fSecPerFrame;
	if (m_fSpriteTime >= m_fOffset)
	{
		m_fSpriteTime -= m_fOffset;
		if (++m_iIndex >= m_spritelist.size())
		{
			return false;
		}
	}
	return true;
}
bool Sprite::Render()
{
	return true;
}
bool Sprite::Release()
{
	return true;
}
bool Sprite::setList(const SPRITELIST& rt)
{
	m_spritelist = rt;
	return true;
}
size_t Sprite::Size()
{
	return m_spritelist.size();
}
bool Sprite::setDivideTime(const FLOAT& offset)
{
	m_fDivideTime = offset;
	m_fOffset = m_fDivideTime / m_spritelist.size();
	return true;
}
bool Sprite::setIndex(const INT& rindex)
{
	if (rindex < 0 || rindex > m_spritelist.size() - 1)
	{
		return false;
	}
	m_iIndex = rindex;
	return true;
}
INT	Sprite::getIndex()
{
	return m_iIndex;
}
RECT Sprite::getSpriteRt()
{
	return m_spritelist[m_iIndex];
}
void * Sprite::operator new(size_t sz, const char* FileName, int iLine)
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
void Sprite::operator delete(void * p)
{
	std::map<void*, MEMINFO>::iterator it;
	it = MemoryMap.find(p);
	MemoryMap.erase(it);
	--::g_inewCount;
	free(p);
}