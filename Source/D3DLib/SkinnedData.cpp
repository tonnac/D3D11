#include "SkinnedData.h"

using namespace DirectX;

void BoneAnimation::Interpoloate(int t, XMFLOAT4X4& M)const
{
	XMVECTOR S;
	XMVECTOR T;
	XMVECTOR SQ;
	XMVECTOR RQ;

	if (t <= Keyframes.front().Tick)
	{
		S = XMLoadFloat3(&Keyframes.front().Scale);
		T = XMLoadFloat3(&Keyframes.front().Translations);
		SQ = XMLoadFloat4(&Keyframes.front().ScaleQuat);
		RQ = XMLoadFloat4(&Keyframes.front().RotationQuat);
	}
	else if (t >= Keyframes.back().Tick)
	{
		S = XMLoadFloat3(&Keyframes.back().Scale);
		T = XMLoadFloat3(&Keyframes.back().Translations);
		SQ = XMLoadFloat4(&Keyframes.back().ScaleQuat);
		RQ = XMLoadFloat4(&Keyframes.back().RotationQuat);
	}
	else
	{
		for (UINT i = 0; i < (UINT)Keyframes.size() - 1; ++i)
		{
			if (t >= Keyframes[i].Tick && t <= Keyframes[i + 1].Tick)
			{
				float lerpPercent = (float)(t - Keyframes[i].Tick) / (float)(Keyframes[i + 1].Tick - Keyframes[i].Tick);

				XMVECTOR s0 = XMLoadFloat3(&Keyframes[i].Scale);
				XMVECTOR s1 = XMLoadFloat3(&Keyframes[i + 1].Scale);

				XMVECTOR sq0 = XMLoadFloat4(&Keyframes[i].ScaleQuat);
				XMVECTOR sq1 = XMLoadFloat4(&Keyframes[i + 1].ScaleQuat);

				XMVECTOR r0 = XMLoadFloat4(&Keyframes[i].RotationQuat);
				XMVECTOR r1 = XMLoadFloat4(&Keyframes[i + 1].RotationQuat);

				XMVECTOR t0 = XMLoadFloat3(&Keyframes[i].Translations);
				XMVECTOR t1 = XMLoadFloat3(&Keyframes[i + 1].Translations);

				S = XMVectorLerp(s0, s1, lerpPercent);
				T = XMVectorLerp(t0, t1, lerpPercent);
				SQ = XMQuaternionSlerp(sq0, sq1, lerpPercent);
				RQ = XMQuaternionSlerp(r0, r1, lerpPercent);

				break;
			}
		}
	}

	XMMATRIX Trans = XMMatrixTranslation(XMVectorGetX(T), XMVectorGetY(T), XMVectorGetZ(T));
	XMMATRIX Rot = XMMatrixRotationQuaternion(RQ);
	XMMATRIX ScaleRot = XMMatrixRotationQuaternion(SQ);
	XMMATRIX ScaleInv = XMMatrixInverse(&XMMatrixDeterminant(ScaleRot), ScaleRot);
	XMMATRIX Scale = XMMatrixScaling(XMVectorGetX(S), XMVectorGetY(S), XMVectorGetZ(S));

	XMMATRIX finalScale = ScaleInv * Scale * ScaleRot;
	XMMATRIX F = finalScale * Rot * Trans;

	XMStoreFloat4x4(&M, finalScale * Rot * Trans);
}

int AnimationClip::GetClipStartTime()const
{
	return SceneInf.FirstFrame * SceneInf.FrameSpeed * SceneInf.FrameTick;
}

int AnimationClip::GetClipEndTime()const
{
	return SceneInf.LastFrame * SceneInf.FrameSpeed * SceneInf.FrameTick;
}

void AnimationClip::Interpoloate(int t, std::vector<XMFLOAT4X4>& boneTransforms)const
{
	for (UINT i = 0; i < (UINT)BoneAnimations.size(); ++i)
	{
		if(!BoneAnimations[i].Keyframes.empty())
			BoneAnimations[i].Interpoloate(t, boneTransforms[i]);
		else
		{
			boneTransforms[i] = BoneAnimations[i].InitialPos;
		}
	}
}

UINT SkinnedData::BoneCount()const
{
	return (UINT)mBoneHierarchy.size();
}

int SkinnedData::GetClipStartTime(const std::wstring& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipStartTime();
}

int SkinnedData::GetClipEndTime(const std::wstring& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipEndTime();
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, std::vector<XMFLOAT4X4>& boneOffsets,
	std::unordered_map<std::wstring, AnimationClip>& animations)
{
	mBoneHierarchy = boneHierarchy;
	mBoneOffsets = boneOffsets;
	mAnimations = animations;
}

void SkinnedData::GetFinalTransforms(const std::wstring& clipName, int timePos,
	std::vector<XMFLOAT4X4>& finalTransforms)const
{
	UINT numBones = (UINT)mBoneHierarchy.size();

	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	auto clip = mAnimations.find(clipName);
	clip->second.Interpoloate(timePos, toParentTransforms);

	std::vector<XMFLOAT4X4> toRootTransforms(numBones);

	UINT p = 0;
	for (;mBoneHierarchy[p] == -1; ++p)
	{
		toRootTransforms[p] = toParentTransforms[p];
	}

	for (; p < numBones; ++p)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[p]);

		int parentIndex = mBoneHierarchy[p];
		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		XMStoreFloat4x4(&toRootTransforms[p], toRoot);
	}


	for (UINT i = 0; i < numBones; ++i)
	{
		XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);
		XMMATRIX toRoot = XMLoadFloat4x4(&toRootTransforms[i]);
		XMMATRIX finalTransform = XMMatrixMultiply(offset, toRoot);
		XMStoreFloat4x4(&finalTransforms[i], XMMatrixTranspose(finalTransform));
	}
}