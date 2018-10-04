#pragma once
#include "SceneMgr.h"
#include "MiscSet.h"
class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	MiscSet				m_Misc;
};