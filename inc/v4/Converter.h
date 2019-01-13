#pragma once

#include "BinaryIO.h"

class Converter : private ZXCLoader
{
public:
	bool ConverttoSBI(
		const std::wstring & FileName);

	bool ConverttoSKN(
		const std::wstring& FileName);

private:
	void SaveMaterial(std::ofstream& os, const std::vector<ZXCSMaterial>& materials)
	{
		for (UINT i = 0; i < (UINT)materials.size(); ++i)
		{
			BinaryIO::WriteMaterial(os, materials[i]);
		}
	}
	void SaveNodes(std::ofstream& os, const std::vector<MeshNode>& nodes)
	{
		for (const auto& p : nodes)
		{
			BinaryIO::WriteNodes(os, p);
		}
	}
	void SaveSubset(std::ofstream& os, const std::vector<Subset>& subsets)
	{
		BinaryIO::WriteBinary(os, subsets.data(), (UINT)(sizeof(Subset) * subsets.size()));
	}
	template<typename X>
	void SaveVertices(std::ofstream & os, const std::vector<X>& vertices)
	{
		BinaryIO::WriteBinary(os, vertices.data(), (UINT)(sizeof(X) * vertices.size()));
	}
	void SaveIndices(std::ofstream& os, const std::vector<DWORD>& indices)
	{
		BinaryIO::WriteBinary(os, indices.data(), (UINT)(sizeof(DWORD) * indices.size()));
	}

	void SaveBoundingBox(std::ofstream& os, const DirectX::BoundingBox& box)
	{
		BinaryIO::WriteBinary(os, box);
	}

private:
	void SaveMaterial(std::wofstream& os, const std::vector<ZXCSMaterial>& materials);
	void SaveNodes(std::wofstream& os, const std::vector<MeshNode>& nodes);
	void SaveSubset(std::wofstream& os, const std::vector<Subset>& subsets);
	void SaveBoundingBox(std::wofstream& os, const DirectX::BoundingBox& box);
	void SaveVertices(std::wofstream& os, const std::vector<SkinnedVertex>& vertices);
	void SaveIndices(std::wofstream& os, const std::vector<DWORD>& indices);
};