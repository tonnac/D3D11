#pragma once
#include "Core.h"

class Sample : public Core
{
public:
	Sample();
public:
	bool		Init();
	bool		Render();
	bool		Release();
};