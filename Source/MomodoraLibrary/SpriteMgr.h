#pragma once
#include "Sprite.h"
#include <fstream>
#include <sstream>

using SPMAP = std::map<std::string, Sprite*>;
using TSPMAP = std::map<std::string, SPMAP>;

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
	bool				SpriteSet	(T_STR);
	Sprite*				LoadSprite	(T_STR, T_STR);
private:
	TSPMAP				m_SpriteMap;
};


#define S_SpriteMgr SpriteMgr::getInstance()