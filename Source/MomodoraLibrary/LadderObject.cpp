#include "LadderObject.h"

bool LadderObject::Collision(Object* pObject)
{
	RECT CollisionArea;
	RECT ObjRT = *(pObject->getCollisionRt());
	POINT Center;
	Center.x = (ObjRT.right + ObjRT.left) / 2;
	Center.y = (ObjRT.bottom + ObjRT.top) / 2;

	CharacterObject * pl = dynamic_cast<CharacterObject*>(pObject);

	LONG xDiff = static_cast<LONG>(abs(Center.x - m_CenterPos.x));
	LONG yDiff = static_cast<LONG>(abs(Center.y - m_CenterPos.y));

	if (xDiff < (ObjRT.right - Center.x) + ((m_rtCollision.right - m_CenterPos.x)) &&
		yDiff < (ObjRT.bottom - Center.y) + ((m_rtCollision.bottom - m_CenterPos.y)))
	{

		if (pl->getCurrentState() == "LadderUp" && m_rtCollision.top >= ObjRT.top)
		{
			pl->setState(L"LadderLeave");
			pl->setCenterPos_y(m_rtCollision.top);
			return true;
		}
		if (pl->getCurrentState() == "LadderDown" && m_rtCollision.bottom <= ObjRT.bottom)
		{
			pl->setState(L"Idle");
			return true;
		}
		CollisionArea.left = (ObjRT.left < m_rtCollision.left) ? m_rtCollision.left : ObjRT.left;
		CollisionArea.right = (ObjRT.right > m_rtCollision.right) ? m_rtCollision.right : ObjRT.right;
		CollisionArea.top = (ObjRT.top < m_rtCollision.top) ? m_rtCollision.top : ObjRT.top;
		CollisionArea.bottom = (ObjRT.bottom > m_rtCollision.bottom) ? m_rtCollision.bottom : ObjRT.bottom;
		pl->setLadder(true);
		return true;
	}
	pl->setLadder(false);
	return false;
}
void LadderObject::ObjectDraw(Object*pObject)
{
	CharacterObject * pl = dynamic_cast<CharacterObject*>(pObject);
	pl->setCenterPos_x((m_rtCollision.left + m_rtCollision.right) / 2);
	pl->setCenterPos_y((m_rtCollision.top));
}