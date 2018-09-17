#pragma once
#include "Sprite.h"

using SPMAP = std::map<std::tstring, Sprite*>;
using TSPMAP = std::map<std::tstring, SPMAP>;

class SpriteMgr : public Singleton<SpriteMgr>
{
	friend class Singleton<SpriteMgr>;
public:
	~SpriteMgr();
public:
	bool				Init		();
	bool				Frame		();
	bool				Render		();
	bool				Release		();
public:
	bool				SpriteSet	(const std::tstring& SpriteList);
	Sprite*				LoadSprite	(const std::tstring& Name, const std::tstring& SpriteName);
private:
	TSPMAP				m_SpriteMap;
};


#define S_Sprite SpriteMgr::getInst()