#include "SkinnedData.h"

using namespace DirectX;

int BoneAnimation::GetStartTime()const
{
	if (!Keyframes.empty())
		return Keyframes.front().Tick;
	else
		return 0;
}

int BoneAnimation::GetEndTime()const
{
	if (!Keyframes.empty())
		return Keyframes.back().Tick;
	else
		return 0;
}

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
	int t = INT_MAX;
	for (UINT i = 0; i < (UINT)BoneAnimations.size(); ++i)
	{
		t = MathHelper::Min(t, BoneAnimations[i].GetStartTime());
	}
	return t;
}

int AnimationClip::GetClipEndTime()const
{
	int t = 0;
	for (UINT i = 0; i < (UINT)BoneAnimations.size(); ++i)
	{
		t = MathHelper::Max(t, BoneAnimations[i].GetEndTime());
	}
	return t;
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

int SkinnedData::GetClipStartTime(const std::string& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipStartTime();
}

int SkinnedData::GetClipEndTime(const std::string& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipEndTime();
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, std::vector<XMFLOAT4X4>& boneOffsets,
	std::unordered_map<std::string, AnimationClip>& animations)
{
	mBoneHierarchy = boneHierarchy;
	mBoneOffsets = boneOffsets;
	mAnimations = animations;
}

void SkinnedData::GetFinalTransforms(const std::string& clipName, int timePos,
	std::vector<XMFLOAT4X4>& finalTransforms)const
{
	UINT numBones = (UINT)mBoneHierarchy.size();

	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	auto clip = mAnimations.find(clipName);
	clip->second.Interpoloate(timePos, toParentTransforms);

	std::vector<XMFLOAT4X4> toRootTransforms(numBones);

	toRootTransforms[0] = toParentTransforms[0];

	for (UINT i = 1; i < numBones; ++i)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[i]);

		int parentIndex = mBoneHierarchy[i];
		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	if (!mBoneOffsets.empty())
	{
		for (UINT i = 0; i < numBones; ++i)
		{
			XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);
			XMMATRIX toRoot = XMLoadFloat4x4(&toRootTransforms[i]);
			XMMATRIX finalTransform = XMMatrixMultiply(offset, toRoot);
			XMStoreFloat4x4(&finalTransforms[i], XMMatrixTranspose(finalTransform));
		}
	}
	else
	{
		for (UINT i = 0; i < numBones; ++i)
		{
			XMStoreFloat4x4(&finalTransforms[i], XMMatrixTranspose(XMLoadFloat4x4(&toRootTransforms[i])));
		}
	}
}