#pragma once

#include "SkinnedData.h"
#include "Shape.h"
#include "ZXCLoader.h"



class Mesh : public Shape
{
public:
	bool LoadFile(const std::tstring& filename, ID3D11Device * device);

	bool Frame();
	bool Render(ID3D11DeviceContext* context);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantbuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mBufferview = nullptr;
	std::vector<std::unique_ptr<MeshNode>> mNodeList;
	std::vector<RenderItem*> mDrawItem;
	std::vector<RenderItem*> mDebugItem;
	std::unique_ptr<SkinnedModelInstance> mSkinnedInst = nullptr;
};