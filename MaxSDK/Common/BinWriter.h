#pragma once
#include "Writer.h"

template<typename X = OutVertex>
class BinWriter : public Writer
{
public:
	BinWriter(const OutputData& outData, const std::vector<X>& vertices)
		: Writer(outData), mVertices(vertices)
	{
		mNumVertices = (UINT)vertices.size();
	}

public:
	virtual bool Savefile()
	{
		std::string fileName = std::string(mOutData.Filename.begin(), mOutData.Filename.end());

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
		BinaryIO::WriteString(os, mOutData.Version);
		BinaryIO::WriteBinary(os, CompositeNum.data(), (UINT)(CompositeNum.size() * sizeof(UINT)));

		SaveMaterial(os);
		SaveNodes(os);
		SaveSubset(os);
		SaveVertices<X>(os);
		SaveIndices(os);

		return true;
	}

protected:
	void SaveMaterial(std::ofstream& os)
	{
		for (UINT i = 0; i < (UINT)mOutData.Materials.size(); ++i)
		{
			BinaryIO::WriteMaterial(os, mOutData.Materials[i]);
		}
	}
	void SaveNodes(std::ofstream& os)
	{
		for (const auto& p : mOutData.OutObjects)
		{
			BinaryIO::WriteNodes(os, p);
		}
	}
	void SaveSubset(std::ofstream& os)
	{
		BinaryIO::WriteBinary(os, mOutData.Subsets.data(), (UINT)(sizeof(Subset) * mOutData.Subsets.size()));
	}
	template<typename X>
	void SaveVertices(std::ofstream & os)
	{
		BinaryIO::WriteBinary(os, mVertices.data(), (UINT)(sizeof(X) * mVertices.size()));
	}
	void SaveIndices(std::ofstream& os)
	{
		BinaryIO::WriteBinary(os, mOutData.Indices.data(), (UINT)(sizeof(std::uint32_t) * mOutData.Indices.size()));
	}

private:
	const std::vector<X>& mVertices;
};