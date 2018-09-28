#include "DownableObject.h"
#include "Player.h"

//COL	DownableObject::Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize)
//{
//	FLOAT Size;
//	COL col = Plane_Object::Collision(pObject, &Size);
//	return col;
//}
COL	DownableObject::Collision(std::shared_ptr<Character> pObject)
{
	FLOAT Size;
	COL col = Plane_Object::Collision(pObject, &Size);
	D2D1_RECT_F Colrt = pObject->getCollisionRT();

	if (col == COL::TOP)
	{
		if (m_rtCollision.bottom > Colrt.bottom && (pObject->getCurrentState() != L"Jump" || pObject->getCurrentState() != L"Jump2"))
		{
			pObject->MoveCenterPos({ 0.0f, -Size });
			Character* pCharacter = pObject.get();
			Player* pPlayer = dynamic_cast<Player*>(pCharacter);
			if (pPlayer != nullptr)
			{
				pPlayer->setDown(true);
			}
			return col;
		}
	}

	D2D1_RECT_F pcol = pObject->getCollisionRT();
	D2D1_RECT_F dCol = { pcol.left, pcol.bottom, pcol.right, pcol.bottom + 15.0f };
	D3DXVECTOR2 dColcen = { (dCol.right + dCol.left) * 0.5f, (dCol.bottom + dCol.top) * 0.5f };


	if (pObject->getCurrentState() != L"Fall" &&
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

//bool DownableObject::Collision(Object* pObject)
//{
//	RECT CollisionArea;
//	RECT ObjRT = *(pObject->getCollisionRt());
//	POINT Center;
//	Center.x = (ObjRT.right + ObjRT.left) / 2;
//	Center.y = (ObjRT.bottom + ObjRT.top) / 2;
//
//	RECT crt = *pObject->getCollisionRt();
//
//	LONG xDiff = static_cast<LONG>(abs(Center.x - m_CenterPos.x));
//	LONG yDiff = static_cast<LONG>(abs(Center.y - m_CenterPos.y));
//
//	if (xDiff < (ObjRT.right - Center.x) + ((m_rtCollision.right - m_CenterPos.x)) &&
//		yDiff < (ObjRT.bottom - Center.y) + ((m_rtCollision.bottom - m_CenterPos.y)))
//	{
//		CollisionArea.left = (ObjRT.left < m_rtCollision.left) ? m_rtCollision.left : ObjRT.left;
//		CollisionArea.right = (ObjRT.right > m_rtCollision.right) ? m_rtCollision.right : ObjRT.right;
//		CollisionArea.top = (ObjRT.top < m_rtCollision.top) ? m_rtCollision.top : ObjRT.top;
//		CollisionArea.bottom = (ObjRT.bottom > m_rtCollision.bottom) ? m_rtCollision.bottom : ObjRT.bottom;
//		if (crt.bottom < m_rtCollision.bottom)
//		{
//			return MoveObject(pObject, CollisionArea);
//		}
//	}
//	return false;
//}
//bool DownableObject::MoveObject(Object* pObject, const RECT& CollisionArea)
//{
//	LONG lWidth = CollisionArea.right - CollisionArea.left;
//	LONG lHeight = CollisionArea.bottom - CollisionArea.top;
//	FloatPoint pObjCenterPos = *(pObject->getCenterPos());
//
//	CharacterObject * pl = dynamic_cast<CharacterObject*>(pObject);
//
//	bool flag = pl->isFallState();
//	if (flag == true)
//	{
//		pObject->setCenterPos_y(pObjCenterPos.y - lHeight + 7);
//	}
//	else
//	{
//		if (pl->getCurrentState() == "LadderEnter" || pl->getCurrentState() == "LadderLeave" ||
//			pl->getCurrentState() == "LadderUp" || pl->getCurrentState() == "LadderDown")
//			return true;
//		pObject->setCenterPos_y(pObjCenterPos.y - lHeight);
//	}
//	pl->setDownable(true);
//	return true;
//}