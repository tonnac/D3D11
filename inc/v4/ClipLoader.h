#pragma once
#include "ZXCLoader.h"
#include "SkinnedData.h"

class Mesh;

class ClipLoader
{
public:
	ClipLoader() = default;

public:
	bool LoadClip(
		const std::wstring & FileName,
		const std::vector<MeshNode>& nodes,
		SkinnedData& skinInfo);
private:
	void ReadAnimationClips(
		std::wifstream& fp,
		UINT numBones, 
		UINT numAnimationClips, 
		std::unordered_map<std::wstring, AnimationClip>& animations, 
		const std::vector<MeshNode>& meshNodes,
		const std::wstring& clipName);
	void ReadBoneKeyframes(std::wifstream& fp, UINT numBones, UINT numKeyframe, BoneAnimation& boneAnimation);

	UINT mFirstFrame = 0;
	UINT mLastFrame = 0;
	UINT mFrameSpeed = 0;
	UINT mFrameTick = 0;
};