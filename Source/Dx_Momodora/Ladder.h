#pragma once
#include "Terrain.h"
#include "Player.h"

class Ladder : public Terrain
{
public:
	COL		Collision(std::shared_ptr<Player> pPlayer) override;
};