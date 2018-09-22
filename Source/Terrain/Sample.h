#pragma once
#include "Core.h"
#include "Background.h"
#include "Terrain.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	std::vector<Terrain*> m_TerrainList;
	Background			m_Background;
	Terrain				m_Terrain;
};