#include "ZXCLoader.h"
#include "DxState.h"
#include "Mesh.h"

using namespace DirectX;

ZXCLoader::ZXCLoader(FileInfo & fileInfo)
	: mFileinfo(fileInfo)
{
}

bool ZXCLoader::LoadZXC()
{
	LoadCommon();
	return true;
}

bool ZXCLoader::LoadSkin()
{
	LoadCommon<SkinnedVertex>();
	BuildDefaultAnimaions();
	return true;
}

void ZXCLoader::ReadCommon(std::wifstream& fp)
{
	std::wstring ignore;

	std::getline(fp, ignore);
	std::getline(fp, ignore);
	std::getline(fp, ignore);

	fp >> ignore >> mNumMaterials;
	fp >> ignore >> mNumNodes;
	fp >> ignore >> mNumVertices;
	fp >> ignore >> mNumTriangles;
	fp >> ignore >> mNumSubSet;

	mFileinfo.Materials.resize(mNumMaterials);
	mFileinfo.Nodes.resize(mNumNodes);
	mFileinfo.Subsets.resize(mNumSubSet);
	mFileinfo.Vertices.resize(mNumVertices);
	mFileinfo.Indices.resize(mNumTriangles * 3);

	ReadMaterial(fp);
	ReadNodes(fp);
	ReadSubsetTable(fp);
	ReadBoundingBox(fp);
}

void ZXCLoader::ReadMaterial(std::wifstream& fp)
{
	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < mNumMaterials; ++i)
	{
		auto& materials = mFileinfo.Materials;

		UINT rootMapNum;
		UINT subMtlNum;
		fp >> ignore >> materials[i].Name;
		fp >> ignore >> materials[i].Diffuse.x >> materials[i].Diffuse.y >> materials[i].Diffuse.z;
		fp >> ignore >> materials[i].Specular.x >> materials[i].Specular.y >> materials[i].Specular.z;
		fp >> ignore >> materials[i].Ambient.x >> materials[i].Ambient.y >> materials[i].Ambient.z;
		fp >> ignore >> materials[i].Shininess;
		fp >> ignore >> rootMapNum;
		fp >> ignore >> subMtlNum;

		materials[i].SubMaterial.resize(subMtlNum);
		for (UINT j = 0; j < subMtlNum; ++j)
		{
			UINT subMapNum;
			UINT subNo;

			fp >> ignore >> materials[i].SubMaterial[j].Name;
			fp >> ignore >> materials[i].SubMaterial[j].Diffuse.x >> materials[i].SubMaterial[j].Diffuse.y >> materials[i].SubMaterial[j].Diffuse.z;
			fp >> ignore >> materials[i].SubMaterial[j].Specular.x >> materials[i].SubMaterial[j].Specular.y >> materials[i].SubMaterial[j].Specular.z;
			fp >> ignore >> materials[i].SubMaterial[j].Ambient.x >> materials[i].SubMaterial[j].Ambient.y >> materials[i].SubMaterial[j].Ambient.z;
			fp >> ignore >> materials[i].SubMaterial[j].Shininess;
			fp >> ignore >> subMapNum;

			for (UINT k = 0; k < subMapNum; ++k)
			{
				fp >> ignore >> subNo;
				fp >> ignore >> ignore;
				materials[i].SubMaterial[j].TexMap[subNo] = ignore;
			}
		}
		for (UINT j = 0; j < rootMapNum; ++j)
		{
			UINT subNo;

			fp >> ignore >> subNo;
			fp >> ignore >> ignore;
			materials[i].TexMap[subNo] = ignore;
		}
	}
}

void ZXCLoader::ReadNodes(std::wifstream& fp)
{
	std::wstring ignore;

	fp >> ignore;
	for (UINT i = 0; i < mNumNodes; ++i)
	{
		auto& nodes = mFileinfo.Nodes;

		UINT Index;
		UINT ParentNum;

		fp >> ignore >> ignore >> Index;
		nodes[Index].NodeName = ignore;
		fp >> ignore >> ignore >> ParentNum;
		nodes[Index].ParentName = ignore;
		fp >> ignore >> ignore;
		
		if (ignore.find(L"Bone") != std::wstring::npos)
		{
			nodes[Index].Type = ObjectType::BONE;
		}
		else if (ignore.find(L"Dummy") != std::wstring::npos)
		{
			nodes[Index].Type = ObjectType::DUMMY;
		}
		else if (ignore.find(L"Biped") != std::wstring::npos)
		{
			nodes[Index].Type = ObjectType::BIPED;
		}
		else
		{
			nodes[Index].Type = ObjectType::MESH;
		}

		nodes[Index].ParentIndex = ParentNum;

		fp >> ignore;
		fp >> ignore >> nodes[Index].World._11 >> nodes[Index].World._12 >> nodes[Index].World._13 >> nodes[Index].World._14;
		fp >> ignore >> nodes[Index].World._21 >> nodes[Index].World._22 >> nodes[Index].World._23 >> nodes[Index].World._24;
		fp >> ignore >> nodes[Index].World._31 >> nodes[Index].World._32 >> nodes[Index].World._33 >> nodes[Index].World._34;
		fp >> ignore >> nodes[Index].World._41 >> nodes[Index].World._42 >> nodes[Index].World._43 >> nodes[Index].World._44;
		fp >> ignore >> nodes[Index].Translations.x >> nodes[Index].Translations.y >> nodes[Index].Translations.z;
		fp >> ignore >> nodes[Index].RotationQuat.x >> nodes[Index].RotationQuat.y >> nodes[Index].RotationQuat.z >> nodes[Index].RotationQuat.w;
		fp >> ignore >> nodes[Index].ScaleQuat.x >> nodes[Index].ScaleQuat.y >> nodes[Index].ScaleQuat.z >> nodes[Index].ScaleQuat.w;
		fp >> ignore >> nodes[Index].Scale.x >> nodes[Index].Scale.y >> nodes[Index].Scale.z;
		fp >> ignore >> nodes[Index].box.Center.x >> nodes[Index].box.Center.y >> nodes[Index].box.Center.z;
		fp >> ignore >> nodes[Index].box.Extents.x >> nodes[Index].box.Extents.y >> nodes[Index].box.Extents.z;
	}
}

