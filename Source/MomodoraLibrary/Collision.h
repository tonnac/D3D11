#pragma once
#include "Std.h"

class CollisionClass
{
public:
	static bool					RectInPt(RECT, POINT);
	static bool					RectInRect(RECT, RECT);
	static bool					RectInRect(RECT, FloatRect);
	static bool					SphereInPt(RECT, POINT);
	static bool					SphereInSphere(RECT, RECT);
private:
};