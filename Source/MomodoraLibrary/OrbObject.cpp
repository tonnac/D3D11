#include "OrbObject.h"
#include "Rendering.h"

OrbObject::OrbObject() : m_bPlatUp(false)
{}


bool OrbObject::Init()
{
	Object::Init();
	return true;
}
bool OrbObject::Frame()
{
	Object::Frame();
	return true;
}
bool OrbObject::Render()
{
	Object::Render();
	return true;
}
bool OrbObject::Release()
{
	if (m_pRendering)
	{
		m_pRendering->Release();
		delete m_pRendering;
	}
	m_pRendering = nullptr;
	return true;
}

bool OrbObject::Collision(Object* pObject)
{
	CharacterObject * pCharObj = dynamic_cast<CharacterObject*>(pObject);
	while (pCharObj->hasNext())
	{
		EffectIter it = pCharObj->getEffectIter();
		RECT itrt = *(*it)->getCollisionRt();
		if (CollisionClass::RectInRect(itrt, m_rtCollision))
		{
			pCharObj->deleteEffect(it);
			m_rtDraw = { 56,0,56,61 };
			m_bPlatUp = true;
		}
	}
	RECT CollisionArea;
	RECT ObjRT = *(pObject->getCollisionRt());
	POINT Center;
	Center.x = (ObjRT.right + ObjRT.left) / 2;
	Center.y = (ObjRT.bottom + ObjRT.top) / 2;

	RECT crt = *pObject->getCollisionRt();

	LONG xDiff = static_cast<LONG>(abs(Center.x - m_CenterPos.x));
	LONG yDiff = static_cast<LONG>(abs(Center.y - m_CenterPos.y));

	if (xDiff < (ObjRT.right - Center.x) + ((m_rtCollision.right - m_CenterPos.x)) &&
		yDiff < (ObjRT.bottom - Center.y) + ((m_rtCollision.bottom - m_CenterPos.y)))
	{
		CollisionArea.left = (ObjRT.left < m_rtCollision.left) ? m_rtCollision.left : ObjRT.left;
		CollisionArea.right = (ObjRT.right > m_rtCollision.right) ? m_rtCollision.right : ObjRT.right;
		CollisionArea.top = (ObjRT.top < m_rtCollision.top) ? m_rtCollision.top : ObjRT.top;
		CollisionArea.bottom = (ObjRT.bottom > m_rtCollision.bottom) ? m_rtCollision.bottom : ObjRT.bottom;
		if (crt.bottom < m_rtCollision.bottom)
		{
			return MoveObject(pObject, CollisionArea);
		}
	}
	return false;
}
bool OrbObject::isPlatUP()
{
	return m_bPlatUp;
}