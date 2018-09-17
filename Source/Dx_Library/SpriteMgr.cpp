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
bool SpriteMgr::SpriteSet(const std::tstring& SpriteList)
{
	std::tifstream fp(SpriteList.c_str());
	
	std::tstring filestring(SpriteList.begin(), SpriteList.end());
	std::tstring pe = filestring.substr(filestring.find_last_of('/')+1, filestring.length());
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
	std::vector<std::tstring> temp;
	std::tstring buffer;

	std::getline(fp, buffer);
	int Total = std::stoi(buffer);
	temp.resize(Total);
	for (int i = 0; i < Total; i++)
	{
		int k;
		getline(fp, buffer);
		std::tistringstream is(buffer);
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
				std::tistringstream is(buffer);
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
Sprite*	SpriteMgr::LoadSprite(const std::tstring& Name, const std::tstring& SpriteName)
{
	std::tstring cName(Name.begin(), Name.end());
	std::tstring Sprite_Name(SpriteName.begin(), SpriteName.end());
	TSPMAP::iterator it;
	SPMAP::iterator iter;
	
	it = m_SpriteMap.find(cName);
	iter = it->second.find(Sprite_Name);
	assert(iter != it->second.end());
	return iter->second;
}