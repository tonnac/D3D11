
#include "ZXCWriter.h"
#include "ObjectExporter.h"
#include <codecvt>

using namespace std::chrono;

ZXCWriter::ZXCWriter(const std::wstring& ExporterVersion,
	const std::wstring& Filename,
	const SceneInfo& sceneinfo,
	const std::vector<ZXCMaterial>& material,
	const std::vector<OutputObject>& object,
	const std::vector<OutVertex>& vertices,
	const std::vector<std::uint32_t>& indices,
	const std::vector<Subset>& subsets)
	: mExporterVersion(ExporterVersion),
	mFilename(Filename),
	mSceneInfo(sceneinfo),
	mMaterial(material),
	mObjects(object),
	mVertices(vertices),
	mIndices(indices),
	mSubsets(subsets)
{}

bool ZXCWriter::Savefile()
{
	std::string fileName = std::string(mFilename.begin(), mFilename.end());

	std::ofstream os(fileName.c_str(), std::ios::binary);

	if (!os.is_open()) return false;

	system_clock::time_point now = system_clock::now();
	time_t nowTime = system_clock::to_time_t(now);

	std::array<UINT, 5> CompositeNum =
	{
		(UINT)mMaterial.size(),
		(UINT)mVertices.size(),
		(UINT)(mIndices.size() / 3),
		(UINT)mObjects.size(),
		(UINT)mSubsets.size()
	};

	BinaryIO::WriteBinary(os, nowTime);
	BinaryIO::WriteString(os, mExporterVersion);
	BinaryIO::WriteBinary(os, CompositeNum.data(), (UINT)(CompositeNum.size() * sizeof(UINT)));

	SaveScene(os);
	SaveMaterial(os);
	SaveNodes(os);
	SaveSubset(os);
	SaveVertices(os);
	SaveIndices(os);

	return true;
}

void ZXCWriter::SaveScene(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mSceneInfo);
}

void ZXCWriter::SaveMaterial(std::ofstream & os)
{
	for (UINT i = 0; i < (UINT)mMaterial.size(); ++i)
	{
		BinaryIO::WriteMaterial(os, mMaterial[i]);
	}
}

void ZXCWriter::SaveNodes(std::ofstream & os)
{
	for (const auto& p : mObjects)
	{
		BinaryIO::WriteNodes(os, p);
	}
}

void ZXCWriter::SaveSubset(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mSubsets.data(), (UINT)(sizeof(Subset) * mSubsets.size()));
}

void ZXCWriter::SaveVertices(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mVertices.data(), (UINT)(sizeof(OutVertex) * mVertices.size()));
}

void ZXCWriter::SaveIndices(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mIndices.data(), (UINT)(sizeof(std::uint32_t) * mIndices.size()));
}
