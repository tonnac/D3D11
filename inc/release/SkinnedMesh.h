#pragma once

#include "Mesh.h"

class SkinnedMesh : public Mesh<SkinnedVertex>
{
//	using Mesh<SkinnedVertex>::Render;
	using Mesh<SkinnedVertex>::Frame;
	using Mesh<SkinnedVertex>::Initialize;
	using Mesh<SkinnedVertex>::GetNodeSize;
	using Mesh<SkinnedVertex>::m_pDevice;

public:
	SkinnedMesh(ID3D11Device * device) : Mesh<SkinnedVertex>(device)
	{}

public:
	virtual bool LoadFile(const std::tstring& filename, const std::tstring& texfilepath = std::tstring())override;

	virtual bool Frame();
	virtual bool Render(ID3D11DeviceContext* context)override;

private:
	bool LoadSkin(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadSkinBin(const std::tstring& filename, const std::tstring& texfilepath);

	bool LoadClip(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadClipBin(const std::tstring& filename, const std::tstring& texfilepath);

	virtual FileInfo CreateFileInfo(const std::tstring& filename, const std::tstring& texfilepath)override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantbuffer = nullptr;

	std::unique_ptr<SkinnedData> mSkinInfo = nullptr;
	std::unique_ptr<SkinnedModelInstance> mSkinnedInst = nullptr;
	SkinnedConstants mSkinnedConstants;
};