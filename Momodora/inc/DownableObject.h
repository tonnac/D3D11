#pragma once
#include "Terrain.h"

class DownableObject : public Terrain
{
public:
//	COL		Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize) override;
	COL		Collision(std::shared_ptr<Character> pObject) override;
};