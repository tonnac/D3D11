#pragma once
#include "TerrainObject.h"

class LadderObject : public TerrainObject
{
public:
	bool		Collision(Object*) override;
	void		ObjectDraw(Object*);
private:
};