#include "UI.h"

bool UI::MoveScrollObj(const LONG& fsize)
{
	if (fsize < 0)
	{

		m_CenterPos.x += fsize;
	}
	else
	{
		m_CenterPos.x -= fsize;
	}
	return true;
}