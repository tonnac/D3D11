#pragma once
#include "Core.h"
#include "SceneMgr.h"
#include "Character.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	Character			m_Character;
};