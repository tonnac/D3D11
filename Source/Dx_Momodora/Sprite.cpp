#include "Sprite.h"

Sprite::Sprite() : m_fSpriteTime(0.0f), m_fDivideTime(1.0f), m_iIndex(0)
{
	m_fOffset = m_fDivideTime / m_spritelist.size();
}
Sprite::Sprite(const int& k) : m_fSpriteTime(0.0f), m_fDivideTime(1.0f), m_iIndex(0)
{
	m_spritelist.resize(k);
	m_fOffset = m_fDivideTime / m_spritelist.size();
}
Sprite::~Sprite()
{

}
bool Sprite::Frame()
{
	m_fSpriteTime += g_fSecPerFrame;
	if (m_fSpriteTime >= m_fOffset)
	{
		m_fSpriteTime -= m_fOffset;
		if (++m_iIndex >= m_spritelist.size())
		{
			m_fSpriteTime = 0.0f;
			return false;
		}
	}
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
INT	Sprite::getIndex() const
{
	return m_iIndex;
}
void Sprite::pushSprite(const D3DXVECTOR4& Vec)
{
	m_spritelist.push_back(Vec);
}
D3DXVECTOR4 Sprite::getSpriteVt() const
{
	return m_spritelist[m_iIndex];
}

///////////////////////////////////////////////////////

SpriteMgr::~SpriteMgr()
{
	Release();
}

bool SpriteMgr::Init()
{
	return true;
}
bool SpriteMgr::Frame()
{
	return true;
}
bool SpriteMgr::Render()
{
	return true;
}
bool SpriteMgr::Release()
{
	for (auto it : m_SpriteMap)
	{
		for (auto iter : it.second)
		{
			delete iter.second;
		}
	}
	m_SpriteMap.clear();
	return true;
}
bool SpriteMgr::SpriteSet(const std::tstring& Filepath)
{
	std::tifstream fp;
	FileExceptErr(fp, Filepath);

	std::tstring pe = Filepath.substr(Filepath.find_last_of('/') + 1, Filepath.length());
	pe = pe.substr(0, pe.find('.'));

	if (!m_SpriteMap[pe].empty())
	{
		return true;
	}

	std::tstring buffer;
	SPMAP addmap;
	std::getline(fp, buffer);
	int Total = std::stoi(buffer);
	for (int i = 0; i < Total; ++i)
	{
		std::tstring temp;
		int k;
		fp >> temp >> k;
		Sprite* pl = new Sprite;
		for (int j = 0; j < k; ++j)
		{
			D3DXVECTOR4 rt;
			fp >> rt.x >> rt.y >> rt.z >> rt.w;
			rt.z += rt.x, rt.w += rt.y;
			pl->pushSprite(rt);
		}
		pl->setDivideTime(1.0f);
		addmap.insert(std::make_pair(temp, pl));
	}
	m_SpriteMap[pe] = addmap;
	return true;
}
Sprite*	SpriteMgr::LoadSprite(const std::tstring& Name, const std::tstring& SpriteName)
{
	TSPMAP::iterator it;
	SPMAP::iterator iter;

	it = m_SpriteMap.find(Name);
	iter = it->second.find(SpriteName);
	assert(iter != it->second.end());
	return iter->second;
}