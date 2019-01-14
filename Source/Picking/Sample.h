#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "Mesh.h"
#include "GeometryGenerator.h"
#include "Computeshader.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;

	void Pick(int sx, int sy);

	virtual void OnResize()override;
	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;

private:
	std::unique_ptr<Computeshader> mSobelFilter = nullptr;
	std::unique_ptr<DxRT> mOffRT = nullptr;
	Mesh mesh;

	bool isPicking = false;
};

