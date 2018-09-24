#pragma once
#include "d3dUtil.h"

using SPRITELIST = std::vector<D3DXVECTOR4>;

class Sprite
{
public:
	Sprite(const int&);
	~Sprite();
public:
	bool				Init			();
	bool				Frame			();
	bool				Render			();
	bool				Release			();
public:
	D3DXVECTOR4			getSpriteVt		();
	bool				setDivideTime	(const FLOAT& offset);
	bool				setIndex		(const INT& rindex);
	INT					getIndex		();
	bool				setList			(const SPRITELIST& rt);
	size_t				Size			();
private:
	INT					m_iIndex;
	FLOAT				m_fDivideTime;
	FLOAT				m_fSpriteTime;
	FLOAT				m_fOffset;
	SPRITELIST			m_spritelist;
};


using SPMAP = std::map<std::tstring, Sprite*>;
using TSPMAP = std::map<std::tstring, SPMAP>;

class SpriteMgr : public Singleton<SpriteMgr>
{
	friend class Singleton<SpriteMgr>;
public:
	~SpriteMgr();
public:
	bool				Init();
	bool				Frame();
	bool				Render();
	bool				Release();
public:
	bool				SpriteSet(const std::tstring& Filepath);
	Sprite*				LoadSprite(const std::tstring& Name, const std::tstring& SpriteName);
private:
	TSPMAP				m_SpriteMap;
};


#define S_Sprite SpriteMgr::getInst()