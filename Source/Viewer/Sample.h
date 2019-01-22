#pragma once

#include "Core.h"
#include "Minimap.h"
#include "Frustum.h"
#include "SkinnedMesh.h"
#include "GeometryGenerator.h"
#include "SobelFilter.h"

class Sample : public Core
{
public:
	Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

protected:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;

private:
	std::unique_ptr<SkinnedMesh> mesh0;
	std::unique_ptr<Mesh<>> mesh1;
	std::unique_ptr<Mesh<>> mesh2;
};

