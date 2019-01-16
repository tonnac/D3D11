
#include "Writer.h"
#include "NodesLoader.h"
#include <codecvt>

using namespace std::chrono;

Writer::Writer(OutputData& outData)
	: mOutData(outData)
{
	mNumMaterials = (UINT)mOutData.Materials.size();
	mNumTriangles = (UINT)(mOutData.Indices.size() / 3);
	mNumNodes = (UINT)mOutData.OutObjects.size();
	mNumSubsets = (UINT)mOutData.Subsets.size();
	mNumVertices = (UINT)mOutData.Vertices.size();
}

bool Writer::Savefile()
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

	std::wstring header = L"**********ZXC_Header**********\n#" + p.Version + L"\n#" + MaxUtil::nowtime();
	os << header << info;

	SaveMaterial(os);
	SaveNodes(os);
	SaveSubset(os);
	SaveBoundingBox(os);
	SaveVertices(os);
	SaveIndices(os);
	return true;
}

void Writer::SaveMaterial(std::wofstream & os)
{
	const auto& p = mOutData;

	std::wstring rootMtlInfo = L"\n\n**********Materials**********";

	os << rootMtlInfo;

	for (UINT i = 0; i < (UINT)p.Materials.size(); ++i)
	{
		std::wstring mtlInfo =
			L"\n#Name " + p.Materials[i].Name +
			L"\n#Diffuse: " + std::to_wstring(p.Materials[i].Diffuse.r) + L" " + std::to_wstring(p.Materials[i].Diffuse.g) + L" " + std::to_wstring(p.Materials[i].Diffuse.b) +
			L"\n#Specular: " + std::to_wstring(p.Materials[i].Specular.r) + L" " + std::to_wstring(p.Materials[i].Specular.g) + L" " + std::to_wstring(p.Materials[i].Specular.b) +
			L"\n#Ambient: " + std::to_wstring(p.Materials[i].Ambient.r) + L" " + std::to_wstring(p.Materials[i].Ambient.g) + L" " + std::to_wstring(p.Materials[i].Ambient.b) +
			L"\n#Shininess: " + std::to_wstring(p.Materials[i].Shininess) +
			L"\n#MapNum " + std::to_wstring(p.Materials[i].TexMap.size()) +
			L"\n#SubMtlNum " + std::to_wstring(p.Materials[i].SubMaterial.size());

		os << mtlInfo;

		if (!p.Materials[i].SubMaterial.empty())
		{
			for (int k = 0; k < p.Materials[i].SubMaterial.size(); ++k)
			{
				std::wstring subMtlInfo =
					L"\n\t#SubMtlName " + p.Materials[i].SubMaterial[k].Name +
					L"\n\t#Diffuse: " + std::to_wstring(p.Materials[i].SubMaterial[k].Diffuse.r) + L" " + std::to_wstring(p.Materials[i].SubMaterial[k].Diffuse.g) + L" " + std::to_wstring(p.Materials[i].SubMaterial[k].Diffuse.b) +
					L"\n\t#Specular: " + std::to_wstring(p.Materials[i].SubMaterial[k].Specular.r) + L" " + std::to_wstring(p.Materials[i].SubMaterial[k].Specular.g) + L" " + std::to_wstring(p.Materials[i].SubMaterial[k].Specular.b) +
					L"\n\t#Ambient: " + std::to_wstring(p.Materials[i].SubMaterial[k].Ambient.r) + L" " + std::to_wstring(p.Materials[i].SubMaterial[k].Ambient.g) + L" " + std::to_wstring(p.Materials[i].SubMaterial[k].Ambient.b) +
					L"\n\t#Shininess: " + std::to_wstring(p.Materials[i].SubMaterial[k].Shininess) +
					L"\n\t#MapNum " + std::to_wstring(p.Materials[i].SubMaterial[k].TexMap.size());

				os << subMtlInfo;

				if (!p.Materials[i].SubMaterial[k].TexMap.empty())
				{
					for (auto &x : p.Materials[i].SubMaterial[k].TexMap)
					{
						std::wstring TexInfo =
							L"\n\t#MapSubNO " + std::to_wstring(x.first) +
							L"\n\t#TexFile " + x.second;
						os << TexInfo;
					}
				}
				os << std::endl;
			}
		}
		else
		{
			if (!p.Materials[i].TexMap.empty())
			{
				for (auto &x : p.Materials[i].TexMap)
				{
					std::wstring TexInfo =
						L"\n\t\t#MapSubNO " + std::to_wstring(x.first) +
						L"\t#TexFile " + x.second;
					os << TexInfo;
				}
			}
			os << std::endl;
		}
	}
}

