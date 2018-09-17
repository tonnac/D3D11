#pragma once
#include "TerrainObject.h"

class DownableObject : public TerrainObject
{
public:
	bool		Collision(Object*) override;
	bool		MoveObject(Object*, const RECT&) override;
};