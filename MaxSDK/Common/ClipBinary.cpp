#include "ClipBinary.h"

ClipBinary::ClipBinary(const std::wstring & ExporterVersion, const std::wstring & Filename, const SceneInfo & sceneinfo, AnimationClip * clip)
	: ClipWriter(ExporterVersion, Filename, sceneinfo, clip)
{
}

bool ClipBinary::Savefile()
{
	std::string fileName = std::string(mFilename.begin(), mFilename.end());

	std::ofstream os(fileName.c_str(), std::ios::binary);

	if (!os.is_open()) return false;

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	time_t nowTime = std::chrono::system_clock::to_time_t(now);

	BinaryIO::WriteBinary(os, nowTime);
	BinaryIO::WriteString(os, mExporterVersion);

	SaveScene(os);
	SaveAnimations(os);

	return true;
}

void ClipBinary::SaveScene(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mSceneInfo);
}

void ClipBinary::SaveAnimations(std::ofstream & os)
{
	int boneSize = (int)mAnimation->BoneAnimations.size();

	BinaryIO::WriteBinary(os, boneSize);
	for (int i = 0; i < boneSize; ++i)
	{
		auto & p = mAnimation->BoneAnimations[i];
		int keyFrameSize = (int)p.Keyframes.size();
		BinaryIO::WriteBinary(os, keyFrameSize);
		BinaryIO::WriteBinary(os, p.Keyframes.data(), (UINT)(sizeof(KeyFrame) * keyFrameSize));
	}
}
