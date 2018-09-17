#pragma once
#include "Core.h"
#include "PlaneObject.h"
#include "TriangleObject.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	PlaneObject			m_object;
};