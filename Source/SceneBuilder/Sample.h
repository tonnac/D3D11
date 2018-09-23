#pragma once
#include "Core.h"
#include "SceneMgr.h"
#include "SceneBuilder.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
};