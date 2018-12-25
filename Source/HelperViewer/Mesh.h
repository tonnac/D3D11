#pragma once

#include "HelperViewer.h"
#include "Shape.h"

class Mesh : public Shape
{
public:
	bool LoadFile(const std::tstring& filename, ID3D11Device * device, bool isDebug = false);
	bool LoadZXC(const std::tstring& filename, ID3D11Device * device, bool isDebug);
	bool LoadZXCS(const std::tstring& filename, ID3D11Device * device, bool isDebug);

	bool Frame();
	bool Render(ID3D11DeviceContext* context);

private:
	virtual void CreateInputLayout(ID3DBlob * vertexblob)override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantbuffer = nullptr;
	std::vector<std::unique_ptr<MeshNode>> mNodeList;
	std::vector<RenderItem*> mDrawItem;

	std::unique_ptr<SkinnedData> mSkinInfo = nullptr;
	std::unique_ptr<SkinnedModelInstance> mSkinnedInst = nullptr;
	SkinnedConstants mSkinnedConstants;

	bool misDebug = false;
	std::unique_ptr<HelperViewer> mHelperViewer = nullptr;
};