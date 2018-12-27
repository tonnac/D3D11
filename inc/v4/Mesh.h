#pragma once

#include "ZXCLoader.h"
#include "Shape.h"

class Mesh : public Shape
{
public:
	bool LoadFile(const std::tstring& filename, ID3D11Device * device);

	bool Frame();
	bool Render(ID3D11DeviceContext* context);
	void SetWorld(DirectX::FXMMATRIX world);
	void SetWorld(const DirectX::XMFLOAT4X4& world);
private:

	bool LoadZXC(const std::tstring& filename);
	bool LoadZXCS(const std::tstring& filename);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantbuffer = nullptr;
	std::vector<std::unique_ptr<MeshNode>> mNodeList;
	std::vector<RenderItem*> mDrawItem;
	std::vector<RenderItem*> mDebugItem;

	std::unique_ptr<SkinnedData> mSkinInfo = nullptr;
	std::unique_ptr<SkinnedModelInstance> mSkinnedInst = nullptr;
	SkinnedConstants mSkinnedConstants;
};