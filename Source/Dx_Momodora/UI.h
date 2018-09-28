#pragma once
#include "Plane_Object.h"


class UI : public Plane_Object
{
public:
	virtual ~UI();
private:

};

using UIPTR = std::shared_ptr<UI>;
