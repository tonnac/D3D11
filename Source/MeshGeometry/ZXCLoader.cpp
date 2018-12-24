#include "ZXCLoader.h"
#include "DxState.h"
#include "Mesh.h"

using namespace DirectX;

bool ZXCLoader::LoadZXC(
	const std::wstring & FileName,
	std::vector<Vertex>& vertices,
	std::vector<DWORD>& indices,
	std::map<std::pair<UINT, int>, std::vector<std::pair<int, Subset>>>& subsets,
	std::map<std::pair<UINT, int>, std::vector<std::pair<UINT, std::wstring>>>& materials,
	std::vector<MeshNode>& nodes,
	SkinnedData& skinInfo)
{
	std::wifstream fp(FileName.c_str());
	std::wstring ignore;

	UINT numMaterials;
	UINT numHelpers;
	UINT numMeshes;
	UINT numVertices;
	UINT numTriangles;
	UINT numAnimationClips;
	UINT numSubSet;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L"Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	std::getline(fp, ignore);
	std::getline(fp, ignore);
	std::getline(fp, ignore);

	fp >> ignore >> numMaterials;
	fp >> ignore >> numHelpers;
	fp >> ignore >> numMeshes;
	fp >> ignore >> numVertices;
	fp >> ignore >> numTriangles;
	fp >> ignore >> numAnimationClips;
	fp >> ignore >> numSubSet;

	nodes.resize(numMeshes + numHelpers);
	std::vector<int> boneHierarchy;
	boneHierarchy.resize(numMeshes + numHelpers);
	std::vector<XMFLOAT4X4> boneOffsets;
	std::unordered_map<std::string, AnimationClip> animations;

	ReadScene(fp);
	ReadMaterial(fp, numMaterials, materials);
	ReadHelper(fp, numHelpers, nodes);
	ReadMesh(fp, numMeshes, nodes);
	ReadSubsetTable(fp, numSubSet, subsets);
	ReadVertex(fp, numVertices, vertices);
	ReadIndex(fp, numTriangles, indices);
	ReadAnimationClips(fp, numMeshes + numHelpers, numAnimationClips, animations, nodes);

	for (UINT i = 0; i < (UINT)boneHierarchy.size(); ++i)
	{
		boneHierarchy[i] = nodes[i].ParentIndex;
	}
	skinInfo.Set(boneHierarchy, boneOffsets, animations);

	return true;
}

void ZXCLoader::ReadScene(std::wifstream& fp)
{
	std::wstring ignore;

	fp >> ignore;
	fp >> ignore >> mFirstFrame;
	fp >> ignore >> mLastFrame;
	fp >> ignore >> mFrameSpeed;
	fp >> ignore >> mFrameTick;
}

void ZXCLoader::ReadMaterial(std::wifstream& fp, UINT numMaterials, std::map<std::pair<UINT, int>, std::vector<std::pair<UINT, std::wstring>>>& materials)
{
	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < numMaterials; ++i)
	{
		UINT rootMapNum;
		UINT subMtlNum;
		fp >> ignore >> ignore;
		fp >> ignore >> rootMapNum;
		fp >> ignore >> subMtlNum;

		for (UINT j = 0; j < subMtlNum; ++j)
		{
			UINT subMapNum;
			UINT subNo;

			fp >> ignore >> ignore;
			fp >> ignore >> subMapNum;

			for (UINT k = 0; k < subMapNum; ++k)
			{
				fp >> ignore >> subNo;
				fp >> ignore >> ignore;
				materials[std::pair<UINT, UINT>(i, j)].push_back(std::pair<UINT, std::wstring>(subNo, ignore));
			}
		}
		for (UINT j = 0; j < rootMapNum; ++j)
		{
			UINT subNo;

			fp >> ignore >> subNo;
			fp >> ignore >> ignore;

			materials[std::pair<UINT, int>(i, -1)].push_back(std::pair<UINT, std::wstring>(subNo, ignore));
		}
	}
}

void ZXCLoader::ReadMesh(std::wifstream& fp, UINT numMeshes, std::vector<MeshNode>& nodes)
{
	std::wstring ignore;

	fp >> ignore;
	for (UINT i = 0; i < numMeshes; ++i)
	{
		UINT Index;
		UINT ParentNum;
		fp >> ignore >> ignore >> Index;
		nodes[Index].NodeName = std::string(ignore.begin(), ignore.end());
		fp >> ignore >> ignore >> ParentNum;
		nodes[Index].ParentName = std::string(ignore.begin(), ignore.end());
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
	}
}

void ZXCLoader::ReadHelper(std::wifstream& fp, UINT numHelpers, std::vector<MeshNode>& nodes)
{
	std::wstring ignore;

	fp >> ignore;
	for (UINT i = 0; i < numHelpers; ++i)
	{
		UINT Index;
		UINT ParentNum;
		XMFLOAT3 Min;
		XMFLOAT3 Max;

		fp >> ignore >> ignore >> Index;
		nodes[Index].NodeName = std::string(ignore.begin(), ignore.end());
		fp >> ignore >> ignore >> ParentNum;
		nodes[Index].ParentName = std::string(ignore.begin(), ignore.end());
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
		fp >> ignore >> Min.x >> Min.y >> Min.z;
		fp >> ignore >> Max.x >> Max.y >> Max.z;

		XMVECTOR min = XMLoadFloat3(&Min);
		XMVECTOR max = XMLoadFloat3(&Max);

		XMStoreFloat3(&nodes[Index].box.Center, (min + max) * 0.5f);
		XMStoreFloat3(&nodes[Index].box.Extents, (max - min) * 0.5f);
	}
}

