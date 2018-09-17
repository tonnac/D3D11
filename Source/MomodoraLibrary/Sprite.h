#pragma once
#include "Std.h"

using SPRITELIST = std::vector<RECT>;

class Sprite
{
public:
	Sprite(const int&);
	~Sprite();
public:
	void * operator new(size_t sz, const char* FileName, int iLine);
	void operator delete(void * p);
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
