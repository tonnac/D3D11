#include "ZXCBinLoader.h"
#include "DxState.h"
#include "Mesh.h"

using namespace DirectX;

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


void ZXCBinLoader::LoadIndices(std::ifstream & fin, std::vector<DWORD>& indices)
{
	ReadBinary(fin, indices.data(), (UINT)(sizeof(std::uint32_t) * indices.size()));
}