void Writer::SaveNodes(std::wofstream & os)
{
	std::wstring helperHead = L"\n**********Nodes**********";

	os << helperHead;

	UINT i = 0;

	for (auto&x : mOutData.OutObjects)
	{
		std::wstring hclass;

		if (x.Type == ObjectType::MESH)
		{
			hclass = L"Mesh";
		}
		if (x.Type == ObjectType::BIPED)
		{
			hclass = L"Biped";
		}
		else if (x.Type == ObjectType::BONE)
		{
			hclass = L"Bone";
		}
		else if (x.Type == ObjectType::DUMMY)
		{
			hclass = L"Dummy";
		}

		std::wstring Nodesinfo = L"\n#Name " + x.NodeName + L" " + std::to_wstring(i) +
			L"\n#ParentName " + x.ParentName + L" " + std::to_wstring((int)x.ParentIndex) +
			L"\n#Class " + hclass +
			L"\n#Node_TM\n\t#TM_ROW0 " + std::to_wstring(x.World.m[0][0]) + L"  " + std::to_wstring(x.World.m[0][1]) + L"  " + std::to_wstring(x.World.m[0][2]) + L"  " + std::to_wstring(x.World.m[0][3]) +
			L"\n\t#TM_ROW1 " + std::to_wstring(x.World.m[1][0]) + L"  " + std::to_wstring(x.World.m[1][1]) + L"  " + std::to_wstring(x.World.m[1][2]) + L"  " + std::to_wstring(x.World.m[1][3]) +
			L"\n\t#TM_ROW2 " + std::to_wstring(x.World.m[2][0]) + L"  " + std::to_wstring(x.World.m[2][1]) + L"  " + std::to_wstring(x.World.m[2][2]) + L"  " + std::to_wstring(x.World.m[2][3]) +
			L"\n\t#TM_ROW3 " + std::to_wstring(x.World.m[3][0]) + L"  " + std::to_wstring(x.World.m[3][1]) + L"  " + std::to_wstring(x.World.m[3][2]) + L"  " + std::to_wstring(x.World.m[3][3]) +
			L"\n\t#TM_POS " + std::to_wstring(x.Translations.x) + L" " + std::to_wstring(x.Translations.y) + L" " + L" " + std::to_wstring(x.Translations.z) +
			L"\n\t#TM_ROTQUAT " + std::to_wstring(x.RotationQuat.x) + L" " + std::to_wstring(x.RotationQuat.y) + L" " + std::to_wstring(x.RotationQuat.z) + L" " + std::to_wstring(x.RotationQuat.w) +
			L"\n\t#TM_SCALEQUAT " + std::to_wstring(x.ScaleQuat.x) + L" " + std::to_wstring(x.ScaleQuat.y) + L" " + std::to_wstring(x.ScaleQuat.z) + L" " + std::to_wstring(x.ScaleQuat.w) +
			L"\n\t#TM_SCALE " + std::to_wstring(x.Scale.x) + L" " + std::to_wstring(x.Scale.y) + L" " + std::to_wstring(x.Scale.z) +
			L"\n#BoundingBoxCenter " + std::to_wstring(x.box.Center.x) + L" " + std::to_wstring(x.box.Center.y) + L" " + std::to_wstring(x.box.Center.z) +
			L"\n#BoundingBoxExtents " + std::to_wstring(x.box.Extents.x) + L" " + std::to_wstring(x.box.Extents.y) + L" " + std::to_wstring(x.box.Extents.z);

		os << Nodesinfo << std::endl;
		++i;
	}
}

