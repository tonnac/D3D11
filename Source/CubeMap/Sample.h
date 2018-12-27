#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "Mesh.h"

/*	To do
	{
		Dxobj�߰� Rasterizer, Blendstate, DepthstencilState
		ShaderStroage �߰�,
		Mesh Manager(?) ����,
		SkyBox,
		<Material>, Lights
		Mesh Load �Լ�ȭ(?)
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
