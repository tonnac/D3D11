#include "ZXCBinLoader.h"
#include "DxState.h"
#include "Mesh.h"

using namespace DirectX;

ZXCBinLoader::ZXCBinLoader(FileInfo & fileInfo)
	: ZXCLoader(fileInfo)
{
}

void ZXCBinLoader::LoadMaterials(std::ifstream & fin)
{
	for (UINT i = 0; i < (UINT)mFileinfo.Materials.size(); ++i)
	{
		ReadMaterial(fin, mFileinfo.Materials[i]);
	}
}

void ZXCBinLoader::LoadNodes(std::ifstream & fin)
{
	for (UINT i = 0; i < (UINT)mFileinfo.Nodes.size(); ++i)
	{
		ReadNodes(fin, mFileinfo.Nodes[i]);
	}
}

void ZXCBinLoader::LoadSubsets(std::ifstream & fin)
{
	ReadBinary(fin, mFileinfo.Subsets.data(), (UINT)(sizeof(Subset) * mFileinfo.Subsets.size()));
}


void ZXCBinLoader::LoadIndices(std::ifstream & fin)
{
	ReadBinary(fin, mFileinfo.Indices.data(), (UINT)(sizeof(std::uint32_t) * mFileinfo.Indices.size()));
}

void ZXCBinLoader::LoadBoundingBox(std::ifstream & fin)
{
	ReadBinary(fin, mFileinfo.Box);
}
