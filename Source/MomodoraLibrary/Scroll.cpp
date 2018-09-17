#include "Scroll.h"
#include "Player.h"
#include "ScrollObject.h"

Scroll::Scroll(Object * pBkObj, std::list<Enemy*>* npcVector) : m_pBkObj(pBkObj), m_npclist(npcVector),
		m_BkRtDraw(pBkObj->getrtDraw()), m_bScene5(false), m_pScrollObject(nullptr)
{}

bool Scroll::Init()
{
	m_pPlayer = g_pPlayer;
	m_PlayerCollisionRt = g_pPlayer->getCollisionRt();
	m_rtCollision.left = g_rtClient.left + 150;
	m_rtCollision.top = g_rtClient.top;
	m_rtCollision.right = (g_rtClient.left + 620 + g_rtClient.right) / 2;
	m_rtCollision.bottom = g_rtClient.bottom;

	m_CenterPos.x = static_cast<FLOAT>((m_rtCollision.left + m_rtCollision.right) / 2);
	m_CenterPos.y = static_cast<FLOAT>((m_rtCollision.top + m_rtCollision.bottom) / 2);

	m_nInitValue.at(0) = m_BkRtDraw->left;
	m_nInitValue.at(1) = m_BkRtDraw->left + m_BkRtDraw->right;
	return true;
}
bool Scroll::Frame()
{
	Collision(*m_PlayerCollisionRt);
	return true;
}
bool Scroll::Render()
{
	return true;
}
bool Scroll::Release()
{
	return true;
}
bool Scroll::Collision(const RECT& rt)
{
	Player * pl = dynamic_cast<Player*>(m_pPlayer);
	if (pl->getCurrentState() == "Attack1" || pl->getCurrentState() == "Attack2" || pl->getCurrentState() == "Attack3"
		|| pl->getCurrentState() == "AirAttack")
	{
		return true;
	}
	LONG x1;			//충돌영역 계산
	
	POINT A_Center;
	A_Center.x = (rt.right + rt.left) / 2;
	A_Center.y = (rt.bottom + rt.top) / 2;

	LONG size = rt.right - rt.left;

	LONG xDiff = static_cast<LONG>(abs(A_Center.x - m_CenterPos.x));
	LONG yDiff = static_cast<LONG>(abs(A_Center.y - m_CenterPos.y));

	if (m_bScene5 && CollisionClass::RectInRect(rt, *m_pScrollObject->getCollisionRt()))
	{
		LONG x1Diff = m_pPlayer->getCenterPos()->x - 200;
		if (x1Diff == 0)
		{
			return true;
		}
		if (x1Diff != 0)
		{
			x1Diff /= abs(x1Diff);
			return MoveCenter(x1Diff);
		}
	}
	if (rt.right >= m_rtCollision.right)
	{
		x1 = rt.right - m_rtCollision.right;
		if ((m_nInitValue[1] - m_BkRtDraw->left)
			<= g_rtClient.right)  // 화면 끝 도달
		{
			return true;
		}
		return MoveCamera(x1);
	}
	else if (rt.left < m_rtCollision.left)
	{
		x1 = rt.left - m_rtCollision.left;
		if (m_BkRtDraw->left - m_nInitValue[0] <= g_rtClient.left)  // 화면 끝 도달
		{
			return true;
		}
		if (pl->getDir() == -1)
		{
			return MoveCamera(x1);
		}
	}
	else
	{
		if (m_BkRtDraw->left - m_nInitValue[0] <= g_rtClient.left)  // 화면 끝 도달
		{
			return true;
		}
		else if ((m_nInitValue[1] - m_BkRtDraw->left)
			<= g_rtClient.right)  // 화면 끝 도달
		{
			return true;
		}
		LONG x1Diff = m_pPlayer->getCenterPos()->x - g_rtClient.right / 2;
		if (x1Diff != 0 && pl->getCurrentState() != "Run")
		{
			x1Diff /= abs(x1Diff);
			return MoveCenter(x1Diff);
		}
	}
	return true;
}


bool Scroll::MoveCamera(const LONG& size)
{
	FLOAT x = (*m_pPlayer->getCenterPos()).x;
	m_pPlayer->setCenterPos_x(x - size);
	for (auto it : *m_npclist)
	{
		FLOAT x1 = (*it->getCenterPos()).x;
		it->MoveScrollObj(size);
		it->setCenterPos_x(x1 - size);
	}
	m_pBkObj->MoveScrollBk(size);
	return true;
}
bool Scroll::MoveCenter(const LONG& size)
{
	FLOAT x = (*m_pPlayer->getCenterPos()).x;
	m_pPlayer->setCenterPos_x(x - size * g_fSecPerFrame * g_fSpeed);
	for (auto it : *m_npclist)
	{
		FLOAT x1 = (*it->getCenterPos()).x;
		it->MoveScrollObj(size);
		it->setCenterPos_x(x1 - size * g_fSecPerFrame * g_fSpeed);
	}
	m_pBkObj->MoveScrollBk(size);
	return true;
}
void Scroll::setScene5(const bool& bflag)
{
	m_bScene5 = true;
}
void Scroll::AddScrollObject(ScrollObject* sobject)
{
	m_pScrollObject = sobject;
}