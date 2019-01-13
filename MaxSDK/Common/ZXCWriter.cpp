#include "ZXCWriter.h"
#include "NodesLoader.h"

ZXCWriter::ZXCWriter(
	const OutputData& outData)
	: Writer(outData)
{
	mNumVertices = (UINT)mOutData.Vertices.size();
}

bool ZXCWriter::Savefile()
{
	const auto& p = mOutData;

	std::wofstream os;
	os.open(p.Filename.c_str());

	if (!os.is_open()) return false;

	std::wstring info =
		L"#Materials " + std::to_wstring(mNumMaterials) +
		L"\n#Nodes " + std::to_wstring(mNumNodes) +
		L"\n#Vertices " + std::to_wstring(mNumVertices) +
		L"\n#Triangles " + std::to_wstring(mNumTriangles) +
		L"\n#Subset " + std::to_wstring(mNumSubsets);

	std::wstring header = L"**********ZXCS_Header**********\n#" + p.Version + L"\n#" + MaxUtil::nowtime();
	os << header << info;

	SaveMaterial(os);
	SaveNodes(os);
	SaveSubset(os);
	SaveBoundingBox(os);
	SaveVertices(os);
	SaveIndices(os);

	return true;
}
