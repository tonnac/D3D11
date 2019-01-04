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
	void ReadScene(std::wifstream& fp);
	void ReadAnimationClips(std::wifstream& fp, UINT numBones, UINT numAnimationClips, std::unordered_map<std::string, AnimationClip>& animations, const std::vector<MeshNode>& meshNodes);
	void ReadBoneKeyframes(std::wifstream& fp, UINT numBones, UINT numKeyframe, BoneAnimation& boneAnimation);

	void AdjustAnimations(AnimationClip& animations, const std::vector<MeshNode>& meshNodes);

	void SetBoneOffsets(std::vector<DirectX::XMFLOAT4X4>& boneOffsets, const std::vector<MeshNode>& nodes);

	UINT mFirstFrame = 0;
	UINT mLastFrame = 0;
	UINT mFrameSpeed = 0;
	UINT mFrameTick = 0;
};