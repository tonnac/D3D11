#pragma once
#include "Plane_Object.h"


class UI : public Plane_Object
{
public:
	virtual ~UI();
public:
	virtual void SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom);
private:

};

using UIPTR = std::shared_ptr<UI>;
