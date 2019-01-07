#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "Mesh.h"
#include "GeometryGenerator.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);
	Sample(HINSTANCE hInstance, HWND hWnd, RECT rt);

public:

	void setWireFrame();
	void setBackColor(float color[4]);
	void setSkyBox();

protected:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;
private:
	GridShape grid;
	Mesh mesh;
	Mesh mesh0;
};
