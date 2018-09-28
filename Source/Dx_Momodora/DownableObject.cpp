#include "DownableObject.h"
#include "Player.h"

COL	DownableObject::Collision(std::shared_ptr<Player> pPlayer)
{
	FLOAT Size;
	COL col = Plane_Object::Collision(pPlayer, &Size);
	D2D1_RECT_F Colrt = pPlayer->getCollisionRT();


	if (col == COL::TOP)
	{
		if (m_rtCollision.bottom > Colrt.bottom && (pPlayer->getCurrentState() != L"Jump" || pPlayer->getCurrentState() != L"Jump2") && 
			(pPlayer->getCurrentState() != L"LadderEnter" && pPlayer->getCurrentState() != L"LadderLeave" &&
				pPlayer->getCurrentState() != L"LadderUp" && pPlayer->getCurrentState() != L"LadderDown"))
		{
			pPlayer->MoveCenterPos({ 0.0f, -Size });
			pPlayer->setDown(true);
			return col;
		}
	}

	D2D1_RECT_F pcol = pPlayer->getCollisionRT();
	D2D1_RECT_F dCol = { pcol.left, pcol.bottom, pcol.right, pcol.bottom + 15.0f };
	D3DXVECTOR2 dColcen = { (dCol.right + dCol.left) * 0.5f, (dCol.bottom + dCol.top) * 0.5f };


	if (pPlayer->getCurrentState() != L"Fall" &&
		(dCol.right - dColcen.x) + m_rtCollision.right - m_Centerpos.x >= abs(m_Centerpos.x - dColcen.x) &&
		(dCol.bottom - dColcen.y) + m_rtCollision.bottom - m_Centerpos.y >= abs(m_Centerpos.y - dColcen.y))
	{
		return COL::TOP;
	}
	else
	{
		return COL::NONE;
	}
}