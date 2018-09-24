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
D3DXVECTOR4 Sprite::getSpriteVt()
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
			iter.second->Release();
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
	SPMAP addmap;

	if (!m_SpriteMap[pe].empty())
	{
		return true;
	}

	std::vector<std::tstring> temp;
	std::tstring buffer;

	std::getline(fp, buffer);
	int Total = std::stoi(buffer);
	temp.resize(Total);
	for (int i = 0; i < Total; i++)
	{
		int k;
		fp >> temp[i] >> k;
		Sprite* pl = new Sprite(k);
		addmap.insert(std::make_pair(temp[i], pl));
	}
	while (!fp.eof())
	{
		for (auto it : temp)
		{
			SPRITELIST sl;
			for (int i = 0; i < addmap[it]->Size();++i)
			{
				FLOAT l, t, r, b;
				fp >> l >> t >> r >> b;;
				r += l, b += t;
				D3DXVECTOR4 rt = { l,t,r,b };
				sl.push_back(rt);
			}
			addmap[it]->setList(sl);
		}
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