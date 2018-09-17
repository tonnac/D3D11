#include "ScrollObject.h"

bool ScrollObject::Init()
{
	return true;
}
bool ScrollObject::Frame()
{
	m_CenterPos.x = m_DrawPos.x + (m_rtDraw.right / 2);
	m_CenterPos.y = m_DrawPos.y + (m_rtDraw.bottom / 2);

	m_rtCollision.left = static_cast<LONG>(m_DrawPos.x);
	m_rtCollision.top = static_cast<LONG>(m_DrawPos.y);
	m_rtCollision.right = static_cast<LONG>(m_DrawPos.x + m_rtDraw.right);
	m_rtCollision.bottom = static_cast<LONG>(m_DrawPos.y + m_rtDraw.bottom);
	return true;
}
bool ScrollObject::Render()
{
	int prevpen = SetROP2(g_hOffScreenDC, R2_MASKPEN);

	Rectangle(g_hOffScreenDC, m_rtCollision.left, m_rtCollision.top,
		m_rtCollision.right, m_rtCollision.bottom);

	SetROP2(g_hOffScreenDC, prevpen);
	return true;
}
bool ScrollObject::Release()
{
	return true;
}
bool ScrollObject::Collision(Object* pObject)
{
	return false;
}
bool ScrollObject::MoveObject(Object* pObject, const RECT& CollisionArea)
{
	return true;
}

void ScrollObject::ReverseSet(const LONG& drawright)
{
	m_DrawPos.x -= drawright;
}