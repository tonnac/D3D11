#pragma once

#include "ZXCBinLoader.h"
#include "ClipBinLoader.h"
#include "Shape.h"
#include "FrameResource.h"

class Mesh : public Shape
{
public:
	bool LoadFile(const std::tstring& filename, const std::tstring& texfilepath = std::tstring(), ID3D11Device * device = nullptr);

	bool Frame();
	bool Render(ID3D11DeviceContext* context);
	void SetWorld(DirectX::FXMMATRIX world);
	void SetWorld(const DirectX::XMFLOAT4X4& world);

private:
	bool LoadZXC(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadSkin(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadZXCBin(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadSkinBin(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadClip(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadClipBin(const std::tstring& filename, const std::tstring& texfilepath);

	void Initialize(
		std::vector<Vertex> vertices,
		std::vector<DWORD> indices, 
		const std::vector<ZXCLoader::Subset> subsets,
		const std::vector<ZXCSMaterial> materials, 
		const std::vector<MeshNode> nodes,
		const std::wstring& texFile,
		const std::wstring& texFilePath);

	void Initialize(
		std::vector<SkinnedVertex> vertices,
		std::vector<DWORD> indices,
		const std::vector<ZXCLoader::Subset> subsets,
		const std::vector<ZXCSMaterial> materials,
		const std::vector<MeshNode> nodes,
		const std::wstring& texFile,
		const std::wstring& texFilePath);

	void BuildRenderItem(const std::vector<ZXCLoader::Subset>& subsets,
		const std::vector<ZXCSMaterial>& materials);
	void BuildMaterials(const std::tstring& texfilepath, const std::vector<ZXCSMaterial>& materials);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantbuffer = nullptr;
	std::vector<std::unique_ptr<MeshNode>> mNodeList;
	std::vector<RenderItem*> mDrawItem;
	std::vector<RenderItem*> mDebugItem;

	std::unique_ptr<SkinnedData> mSkinInfo = nullptr;
	std::unique_ptr<SkinnedModelInstance> mSkinnedInst = nullptr;
	SkinnedConstants mSkinnedConstants;
};