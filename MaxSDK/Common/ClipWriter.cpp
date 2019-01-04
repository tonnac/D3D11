#include "ClipWriter.h"

ClipWriter::ClipWriter(const std::wstring & ExporterVersion, const std::wstring & Filename, const SceneInfo & sceneinfo, AnimationClip * clip)
	: mExporterVersion(ExporterVersion), mFilename(Filename), mSceneInfo(sceneinfo), mAnimation(clip)
{
}

bool ClipWriter::Savefile()
{
	std::wofstream os;
	os.open(mFilename.c_str());

	if (!os.is_open()) return false;

	std::wstring nowTime = MaxUtil::nowtime();

	std::wstring info =
		L"#AnimationClips " + std::to_wstring(1);

	std::wstring header = L"**********ZXCS_Header**********\n#" + mExporterVersion + L"\n#" + nowTime;
	os << header << info;

	SaveAnimations(os);

	return true;
}

void ClipWriter::SaveAnimations(std::wofstream & os)
{
	std::wstring Name(mFilename, 0, mFilename.find_last_of('.'));

	std::wstring aniHeader = L"\n\n**********AnimationClip**********";
	std::wstring NameInfo = L"\n#AnimationName: " + Name + L"\n";

	aniHeader += NameInfo;

	os << aniHeader;


	for (UINT i = 0; i < (UINT)mAnimation->BoneAnimations.size(); ++i)
	{
		auto& ani = mAnimation->BoneAnimations[i].Keyframes;
		std::wstring aniInfo = L"\nNodeIndex: " + std::to_wstring(i) + L" FrameNum: " + std::to_wstring(ani.size());
		os << aniInfo;
		for (UINT j = 0; j < (UINT)mAnimation->BoneAnimations[i].Keyframes.size(); ++j)
		{
			std::wstring trackInfo =
				L"\n\tTick: " + std::to_wstring(ani[j].Tick) +
				L" Translation: " + std::to_wstring(ani[j].Translations.x) + L" " + std::to_wstring(ani[j].Translations.y) + L" " + std::to_wstring(ani[j].Translations.z) +
				L" Scale: " + std::to_wstring(ani[j].Scale.x) + L" " + std::to_wstring(ani[j].Scale.y) + L" " + std::to_wstring(ani[j].Scale.z) +
				L" ScaleQuat: " + std::to_wstring(ani[j].ScaleQuat.x) + L" " + std::to_wstring(ani[j].ScaleQuat.y) + L" " + std::to_wstring(ani[j].ScaleQuat.z) + L" " + std::to_wstring(ani[j].ScaleQuat.w) +
				L" RotateQuat: " + std::to_wstring(ani[j].RotationQuat.x) + L" " + std::to_wstring(ani[j].RotationQuat.y) + L" " + std::to_wstring(ani[j].RotationQuat.z) + L" " + std::to_wstring(ani[j].RotationQuat.w);

			os << trackInfo;
		}
		os << std::endl;
	}
}
