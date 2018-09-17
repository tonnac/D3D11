#include "TrapObject.h"
#include "mSound.h"

bool TrapObject::Collision(Object* pObject)
{
	CharacterObject * pl = dynamic_cast<CharacterObject*>(pObject);
	RECT ObjRT = *(pObject->getCollisionRt());
	POINT Center;
	Center.x = (ObjRT.right + ObjRT.left) / 2;
	Center.y = (ObjRT.bottom + ObjRT.top) / 2;
	
	LONG xDiff = static_cast<LONG>(abs(Center.x - m_CenterPos.x));
	LONG yDiff = static_cast<LONG>(abs(Center.y - m_CenterPos.y));

	if (xDiff < (ObjRT.right - Center.x) + ((m_rtCollision.right - m_CenterPos.x)) &&
		yDiff < (ObjRT.bottom - Center.y) + ((m_rtCollision.bottom - m_CenterPos.y)))
	{
		if (pl->getHP() <= 0)
		{
			return true;
		}
		pl->setHP(100);
		S_Sound.Play(Effect::HURT);
		pl->setState(L"Hurt");
	}
	return false;
}