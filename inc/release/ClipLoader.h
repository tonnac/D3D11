#pragma once
#include "ZXCLoader.h"
#include "SkinnedData.h"

//class Mesh<X>;

class ClipLoader
{
public:
	ClipLoader() = default;

public:
	virtual bool LoadClip(
		const std::wstring & FileName,
		UINT numBones,
		std::unordered_map<std::wstring, AnimationClip>& clips);
private:
	virtual void ReadScene(std::wifstream& fp);
	virtual void ReadAnimationClips(
		std::wifstream& fp,
		UINT numBones,
		UINT numAnimationClips,
		std::unordered_map<std::wstring, AnimationClip>& animations);
	virtual void ReadBoneKeyframes(std::wifstream& fp, UINT numBones, UINT numKeyframe, BoneAnimation& boneAnimation);

	UINT mFirstFrame = 0;
	UINT mLastFrame = 0;
	UINT mFrameSpeed = 0;
	UINT mFrameTick = 0;
};