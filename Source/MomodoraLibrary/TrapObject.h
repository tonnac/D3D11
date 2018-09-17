#pragma once
#include "TerrainObject.h"

class TrapObject : public TerrainObject
{
public:
	bool		Collision(Object*) override;
private:
};