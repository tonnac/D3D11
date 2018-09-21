#pragma once
#include "Core.h"
#include "Background.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	Background			m_Background;
};