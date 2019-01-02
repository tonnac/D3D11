#include "ZXCBinLoader.h"
#include "DxState.h"
#include "Mesh.h"

using namespace DirectX;

bool ZXCBinLoader::LoadZXCBin(const std::wstring & FileName,
	std::vector<Vertex>& vertices,
	std::vector<DWORD>& indices,
	std::vector<Subset>& subsets,
	std::vector<ZXCSMaterial>& materials,
	std::vector<MeshNode>& nodes)
{
	std::ifstream fin(FileName.c_str(), std::ios::binary);

	if (!fin.is_open()) return false;

	SceneInfo scene;
	time_t saveTime;
	std::wstring ExporterVersion;
	std::array<UINT, 5> CompositeNum;

	ReadBinary(fin, saveTime);
	ReadString(fin, ExporterVersion);
	ReadBinary(fin, CompositeNum[0], (UINT)(CompositeNum.size() * sizeof(UINT)));

	UINT numMaterials = CompositeNum[0];
	UINT numVertices = CompositeNum[1];
	UINT numTriangles = CompositeNum[2];
	UINT numNodes = CompositeNum[3];
	UINT numSubsets = CompositeNum[4];

	materials.resize(numMaterials);
	nodes.resize(numNodes);
	subsets.resize(numSubsets);
	vertices.resize(numVertices);
	indices.resize(numTriangles * 3);

	LoadScene(fin, scene);
	LoadMaterials(fin, materials);
	LoadNodes(fin, nodes);
	LoadSubsets(fin, subsets);
	LoadVertices(fin, vertices);
	LoadIndices(fin, indices);

	return true;
}

void ZXCBinLoader::LoadScene(std::ifstream & fin, SceneInfo & scene)
{
	ReadBinary(fin, scene);
}

void ZXCBinLoader::LoadMaterials(std::ifstream & fin, std::vector<ZXCSMaterial>& materials)
{
	for (UINT i = 0; i < (UINT)materials.size(); ++i)
	{
		ReadMaterial(fin, materials[i]);
	}
}

void ZXCBinLoader::LoadNodes(std::ifstream & fin, std::vector<MeshNode>& nodes)
{
	for (UINT i = 0; i < (UINT)nodes.size(); ++i)
	{
		ReadNodes(fin, nodes[i]);
	}
}

void ZXCBinLoader::LoadSubsets(std::ifstream & fin, std::vector<Subset>& subsets)
{
	ReadBinary(fin, subsets.data(), (UINT)(sizeof(Subset) * subsets.size()));
}

void ZXCBinLoader::LoadVertices(std::ifstream & fin, std::vector<Vertex>& vertices)
{
	ReadBinary(fin, vertices.data(), (UINT)(sizeof(Vertex) * vertices.size()));
}

void ZXCBinLoader::LoadIndices(std::ifstream & fin, std::vector<DWORD>& indices)
{
	ReadBinary(fin, indices.data(), (UINT)(sizeof(std::uint32_t) * indices.size()));
}
