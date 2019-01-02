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

	time_t saveTime;
	std::wstring ExporterVersion;
	std::array<UINT, 6> CompositeNum;
	

	ReadBinary(fin, saveTime);
	ReadString(fin, ExporterVersion);
	ReadBinary(fin, CompositeNum[0], (UINT)(CompositeNum.size() * sizeof(UINT)));

	return false;
}
