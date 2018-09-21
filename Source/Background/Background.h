#pragma once
#include "Plane_Object.h"

class Background : public Plane_Object
{
public:
	Background() = default;
public:
	void SetPos(const D2D1_POINT_2F& pos, const D2D1_RECT_F& rect) override;
public:
	bool Frame() override;
private:
};