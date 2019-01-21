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

	virtual void OnResize()override;

private:
	std::unique_ptr<SkinnedMesh> mesh0;
	std::unique_ptr<SkinnedMesh> mesh1;
	std::unique_ptr<SobelFilter> mSobelfilter;
	std::unique_ptr<DxRT> mDxRT;
	GridShape grid;
//	std::unique_ptr<Mesh<>> mesh;

	bool misSobel = false;
};

