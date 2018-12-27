#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "Mesh.h"
#include "GeometryGenerator.h"

/*	To do
	{
		Mesh Manager(?) 보류,
		<Material>, Lights
		Mesh Load 함수화(?)
	}
*/


class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;
private:
	GridShape grid;
	Mesh mesh;
};
