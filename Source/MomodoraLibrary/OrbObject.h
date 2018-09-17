#pragma once
#include "TerrainObject.h"

class OrbObject : public TerrainObject
{
public:
	OrbObject();
public:
	bool		Collision(Object*) override;
	bool		isPlatUP() override;
	bool		Init() override;
	bool		Frame() override;
	bool		Render() override;
	bool		Release() override;
private:
	bool		m_bPlatUp;
};