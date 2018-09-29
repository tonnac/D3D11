#pragma once
#include "SceneMgr.h"
#include "LobbyMenu.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	Lobbymenu			m_Lobby;
};