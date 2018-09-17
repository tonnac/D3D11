#pragma once
#include "Enemy.h"

class ScrollObject;

class Scroll
{
public:
	Scroll(Object * pBkObj, std::list<Enemy*>* npcVector = nullptr);
public:
	bool				Init			();
	bool				Frame			();
	bool				Render			();
	bool				Release			();
	bool				MoveCamera		(const LONG& size);
	bool				MoveCenter		(const LONG& size);
	void				setScene5		(const bool&);
public:
	void				AddScrollObject	(ScrollObject*);
private:
	bool				Collision		(const RECT&);
private:
	bool				m_bScene5;
	std::array<INT,2>	m_nInitValue;
	std::list<Enemy*>*	m_npclist;
	Object*				m_pPlayer;
	Object*				m_pBkObj;
	ScrollObject*		m_pScrollObject;
	RECT*				m_PlayerCollisionRt;
	RECT*				m_BkRtDraw;
	FloatPoint			m_CenterPos;
	RECT				m_rtCollision;
};