void ZXCLoader::ReadVertex(std::wifstream & fp, UINT numVertices, std::vector<Vertex>& vertices)
{
	std::wstring ignore;
	fp >> ignore;

	vertices.resize(numVertices);
	for (UINT i = 0; i < numVertices; ++i)
	{
		fp >> ignore >> vertices[i].p.x >> vertices[i].p.y >> vertices[i].p.z;
		fp >> ignore >> vertices[i].n.x >> vertices[i].n.y >> vertices[i].n.z;
		fp >> ignore >> vertices[i].c.x >> vertices[i].c.y >> vertices[i].c.z >> vertices[i].c.w;
		fp >> ignore >> vertices[i].t.x >> vertices[i].t.y;
	}
}

void ZXCLoader::ReadIndex(std::wifstream & fp, UINT numIndices, std::vector<DWORD>& indices)
{
	std::wstring ignore;
	fp >> ignore;
	indices.resize(numIndices * 3);
	for (UINT i = 0; i < numIndices; ++i)
	{
		fp >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
}

void ZXCLoader::ReadSubsetTable(std::wifstream & fp, UINT numSubsets, std::map<std::pair<UINT, int>, std::vector<std::pair<int, Subset>>>& subsets)
{
	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < numSubsets; ++i)
	{
		UINT nodeIndex;
		UINT mtrlRef;
		UINT subMtlID;
		Subset subset;
		fp >> ignore >> nodeIndex >> mtrlRef >> subMtlID;
		fp >> ignore >> subset.VertexStart;
		fp >> ignore >> subset.VertexCount;
		fp >> ignore >> subset.FaceStart;
		fp >> ignore >> subset.FaceCount;

		subsets[std::pair<UINT, int>(nodeIndex, mtrlRef)].push_back(std::pair<int, Subset>(subMtlID, subset));
	}
}

void ZXCLoader::ReadAnimationClips(std::wifstream& fp, UINT numBones, UINT numAnimationClips, std::unordered_map<std::string, AnimationClip>& animations, const std::vector<MeshNode>& meshNodes)
{
	std::wstring ignore;
	for (UINT clipIndex = 0; clipIndex < numAnimationClips; ++clipIndex)
	{
		UINT nodeIndex;
		UINT frameNum;

		AnimationClip clip;
		clip.BoneAnimations.resize(numBones);

		clip.SceneInf.FirstFrame = mFirstFrame;
		clip.SceneInf.LastFrame = mLastFrame;
		clip.SceneInf.FrameSpeed = mFrameSpeed;
		clip.SceneInf.FrameTick = mFrameTick;

		for (UINT boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			fp >> ignore >> nodeIndex >> ignore >> frameNum;
			ReadBoneKeyframes(fp, numBones, frameNum, clip.BoneAnimations[boneIndex]);
		}

		animations["default"] = clip;
		AdjustAnimations(animations["default"], meshNodes);
	}
}

void ZXCLoader::ReadBoneKeyframes(std::wifstream & fp, UINT numBones, UINT numKeyframe, BoneAnimation & boneAnimation)
{
	std::wstring ignore;
	
	boneAnimation.Keyframes.resize(numKeyframe);
	for (UINT i = 0; i < numKeyframe; ++i)
	{
		int tick = 0;
		XMFLOAT3 t(0.0f, 0.0f, 0.0f);
		XMFLOAT3 s(1.0f, 1.0f, 1.0f);
		XMFLOAT4 sq(0.0f, 0.0f, 0.0f, 1.0f);
		XMFLOAT4 r(0.0f, 0.0f, 0.0f, 1.0f);

		fp >> ignore >> tick;
		fp >> ignore >> t.x >> t.y >> t.z;
		fp >> ignore >> s.x >> s.y >> s.z;
		fp >> ignore >> sq.x >> sq.y >> sq.z >> sq.w;
		fp >> ignore >> r.x >> r.y >> r.z >> r.w;

		boneAnimation.Keyframes[i].Tick			= tick;
		boneAnimation.Keyframes[i].Translations = t;
		boneAnimation.Keyframes[i].Scale		= s;
		boneAnimation.Keyframes[i].ScaleQuat	= sq;
		boneAnimation.Keyframes[i].RotationQuat = r;
	}
}

void ZXCLoader::AdjustAnimations(AnimationClip& animations, const std::vector<MeshNode>& meshNodes)
{
	for (UINT i = 0; i < (UINT)meshNodes.size(); ++i)
	{
		if (animations.BoneAnimations[i].Keyframes.empty())
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

			XMStoreFloat4x4(&animations.BoneAnimations[i].InitialPos, W);
		}
	}
}