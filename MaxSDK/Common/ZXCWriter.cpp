#include "ZXCWriter.h"
#include "NodesLoader.h"

ZXCWriter::ZXCWriter(
	const OutputData& outData)
	: Writer(outData)
{
	mNumVertices = (UINT)mOutData.Vertices.size();
}

