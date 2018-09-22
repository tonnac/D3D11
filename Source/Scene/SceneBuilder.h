#pragma once
#include "GameScene.h"

class SceneBuilder
{
public:
	bool	Initialize(const std::tstring& ScriptPath);
private:
	std::tifstream	m_Script;
};

