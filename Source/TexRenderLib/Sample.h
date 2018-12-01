#pragma once

#include "Core.h"
#include "Shape.h"
#include "Minimap.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	bool Init();
	bool Frame();
	bool Render();

private:
	Minimap m_Minimap;

	BoxShape m_Box;
};
