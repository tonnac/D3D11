#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"

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

	Frustum mFrustum;
};
