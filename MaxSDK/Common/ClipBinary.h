#pragma once

#include "ClipWriter.h"

class ClipBinary : public ClipWriter
{
public:
	ClipBinary(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		AnimationClip * clip);

public:
	virtual bool Savefile()override;

private:
	void SaveScene(std::ofstream& os);
	void SaveAnimations(std::ofstream& os);
};