void ZXCLoader::ReadIndices(std::wifstream & fp)
{
	auto& indices = mFileinfo.Indices;

	std::wstring ignore;
	fp >> ignore;
	indices.resize(mNumTriangles * 3);
	for (UINT i = 0; i < mNumTriangles; ++i)
	{
		fp >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
}

void ZXCLoader::ReadSubsetTable(std::wifstream & fp)
{
	auto & subsets = mFileinfo.Subsets;

	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < mNumSubSet; ++i)
	{
		fp >> ignore >> ignore >> subsets[i].NodeIndex;
		fp >> ignore >> subsets[i].MtrlRef;
		fp >> ignore >> subsets[i].SubMtlID;
		fp >> ignore >> subsets[i].VertexStart;
		fp >> ignore >> subsets[i].VertexCount;
		fp >> ignore >> subsets[i].FaceStart;
		fp >> ignore >> subsets[i].FaceCount;
	}
}

void ZXCLoader::ReadBoundingBox(std::wifstream & fp)
{
	auto& box = mFileinfo.Box;

	std::wstring ignore;

	fp >> ignore;

	fp >> ignore >> box.Center.x >> box.Center.y >> box.Center.z;
	fp >> ignore >> box.Extents.x >> box.Extents.y >> box.Extents.z;
}

void ZXCLoader::ReadOffsets(std::wifstream & fp, std::vector<DirectX::XMFLOAT4X4>& boneOffsets)
{
	std::wstring ignore;

	fp >> ignore;

	for (UINT i = 0; i < (UINT)boneOffsets.size(); ++i)
	{
		fp >> ignore;
		fp >> ignore >> boneOffsets[i]._11 >> boneOffsets[i]._12 >> boneOffsets[i]._13 >> boneOffsets[i]._14;
		fp >> ignore >> boneOffsets[i]._21 >> boneOffsets[i]._22 >> boneOffsets[i]._23 >> boneOffsets[i]._24;
		fp >> ignore >> boneOffsets[i]._31 >> boneOffsets[i]._32 >> boneOffsets[i]._33 >> boneOffsets[i]._34;
		fp >> ignore >> boneOffsets[i]._41 >> boneOffsets[i]._42 >> boneOffsets[i]._43 >> boneOffsets[i]._44;
	}
}

void ZXCLoader::BuildInitPos(std::vector<DirectX::XMFLOAT4X4>& initPos, const std::vector<MeshNode>& meshNodes)
{
	for (UINT i = 0; i < (UINT)meshNodes.size(); ++i)
	{
		XMMATRIX W = XMLoadFloat4x4(&meshNodes[i].World);
		XMMATRIX InvW = XMMatrixIdentity();
		if (meshNodes[i].ParentIndex != -1)
		{
			int pIndex = meshNodes[i].ParentIndex;
			XMMATRIX PW = XMLoadFloat4x4(&meshNodes[pIndex].World);
			InvW = XMMatrixInverse(&XMMatrixDeterminant(PW), PW);
		}

		W = XMMatrixMultiply(W, InvW);

		XMStoreFloat4x4(&initPos[i], W);
	}
}

void ZXCLoader::BuildDefaultAnimaions()
{
	AnimationClip clip;
	clip.BoneAnimations.resize((UINT)mFileinfo.Nodes.size());

	std::vector<int> boneHierarchy;
	std::vector<XMFLOAT4X4> initPos;
	std::vector<XMFLOAT4X4> boneOffsets;
	std::unordered_map<std::wstring, AnimationClip> animations;

	boneHierarchy.resize((UINT)mFileinfo.Nodes.size());
	initPos.resize((UINT)mFileinfo.Nodes.size());
	boneOffsets.resize((UINT)mFileinfo.Nodes.size());
	
	animations[L"default"] = clip;
	BuildInitPos(initPos, mFileinfo.Nodes);

//	ReadOffsets(fp, boneOffsets);
	SetBoneOffsets(boneOffsets, mFileinfo.Nodes);
	for (UINT i = 0; i < (UINT)boneHierarchy.size(); ++i)
	{
		boneHierarchy[i] = mFileinfo.Nodes[i].ParentIndex;
	}
	mFileinfo.skinInfo->Set(boneHierarchy, boneOffsets, initPos, animations);
}

void ZXCLoader::SetBoneOffsets(std::vector<DirectX::XMFLOAT4X4>& boneOffsets, const std::vector<MeshNode>& nodes)
{
	boneOffsets.resize(nodes.size());

	for (UINT i = 0; i < (UINT)boneOffsets.size(); ++i)
	{
		XMMATRIX W = XMLoadFloat4x4(&nodes[i].World);
		XMMATRIX InvW = XMMatrixInverse(&XMMatrixDeterminant(W), W);
		XMStoreFloat4x4(&boneOffsets[i], InvW);
	}
}
