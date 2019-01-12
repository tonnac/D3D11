#include "ClipBinLoader.h"
#include "ZXCBinLoader.h"

using namespace DirectX;

bool ClipBinLoader::LoadClip(const std::wstring & FileName, UINT numBones, std::unordered_map<std::wstring, AnimationClip>& clips)
{
	std::string File = std::string(FileName.begin(), FileName.end());
	std::ifstream fp(File.c_str(), std::ios::binary);
	std::wstring ignore;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L" Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}
	ZXCBinLoader::ReadBinary(fp, SaveTime);
	ZXCBinLoader::ReadString(fp, mExporterVersion);

	ReadScene(fp);
	ReadAnimationClips(fp, numBones, 0, clips);

	return true;
}

void ClipBinLoader::ReadScene(std::ifstream& fp)
{
	ZXCBinLoader::ReadBinary(fp, mSceneinfo);
}

void ClipBinLoader::ReadAnimationClips(
	std::ifstream& fp,
	UINT numBones,
	UINT numAnimationClips,
	std::unordered_map<std::wstring, AnimationClip>& animations)
{
	std::wstring Name;
	AnimationClip clip;
	int bonesize;
	ZXCBinLoader::ReadString(fp, Name);
	ZXCBinLoader::ReadBinary(fp, bonesize);
	clip.BoneAnimations.resize(bonesize);
	clip.SceneInf = mSceneinfo;
	for (int i = 0; i < bonesize; ++i)
	{
		int size = 0;
		ZXCBinLoader::ReadBinary(fp, size);
		clip.BoneAnimations[i].Keyframes.resize(size);
		ZXCBinLoader::ReadBinary(fp, clip.BoneAnimations[i].Keyframes.data(), (UINT)(sizeof(KeyFrame) * size));
	}

	animations[Name] = clip;
}
