#pragma once
#include "Writer.h"

template<typename X = OutVertex>
class BinWriter : public Writer
{
public:
	BinWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const std::vector<ZXCMaterial>& material,
		const std::vector<OutputObject>& object,
		const std::vector<X>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<Subset>& subsets)
		: Writer(ExporterVersion, Filename, material, object, indices, subsets), mVertices(vertices)
	{
		mNumVertices = (UINT)vertices.size();
	}

public:
	virtual bool Savefile()
	{
		std::string fileName = std::string(mFilename.begin(), mFilename.end());

		std::ofstream os(fileName.c_str(), std::ios::binary);

		if (!os.is_open()) return false;

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		time_t nowTime = std::chrono::system_clock::to_time_t(now);

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

		SaveMaterial(os);
		SaveNodes(os);
		SaveSubset(os);
		SaveVertices<X>(os);
		SaveIndices(os);

		return true;
	}

protected:
	void SaveScene(std::ofstream& os)
	{
		BinaryIO::WriteBinary(os, mSceneInfo);
	}
	void SaveMaterial(std::ofstream& os)
	{
		for (UINT i = 0; i < (UINT)mMaterial.size(); ++i)
		{
			BinaryIO::WriteMaterial(os, mMaterial[i]);
		}
	}
	void SaveNodes(std::ofstream& os)
	{
		for (const auto& p : mObjects)
		{
			BinaryIO::WriteNodes(os, p);
		}
	}
	void SaveSubset(std::ofstream& os)
	{
		BinaryIO::WriteBinary(os, mSubsets.data(), (UINT)(sizeof(Subset) * mSubsets.size()));
	}
	template<typename X>
	void SaveVertices(std::ofstream & os)
	{
		BinaryIO::WriteBinary(os, mVertices.data(), (UINT)(sizeof(X) * mVertices.size()));
	}
	void SaveIndices(std::ofstream& os)
	{
		BinaryIO::WriteBinary(os, mIndices.data(), (UINT)(sizeof(std::uint32_t) * mIndices.size()));
	}

private:
	const std::vector<X>& mVertices;
};