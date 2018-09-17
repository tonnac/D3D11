#include "Collision.h"


bool CollisionClass::RectInPt(RECT rt, POINT pt)
{
	if (pt.x <= rt.right && pt.x >= rt.left)
	{
		if (pt.y <= rt.bottom && pt.y >= rt.top)
		{
			return true;
		}
	}
	return false;
}
bool CollisionClass::RectInRect(RECT A_rt, RECT B_rt)
{
	POINT A_Center, B_Center;
	A_Center.x = (A_rt.right + A_rt.left) / 2;
	A_Center.y = (A_rt.bottom + A_rt.top) / 2;

	B_Center.x = (B_rt.right + B_rt.left) / 2;
	B_Center.y = (B_rt.bottom + B_rt.top) / 2;

	LONG xDiff = abs(A_Center.x - B_Center.x);
	LONG yDiff = abs(A_Center.y - B_Center.y);

	if (xDiff <= (A_rt.right - A_Center.x) + (B_rt.right - B_Center.x) &&
		yDiff <= (A_rt.bottom - A_Center.y) + (B_rt.bottom - B_Center.y))
		return true;

	return false;
}
bool CollisionClass::RectInRect(RECT A_rt, FloatRect B_rt)
{
	POINT A_Center, B_Center;
	A_Center.x = (A_rt.right + A_rt.left) / 2;
	A_Center.y = (A_rt.bottom + A_rt.top) / 2;

	B_Center.x = (B_rt.right + B_rt.left) / 2;
	B_Center.y = (B_rt.bottom + B_rt.top) / 2;

	LONG xDiff = abs(A_Center.x - B_Center.x);
	LONG yDiff = abs(A_Center.y - B_Center.y);

	if (xDiff <= (A_rt.right - A_Center.x) + (B_rt.right - B_Center.x) &&
		yDiff <= (A_rt.bottom - A_Center.y) + (B_rt.bottom - B_Center.y))
		return true;

	return false;
}
bool CollisionClass::SphereInPt(RECT rt, POINT pt)
{
	Sphere rtSphere;
	rtSphere.CenterPos.x = (rt.right + rt.left) / 2;
	rtSphere.CenterPos.y = (rt.bottom + rt.top) / 2;
	
	LONG diffX = (rt.right - rt.left) / 2;
	LONG diffY = (rt.bottom - rt.top) / 2;

	rtSphere.fRadius = (diffX > diffY) ? static_cast<float>(diffX) : static_cast<float>(diffY);
	
	float diffpt = static_cast<float>(sqrt((pt.x - rtSphere.CenterPos.x) *(pt.x - rtSphere.CenterPos.x) +
		(pt.y - rtSphere.CenterPos.y) * (pt.y - rtSphere.CenterPos.y)));

	if (diffpt <= rtSphere.fRadius)
	{
		return true;
	}
	return false;
}
bool CollisionClass::SphereInSphere(RECT A_rt, RECT B_rt)
{
	Sphere A_Sphere, B_Sphere;

	A_Sphere.CenterPos.x = (A_rt.right + A_rt.left) / 2;
	A_Sphere.CenterPos.y = (A_rt.bottom + A_rt.top) / 2;

	B_Sphere.CenterPos.x = (B_rt.right + B_rt.left) / 2;
	B_Sphere.CenterPos.y = (B_rt.bottom + B_rt.top) / 2;

	LONG diffX = (A_rt.right - A_rt.left) / 2;
	LONG diffY = (A_rt.bottom - A_rt.top) / 2;
	A_Sphere.fRadius = (diffX > diffY) ? static_cast<float>(diffX) : static_cast<float>(diffY);

	diffX = (B_rt.right - B_rt.left) / 2;
	diffY = (B_rt.bottom - B_rt.top) / 2;
	B_Sphere.fRadius = (diffX > diffY) ? static_cast<float>(diffX) : static_cast<float>(diffY);


	float diffpt = static_cast<float>(sqrt((A_Sphere.CenterPos.x - B_Sphere.CenterPos.x) * (A_Sphere.CenterPos.x - B_Sphere.CenterPos.x) +
		(A_Sphere.CenterPos.y - B_Sphere.CenterPos.y) * (A_Sphere.CenterPos.y - B_Sphere.CenterPos.y)));

	if (diffpt <= (A_Sphere.fRadius + B_Sphere.fRadius))
	{
		return true;
	}
	return false;
}