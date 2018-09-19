#pragma once
#include "Core.h"
#include "PlaneObject.h"
#include "TriangleObject.h"
#include "Button.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
private:
	Button				m_object;
};