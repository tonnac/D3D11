#pragma once

#include "d3dUtil.h"
#include "MathHelper.h"

struct SceneInfo
{
	UINT FirstFrame = 0;
	UINT LastFrame = 0;
	UINT FrameSpeed = 0;
	UINT FrameTick = 0;
};

struct KeyFrame
{
	KeyFrame() {}
	~KeyFrame() {}

	int Tick;
	DirectX::XMFLOAT3 Translations;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT4 ScaleQuat;
	DirectX::XMFLOAT4 RotationQuat;
};

struct BoneAnimation
{
	void Interpoloate(int t, DirectX::XMFLOAT4X4& M)const;

	std::vector<KeyFrame> Keyframes;
};

struct AnimationClip
{
	int GetClipStartTime()const;
	int GetClipEndTime()const;

	void Interpoloate(int t, std::vector<DirectX::XMFLOAT4X4>& boneTransforms, const std::vector<DirectX::XMFLOAT4X4>& initPos)const;

	SceneInfo SceneInf;
	std::vector<BoneAnimation> BoneAnimations;
};

class SkinnedData
{
public:
	UINT BoneCount()const;

	int GetClipStartTime(const std::wstring& clipName)const;
	int GetClipEndTime(const std::wstring& clipName)const;

	void Set(
		std::vector<int>& boneHierarchy,
		std::vector<DirectX::XMFLOAT4X4>& boneOffsets,
		std::vector<DirectX::XMFLOAT4X4>& initpos,
		std::unordered_map<std::wstring, AnimationClip>& animations);

	void SetOffsets(std::vector<DirectX::XMFLOAT4X4>& offsets);

	void AddAnimation(
		std::pair<std::wstring, AnimationClip>& animations);

	void GetFinalTransforms(const std::wstring& clipName, int timePos,
		std::vector<DirectX::XMFLOAT4X4>& finalTransforms)const;

	void GetFrameTick(const std::wstring& name, UINT& speed, UINT& tick);

private:
	std::vector<int> mBoneHierarchy;

	std::vector<DirectX::XMFLOAT4X4> mInitPos;
	std::vector<DirectX::XMFLOAT4X4> mBoneOffsets;

	std::unordered_map<std::wstring, AnimationClip> mAnimations;
};

struct SkinnedModelInstance
{
	SkinnedData* SkinnedInfo = nullptr;
	UINT FrameSpeed = 0;
	UINT FrameTick = 0;
	float AnimationSpeed = 1.0f;

	UINT CurClipindex = 0;

	std::vector<std::wstring> Animations;
	std::vector<DirectX::XMFLOAT4X4> FinalTransforms;
	std::wstring ClipName;
	int TimePos = 0;
	void setClipName(const std::wstring& name)
	{
		ClipName = name;
		SkinnedInfo->GetFrameTick(ClipName, FrameSpeed, FrameTick);
	}

	void UpdateSkinnedAnimation(float dt)
	{
		TimePos += (int)(dt * AnimationSpeed * FrameSpeed * FrameTick);

		if (TimePos > SkinnedInfo->GetClipEndTime(ClipName))
		{
			CurClipindex = (CurClipindex + 1) % (UINT)Animations.size();
			ClipName = Animations[CurClipindex];
			TimePos = 0;
		}

		SkinnedInfo->GetFinalTransforms(ClipName, TimePos, FinalTransforms);
	}
};