#include "ClipLoader.h"

using namespace DirectX;

bool ClipLoader::LoadClip(const std::wstring & FileName, const std::vector<MeshNode>& nodes, SkinnedData & skinInfo)
{
	std::wifstream fp(FileName.c_str());
	std::wstring ignore;

	UINT numMaterials;
	UINT numHelpers;
	UINT numMeshes;
	UINT numVertices;
	UINT numTriangles;
	UINT numSubSet;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L" Not Found.";
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
	fp >> ignore >> numSubSet;

	//materials.resize(numMaterials);
	//nodes.resize(numMeshes + numHelpers);

	//ReadScene(fp);
	//ReadMaterial(fp, numMaterials, materials);
	//ReadHelper(fp, numHelpers, nodes);
	//ReadMesh(fp, numMeshes, nodes);
	//ReadVertex(fp, numVertices, vertices);
	//ReadIndex(fp, numTriangles, indices);
	//ReadSubsetTable(fp, numSubSet, subsets);

	return true;
}

void ClipLoader::ReadAnimationClips(
	std::wifstream& fp,
	UINT numBones,
	UINT numAnimationClips,
	std::unordered_map<std::wstring, AnimationClip>& animations,
	const std::vector<MeshNode>& meshNodes,
	const std::wstring& clipName)
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

		animations[clipName] = clip;
	}
}

void ClipLoader::ReadBoneKeyframes(std::wifstream & fp, UINT numBones, UINT numKeyframe, BoneAnimation & boneAnimation)
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

		boneAnimation.Keyframes[i].Tick = tick;
		boneAnimation.Keyframes[i].Translations = t;
		boneAnimation.Keyframes[i].Scale = s;
		boneAnimation.Keyframes[i].ScaleQuat = sq;
		boneAnimation.Keyframes[i].RotationQuat = r;
	}
}