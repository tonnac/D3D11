#include "BinWriter.h"
#include "NodesLoader.h"

using namespace std::chrono;

BinWriter::BinWriter(
	const std::wstring & ExporterVersion,
	const std::wstring & Filename,
	const SceneInfo & sceneinfo,
	const std::vector<ZXCMaterial>& material,
	const std::vector<OutputObject>& object,
	const std::vector<OutVertex>& vertices,
	const std::vector<std::uint32_t>& indices,
	const std::vector<Subset>& subsets)
	: Writer(ExporterVersion, Filename, sceneinfo, material, object, indices, subsets), mVertices(vertices)
{
	mNumVertices = (UINT)vertices.size();
}

bool BinWriter::Savefile()
{
	std::string fileName = std::string(mFilename.begin(), mFilename.end());

	std::ofstream os(fileName.c_str(), std::ios::binary);

	if (!os.is_open()) return false;

	system_clock::time_point now = system_clock::now();
	time_t nowTime = system_clock::to_time_t(now);

	std::array<UINT, 5> CompositeNum =
	{
		mNumMaterials,
		mNumVertices,
		mNumTriangles,
		mNumNodes,
		mNumSubsets
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

void BinWriter::SaveScene(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mSceneInfo);
}

void BinWriter::SaveMaterial(std::ofstream & os)
{
	for (UINT i = 0; i < (UINT)mMaterial.size(); ++i)
	{
		BinaryIO::WriteMaterial(os, mMaterial[i]);
	}
}

void BinWriter::SaveNodes(std::ofstream & os)
{
	for (const auto& p : mObjects)
	{
		BinaryIO::WriteNodes(os, p);
	}
}

void BinWriter::SaveSubset(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mSubsets.data(), (UINT)(sizeof(Subset) * mSubsets.size()));
}

void BinWriter::SaveIndices(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mIndices.data(), (UINT)(sizeof(std::uint32_t) * mIndices.size()));
}


void BinWriter::SaveVertices(std::ofstream & os)
{
	BinaryIO::WriteBinary(os, mVertices.data(), (UINT)(sizeof(OutVertex) * mVertices.size()));
}