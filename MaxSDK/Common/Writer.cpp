
#include "Writer.h"
#include "NodesLoader.h"
#include <codecvt>

using namespace std::chrono;

Writer::Writer(const OutputData& outData)
	: mExporterVersion(outData.Version),
	mFilename(outData.Filename),
	mMaterial(outData.Materials),
	mObjects(outData.OutObjects),
	mIndices(outData.Indices),
	mSubsets(outData.Subsets)
{
	mNumMaterials = (UINT)mMaterial.size();
	mNumTriangles = (UINT)(mIndices.size() / 3);
	mNumNodes = (UINT)mObjects.size();
	mNumSubsets = (UINT)mSubsets.size();
}