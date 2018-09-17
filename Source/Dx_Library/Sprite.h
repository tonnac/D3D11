#pragma once
#include "Core.h"

using SPRITELIST = std::vector<RECT>;

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
	RECT				getSpriteRt		();
	bool				setDivideTime	(const FLOAT&);
	bool				setIndex		(const INT&);
	INT					getIndex		();
	bool				setList			(const SPRITELIST&);
	size_t				Size			();
private:
	INT					m_iIndex;
	FLOAT				m_fDivideTime;
	FLOAT				m_fSpriteTime;
	FLOAT				m_fOffset;
	SPRITELIST			m_spritelist;
};
