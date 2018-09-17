#include "SpriteMgr.h"

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
bool SpriteMgr::SpriteSet(T_STR szSpriteList)
{
	std::ifstream fp(szSpriteList.c_str());
	
	std::string filestring(szSpriteList.begin(), szSpriteList.end());
	std::string pe = filestring.substr(filestring.find_last_of('/')+1, filestring.length());
	pe = pe.substr(0, pe.find('.'));
	SPMAP addmap;

	if (!m_SpriteMap[pe].empty())
	{
		return true;
	}

	if (!fp.is_open())
	{
		MessageBox(nullptr, L"FILE OPEN", L"SPRITE OPEN ERROR", MB_OK);
	}
	std::vector<std::string> temp;
	std::string buffer;

	std::getline(fp, buffer);
	int Total = std::stoi(buffer);
	temp.resize(Total);
	for (int i = 0; i < Total; i++)
	{
		int k;
		getline(fp, buffer);
		std::istringstream is(buffer);
		is >> temp[i] >> k;
		//Sprite* pl = new Sprite(k);
		Sprite* pl = new Sprite(k);
		addmap.insert(std::make_pair(temp[i], pl));
	}
	while (!fp.eof())
	{
		for (auto it : temp)
		{
			SPRITELIST sl;
			for (int i = 0; i < addmap[it]->Size() ;++i)
			{
				int l, t, r, b;
				std::getline(fp, buffer);
				std::istringstream is(buffer);
				is >> l >> t >> r >> b;;
				RECT rt = { l,t,r,b };
				sl.push_back(rt);
			}
			addmap[it]->setList(sl);
		}
	}
	m_SpriteMap[pe] = addmap;
	return true;
}
Sprite*	SpriteMgr::LoadSprite(T_STR szCName, T_STR szSpriteName)
{
	std::string cName(szCName.begin(), szCName.end());
	std::string SpriteName(szSpriteName.begin(), szSpriteName.end());
	TSPMAP::iterator it;
	SPMAP::iterator iter;
	
	it = m_SpriteMap.find(cName);
	iter = it->second.find(SpriteName);
	assert(iter != it->second.end());
	return iter->second;
}