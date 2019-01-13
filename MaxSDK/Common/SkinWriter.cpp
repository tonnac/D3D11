#include "SkinWriter.h"
#include "NodesLoader.h"

SkinWriter::SkinWriter(const OutputData& outData)
	: Writer(outData)
{
	mNumVertices = (UINT)mOutData.SkinnedVertices.size();
}

void SkinWriter::SaveVertices(std::wofstream & os)
{
	std::wstring vertexHeader = L"\n**********Vertices**********";

	os << vertexHeader;

	for (auto&m : mOutData.SkinnedVertices)
	{
		std::wstring weightInfo = L"\nWeightNum: " + std::to_wstring((int)m.w[3]);
		std::wstring weight;
		for (int i = 0; i < (int)m.w[3]; ++i)
		{
			weight += L" Index" + std::to_wstring(i) + L" " + std::to_wstring(m.i[i]) + L" " + std::to_wstring(m.w[i]);
		}
		
		weightInfo += weight;

		std::wstring VertexWInfo = L"\nPosition: " + std::to_wstring(m.p.x) + L" " + std::to_wstring(m.p.y) + L" " + std::to_wstring(m.p.z) +
			L"\nNormal: " + std::to_wstring(m.n.x) + L" " + std::to_wstring(m.n.y) + L" " + std::to_wstring(m.n.z) +
			L"\nColor: " + std::to_wstring(m.c.x) + L" " + std::to_wstring(m.c.y) + L" " + std::to_wstring(m.c.z) + L" " + std::to_wstring(m.c.w) +
			L"\nTex-Coords: " + std::to_wstring(m.t.x) + L" " + std::to_wstring(m.t.y) +
			L"\nTangent: " + std::to_wstring(m.Tangent.x) + L" " + std::to_wstring(m.Tangent.y) + L" " + std::to_wstring(m.Tangent.z) +
			weightInfo;

		os << VertexWInfo << std::endl;
	}
}

