#pragma once
#include "Terrain.h"

class DownableObject : public Terrain
{
public:
	COL		Collision(std::shared_ptr<Player> pPlayer) override;
};