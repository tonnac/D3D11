#include "ClipLoader.h"

using namespace DirectX;

bool ClipLoader::LoadClip(const std::wstring & FileName, UINT numBones, std::unordered_map<std::wstring, AnimationClip>& clips)
{
	std::wifstream fp(FileName.c_str());
	std::wstring ignore;

	UINT numClips;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L" Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	std::getline(fp, ignore);
	std::getline(fp, ignore);
	std::getline(fp, ignore);

	fp >> ignore >> numClips;

	ReadScene(fp);
	ReadAnimationClips(fp, numBones, numClips, clips);

	return true;
}

void ClipLoader::ReadScene(std::wifstream& fp)
{
	std::wstring ignore;

	fp >> ignore;
	fp >> ignore >> mFirstFrame;
	fp >> ignore >> mLastFrame;
	fp >> ignore >> mFrameSpeed;
	fp >> ignore >> mFrameTick;
}

void ClipLoader::ReadAnimationClips(
	std::wifstream& fp,
	UINT numBones,
	UINT numAnimationClips,
	std::unordered_map<std::wstring, AnimationClip>& animations)
{
	std::wstring ignore;

	fp >> ignore;

	for (UINT clipIndex = 0; clipIndex < numAnimationClips; ++clipIndex)
	{
		UINT nodeIndex;
		UINT frameNum;

		fp >> ignore >> ignore;
		std::wstring clipName = ignore;

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