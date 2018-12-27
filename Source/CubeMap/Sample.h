#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "Mesh.h"

/*	To do
	{
		Dxobj추가 Rasterizer, Blendstate, DepthstencilState
		ShaderStroage 추가,
		Mesh Manager(?) 보류,
		SkyBox,
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
	Mesh mesh;
};