void Writer::SaveSubset(std::wofstream & os)
{
	UINT i = 0;
	std::wstring subsetHeader = L"\n**********SubsetTable**********";
	os << subsetHeader;
	for (auto & x : mOutData.Subsets)
	{
		std::wstring subinfo = L"\nSubsetID" + std::to_wstring(i) +
			L" NodeIndex: " + std::to_wstring(x.NodeIndex) +
			L" MtrlRef: " + std::to_wstring(x.MtrlRef) +
			L" SubMtlID: " + std::to_wstring(x.SubMtlID) +
			L" VertexStart: " + std::to_wstring(x.VertexStart) + L" VertexCount: " + std::to_wstring(x.VertexCount) +
			L" FaceStart: " + std::to_wstring(x.FaceStart) + L" FaceCount: " + std::to_wstring(x.FaceCount);

		os << subinfo;
		++i;
	}
	os << std::endl;
}

void Writer::SaveBoundingBox(std::wofstream & os)
{
	std::wstring BoxHeader = L"\n**********BoundingBox**********";
	os << BoxHeader;

	const auto& center = mOutData.Box.Center;
	const auto& extents = mOutData.Box.Extents;

	std::wstring subinfo = L"\nCenter: " + std::to_wstring(center.x) + L" " + std::to_wstring(center.y) + L" " + std::to_wstring(center.z) +
		L"\nExtents: " + std::to_wstring(extents.x) + L" " + std::to_wstring(extents.y) + L" " + std::to_wstring(extents.z);

	os << subinfo;

	os << std::endl;
}

void Writer::SaveIndices(std::wofstream & os)
{
	std::wstring indexHeader = L"\n**********Index**********\n";

	os << indexHeader;

	for (UINT i = 0; i < (UINT)(mOutData.Indices.size() / 3); ++i)
	{
		std::uint32_t i0 = mOutData.Indices[i * 3 + 0];
		std::uint32_t i1 = mOutData.Indices[i * 3 + 1];
		std::uint32_t i2 = mOutData.Indices[i * 3 + 2];

		std::wstring indexInfo = std::to_wstring(i0) + L" " + std::to_wstring(i1) + L" " + std::to_wstring(i2);
		os << indexInfo << std::endl;
	}
}

void Writer::SaveVertices(std::wofstream & os)
{
	std::wstring vertexHeader = L"\n**********Vertices**********";

	os << vertexHeader;

	for (auto&m : mOutData.Vertices)
	{
#ifdef SKINNED
		std::wstring weightInfo = L"\nWeightNum: " + std::to_wstring((int)m.w[3]);
		std::wstring weight;
		for (int i = 0; i < (int)m.w[3]; ++i)
		{
			weight += L" Index" + std::to_wstring(i) + L" " + std::to_wstring(m.i[i]) + L" " + std::to_wstring(m.w[i]);
		}

		weightInfo += weight;

		std::wstring vertexInfo = L"\nPosition: " + std::to_wstring(m.p.x) + L" " + std::to_wstring(m.p.y) + L" " + std::to_wstring(m.p.z) +
			L"\nNormal: " + std::to_wstring(m.n.x) + L" " + std::to_wstring(m.n.y) + L" " + std::to_wstring(m.n.z) +
			L"\nColor: " + std::to_wstring(m.c.x) + L" " + std::to_wstring(m.c.y) + L" " + std::to_wstring(m.c.z) + L" " + std::to_wstring(m.c.w) +
			L"\nTex-Coords: " + std::to_wstring(m.t.x) + L" " + std::to_wstring(m.t.y) +
			L"\nTangent: " + std::to_wstring(m.Tangent.x) + L" " + std::to_wstring(m.Tangent.y) + L" " + std::to_wstring(m.Tangent.z) +
			weightInfo;
#else
		std::wstring vertexInfo = L"\nPosition: " + std::to_wstring(m.p.x) + L" " + std::to_wstring(m.p.y) + L" " + std::to_wstring(m.p.z) +
			L"\nNormal: " + std::to_wstring(m.n.x) + L" " + std::to_wstring(m.n.y) + L" " + std::to_wstring(m.n.z) +
			L"\nColor: " + std::to_wstring(m.c.x) + L" " + std::to_wstring(m.c.y) + L" " + std::to_wstring(m.c.z) + L" " + std::to_wstring(m.c.w) +
			L"\nTex-Coords: " + std::to_wstring(m.t.x) + L" " + std::to_wstring(m.t.y) +
			L"\nTangent: " + std::to_wstring(m.Tangent.x) + L" " + std::to_wstring(m.Tangent.y) + L" " + std::to_wstring(m.Tangent.z);
#endif
		os << vertexInfo << std::endl;
	}
}