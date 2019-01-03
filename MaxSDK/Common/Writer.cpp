
#include "Writer.h"
#include "NodesLoader.h"
#include <codecvt>

using namespace std::chrono;

Writer::Writer(const std::wstring& ExporterVersion,
	const std::wstring& Filename,
	const SceneInfo& sceneinfo,
	const std::vector<ZXCMaterial>& material,
	const std::vector<OutputObject>& object,
	const std::vector<std::uint32_t>& indices,
	const std::vector<Subset>& subsets)
	: mExporterVersion(ExporterVersion),
	mFilename(Filename),
	mSceneInfo(sceneinfo),
	mMaterial(material),
	mObjects(object),
	mIndices(indices),
	mSubsets(subsets)
{
	mNumMaterials = (UINT)mMaterial.size();
	mNumTriangles = (UINT)(mIndices.size() / 3);
	mNumNodes = (UINT)mObjects.size();
	mNumSubsets = (UINT)mSubsets.size();
}

std::wstring Writer::Savetime()
{
	system_clock::time_point now = system_clock::now();
	time_t nowTime = system_clock::to_time_t(now);

	char buf[256] = { 0, };
	wchar_t wbuf[256] = { 0, };
	ctime_s(buf, sizeof(buf), &nowTime);

	MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, 256);

	return std::wstring(wbuf);
}