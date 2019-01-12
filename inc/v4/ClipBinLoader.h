#pragma once
#include "ClipLoader.h"

class ClipBinLoader : public ClipLoader
{
public:
	ClipBinLoader() = default;

public:
	virtual bool LoadClip(
		const std::wstring & FileName,
		UINT numBones,
		std::unordered_map<std::wstring, AnimationClip>& clips) override;

private:
	void ReadScene(std::ifstream& fp);
	void ReadAnimationClips(
		std::ifstream& fp,
		UINT numBones,
		UINT numAnimationClips,
		std::unordered_map<std::wstring, AnimationClip>& animations);

	SceneInfo mSceneinfo;

	time_t SaveTime;
	std::wstring mExporterVersion;
};