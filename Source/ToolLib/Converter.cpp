#include "Converter.h"

bool Converter::ConverttoSBI(const std::wstring & FileName)
{
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	std::vector<Subset> subsets;
	std::vector<SkinnedVertex> vertices;
	std::vector<DWORD> indices;

	std::wifstream fp(FileName.c_str());
	std::wstring ignore;
	std::wstring ExporterVersion;

	auto time = std::chrono::system_clock::now();
	time_t nowTime = std::chrono::system_clock::to_time_t(time);

	UINT numMaterials;
	UINT numNodes;
	UINT numVertices;
	UINT numTriangles;
	UINT numSubSet;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L"Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	std::getline(fp, ignore);
	std::getline(fp, ExporterVersion);
	std::getline(fp, ignore);

	fp >> ignore >> numMaterials;
	fp >> ignore >> numNodes;
	fp >> ignore >> numVertices;
	fp >> ignore >> numTriangles;
	fp >> ignore >> numSubSet;

	materials.resize(numMaterials);
	nodes.resize(numNodes);

	ReadMaterial(fp, numMaterials, materials);
	ReadNodes(fp, numNodes, nodes);
	ReadSubsetTable(fp, numSubSet, subsets);
	ReadVertex(fp, numVertices, vertices);
	ReadIndices(fp, numTriangles, indices);

	fp.close();
	
	std::wstring file = FileName;

	size_t pos = file.find(L'.');
	file.replace(pos, file.length(), L".sbi");

	std::ofstream fin(file.c_str(), std::ios::binary);

	std::array<UINT, 5> CompositeNum =
	{
		numMaterials,
		numVertices,
		numTriangles,
		numNodes,
		numSubSet
	};

	BinaryIO::WriteBinary(fin, nowTime);
	BinaryIO::WriteString(fin, ExporterVersion);
	BinaryIO::WriteBinary(fin, CompositeNum.data(), (UINT)(CompositeNum.size() * sizeof(UINT)));

	SaveMaterial(fin, materials);
	SaveNodes(fin, nodes);
	SaveSubset(fin, subsets);
	SaveVertices(fin, vertices);
	SaveIndices(fin, indices);

	return true;
}