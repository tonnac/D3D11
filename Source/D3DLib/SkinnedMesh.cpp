#include "SkinnedMesh.h"

bool SkinnedMesh::LoadFile(const std::tstring & filename, const std::tstring & texfilepath)
{
	std::tstring Ext(filename, filename.find_last_of(L".") + 1, filename.length());

	std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);

	if (Ext == L"SKN")
	{
		return LoadSkin(filename, texfilepath);
	}
	else if (Ext == L"SBI")
	{
		return LoadSkinBin(filename, texfilepath);
	}
	else if (Ext == L"CLP")
	{
		return LoadClip(filename, texfilepath);
	}
	else if (Ext == L"CLB")
	{
		return LoadClipBin(filename, texfilepath);
	}
	return false;
}

bool SkinnedMesh::Frame()
{
	mSkinnedInst->UpdateSkinnedAnimation(g_fSecPerFrame);

	std::copy(
		std::begin(mSkinnedInst->FinalTransforms),
		std::end(mSkinnedInst->FinalTransforms),
		&mSkinnedConstants.BoneTransforms[0]
	);
	return true;
}

bool SkinnedMesh::Render(ID3D11DeviceContext * context)
{
	context->UpdateSubresource(mConstantbuffer.Get(), 0, nullptr, &mSkinnedConstants, 0, 0);
	context->VSSetConstantBuffers(3, 1, mConstantbuffer.GetAddressOf());
	return Mesh<SkinnedVertex>::Render(context);
}

bool SkinnedMesh::LoadSkin(const std::tstring & filename, const std::tstring & texfilepath)
{
	FileInfo fileInfo = CreateFileInfo(filename, texfilepath);

	ZXCLoader loader(fileInfo);

	if (!loader.LoadSkin())
		return false;

	mSkinnedInst = std::make_unique<SkinnedModelInstance>();
	mSkinnedInst->FinalTransforms.resize(fileInfo.Nodes.size());
	mSkinnedInst->SkinnedInfo = mSkinInfo.get();
	mSkinnedInst->ClipName = L"default";

	Initialize(fileInfo);

	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(SkinnedConstants), mConstantbuffer.GetAddressOf());

	return true;
}

bool SkinnedMesh::LoadSkinBin(const std::tstring & filename, const std::tstring & texfilepath)
{
	FileInfo fileInfo = CreateFileInfo(filename, texfilepath);

	ZXCBinLoader loader(fileInfo);

	if (!loader.LoadBinary<SkinnedVertex>())
		return false;

	mSkinnedInst = std::make_unique<SkinnedModelInstance>();
	mSkinnedInst->FinalTransforms.resize(fileInfo.Nodes.size());
	mSkinnedInst->SkinnedInfo = mSkinInfo.get();
	mSkinnedInst->ClipName = L"default";

	Initialize(fileInfo);

	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(SkinnedConstants), mConstantbuffer.GetAddressOf());

	return true;
}

bool SkinnedMesh::LoadClip(const std::tstring & filename, const std::tstring & texfilepath)
{
	std::unordered_map<std::wstring, AnimationClip> clips;

	ClipLoader loader;
	loader.LoadClip(filename, GetNodeSize(), clips);

	std::wstring name;
	for (auto& x : clips)
	{
		name = x.first;

		auto& p = std::make_pair(x.first, x.second);
		mSkinInfo->AddAnimation(p);
		mSkinnedInst->Animations.push_back(name);
	}

	mSkinnedInst->setClipName(name);
	return true;
}

bool SkinnedMesh::LoadClipBin(const std::tstring & filename, const std::tstring & texfilepath)
{
	std::unordered_map<std::wstring, AnimationClip> clips;

	ClipBinLoader loader;
	loader.LoadClip(filename, GetNodeSize(), clips);

	std::wstring name;
	for (auto& x : clips)
	{
		name = x.first;

		auto& p = std::make_pair(name, x.second);
		mSkinInfo->AddAnimation(p);
		mSkinnedInst->Animations.push_back(name);
	}

	mSkinnedInst->setClipName(name);

	return true;
}

FileInfo SkinnedMesh::CreateFileInfo(const std::tstring & filename, const std::tstring & texfilepath)
{
	mSkinInfo = std::make_unique<SkinnedData>();

	FileInfo fileInfo;
	fileInfo.FileName = filename;
	fileInfo.TexPath = texfilepath;
	fileInfo.skinInfo = mSkinInfo.get();

	return fileInfo;
}
