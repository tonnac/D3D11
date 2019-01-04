#pragma once

#include "Header.h"
#include <chrono>

class ClipWriter
{
public:
	ClipWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		AnimationClip * clip);

public:
	bool Savefile();

private:
	void SaveScene(std::wofstream& os);
	void SaveAnimations(std::wofstream& os);

private:

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;

	const SceneInfo& mSceneInfo;

	const AnimationClip * mAnimation = nullptr;
};