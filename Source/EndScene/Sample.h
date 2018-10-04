#pragma once
#include "SceneMgr.h"
#include "EndScene.h"
class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	EndScene			m_Empty;
};