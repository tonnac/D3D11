#pragma once

#include "Object.h"
#include "ZXCBinLoader.h"
#include "ClipBinLoader.h"
#include "FrameResource.h"
#include <cstdarg>

class Mesh : public Object<Vertex>
{
public:
	bool LoadFile(const std::tstring& filename, const std::tstring& texfilepath = std::tstring(), ID3D11Device * device = nullptr);

	virtual bool Frame()override;
	virtual bool Render(ID3D11DeviceContext* context)override;
	virtual void SetWorld(DirectX::FXMMATRIX world)override;
	virtual void SetWorld(const DirectX::XMFLOAT4X4& world)override;

	virtual bool Intersects(DirectX::FXMVECTOR& origin, DirectX::FXMVECTOR& dir, DirectX::CXMMATRIX& invView, float& tmin)override;

	bool DebugRender(ID3D11DeviceContext* context);

private:
	bool LoadZXC(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadZXCBin(const std::tstring& filename, const std::tstring& texfilepath);

	bool LoadSkin(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadSkinBin(const std::tstring& filename, const std::tstring& texfilepath);

	bool LoadClip(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadClipBin(const std::tstring& filename, const std::tstring& texfilepath);

	template<typename X>
	void Initialize(
		std::vector<X>& vertices,
		std::vector<DWORD>& indices,
		const std::vector<ZXCLoader::Subset> subsets,
		const std::vector<ZXCSMaterial> materials,
		const std::vector<MeshNode> nodes,
		const std::wstring& texFile,
		const std::wstring& texFilePath)
	{
		BuildMaterials(texFilePath, materials);

		std::wstring name(texFile, 0, texFile.find_last_of('.'));

		if (S_Geometry[name] != nullptr)
		{
			mGeometry = S_Geometry[name];
		}
		else
		{
			std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
			geo->Name = name;
			mGeometry = geo.get();
			S_Geometry.SaveGeometry(geo);

			const UINT vbByteSize = (UINT)vertices.size() * sizeof(X);
			const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

			BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize, sizeof(X));
		}

		mNodeList.resize(nodes.size());
		for (UINT i = 0; i < (UINT)nodes.size(); ++i)
		{
			mNodeList[i] = std::make_unique<MeshNode>();
			*(mNodeList[i].get()) = nodes[i];
		}

		BuildRenderItem(subsets, materials);
	}

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