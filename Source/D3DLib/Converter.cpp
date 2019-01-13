#include "Converter.h"
#include "ZXCBinLoader.h"

bool Converter::ConverttoSBI(const std::wstring & FileName)
{
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	std::vector<Subset> subsets;
	std::vector<SkinnedVertex> vertices;
	std::vector<DWORD> indices;
	DirectX::BoundingBox box;

	std::wifstream fp(FileName.c_str());
	std::wstring ignore;
	std::wstring ExporterVersion;

	auto time = std::chrono::system_clock::now();
	time_t nowTime = std::chrono::system_clock::to_time_t(time);

	if (!fp.is_open())
	{
		std::wstring message = FileName + L"Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	std::getline(fp, ignore);
	std::getline(fp, ExporterVersion);
	std::getline(fp, ignore);

	fp >> ignore >> mNumMaterials;
	fp >> ignore >> mNumNodes;
	fp >> ignore >> mNumVertices;
	fp >> ignore >> mNumTriangles;
	fp >> ignore >> mNumSubSet;

	materials.resize(mNumMaterials);
	nodes.resize(mNumNodes);

	ReadMaterial(fp, materials);
	ReadNodes(fp, nodes);
	ReadSubsetTable(fp, subsets);
	ReadBoundingBox(fp, box);
	ReadVertex(fp, vertices);
	ReadIndices(fp, indices);

	fp.close();
	
	std::wstring file = FileName;

	size_t pos = file.find(L'.');
	file.replace(pos, file.length(), L".sbi");

	std::ofstream fin(file.c_str(), std::ios::binary);

	std::array<UINT, 5> CompositeNum =
	{
		mNumMaterials,
		mNumVertices,
		mNumTriangles,
		mNumNodes,
		mNumSubSet
	};

	BinaryIO::WriteBinary(fin, nowTime);
	BinaryIO::WriteString(fin, ExporterVersion);
	BinaryIO::WriteBinary(fin, CompositeNum.data(), (UINT)(CompositeNum.size() * sizeof(UINT)));

	SaveMaterial(fin, materials);
	SaveNodes(fin, nodes);
	SaveSubset(fin, subsets);
	SaveBoundingBox(fin, box);
	SaveVertices(fin, vertices);
	SaveIndices(fin, indices);

	return true;
}

bool Converter::ConverttoSKN(const std::wstring & FileName)
{
	ZXCBinLoader loader;

	std::vector<SkinnedVertex> vertices;
	std::vector<DWORD> indices;
	std::vector<ZXCLoader::Subset> subsets;
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	DirectX::BoundingBox box;

	if (!loader.LoadBinary(FileName, vertices, indices, subsets, materials, nodes, box))
		return false;

	std::wstring file = FileName;

	size_t pos = file.find(L'.');
	file.replace(pos, file.length(), L".skn");

	std::wofstream os;
	os.open(file.c_str());

	if (!os.is_open()) return false;

	UINT numaterialss = (UINT)materials.size();
	UINT numVertices = (UINT)vertices.size();
	UINT numTriangles = (UINT)((indices.size() / 3));
	UINT numNodes = (UINT)nodes.size();
	UINT numSubsets = (UINT)subsets.size();

	std::wstring info =
		L"#Materials " + std::to_wstring(numaterialss) +
		L"\n#Nodes " + std::to_wstring(numNodes) +
		L"\n#Vertices " + std::to_wstring(numVertices) +
		L"\n#Triangles " + std::to_wstring(numTriangles) +
		L"\n#Subset " + std::to_wstring(numSubsets);

	std::wstring exporterversion = L"qwe";
	std::wstring nowtime = L"\nqwe";

	std::wstring header = L"**********ZXCS_Header**********\n#" + exporterversion + L"\n#" + nowtime;
	os << header << info;

	SaveMaterial(os, materials);
	SaveNodes(os, nodes);
	SaveSubset(os, subsets);
	SaveBoundingBox(os, box);
	SaveVertices(os, vertices);
	SaveIndices(os, indices);


	return true;
}

void Converter::SaveMaterial(std::wofstream & os, const std::vector<ZXCSMaterial>& materials)
{
	std::wstring rootMtlInfo = L"\n\n**********Materials**********";

	os << rootMtlInfo;

	for (UINT i = 0; i < (UINT)materials.size(); ++i)
	{
		std::wstring mtlInfo =
			L"\n#Name " + materials[i].Name +
			L"\n#Diffuse: " + std::to_wstring(materials[i].Diffuse.x) + L" " + std::to_wstring(materials[i].Diffuse.y) + L" " + std::to_wstring(materials[i].Diffuse.z) +
			L"\n#Specular: " + std::to_wstring(materials[i].Specular.x) + L" " + std::to_wstring(materials[i].Specular.y) + L" " + std::to_wstring(materials[i].Specular.z) +
			L"\n#Ambient: " + std::to_wstring(materials[i].Ambient.x) + L" " + std::to_wstring(materials[i].Ambient.y) + L" " + std::to_wstring(materials[i].Ambient.z) +
			L"\n#Shininess: " + std::to_wstring(materials[i].Shininess) +
			L"\n#MapNum " + std::to_wstring(materials[i].TexMap.size()) +
			L"\n#SubMtlNum " + std::to_wstring(materials[i].SubMaterial.size());

		os << mtlInfo;

		if (!materials[i].SubMaterial.empty())
		{
			for (int k = 0; k < materials[i].SubMaterial.size(); ++k)
			{
				std::wstring subMtlInfo =
					L"\n\t#SubMtlName " + materials[i].SubMaterial[k].Name +
					L"\n\t#Diffuse: " + std::to_wstring(materials[i].SubMaterial[k].Diffuse.x) + L" " + std::to_wstring(materials[i].SubMaterial[k].Diffuse.y) + L" " + std::to_wstring(materials[i].SubMaterial[k].Diffuse.z) +
					L"\n\t#Specular: " + std::to_wstring(materials[i].SubMaterial[k].Specular.x) + L" " + std::to_wstring(materials[i].SubMaterial[k].Specular.y) + L" " + std::to_wstring(materials[i].SubMaterial[k].Specular.z) +
					L"\n\t#Ambient: " + std::to_wstring(materials[i].SubMaterial[k].Ambient.x) + L" " + std::to_wstring(materials[i].SubMaterial[k].Ambient.y) + L" " + std::to_wstring(materials[i].SubMaterial[k].Ambient.z) +
					L"\n\t#Shininess: " + std::to_wstring(materials[i].SubMaterial[k].Shininess) +
					L"\n\t#MapNum " + std::to_wstring(materials[i].SubMaterial[k].TexMap.size());

				os << subMtlInfo;

				if (!materials[i].SubMaterial[k].TexMap.empty())
				{
					for (auto &x : materials[i].SubMaterial[k].TexMap)
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
			if (!materials[i].TexMap.empty())
			{
				for (auto &x : materials[i].TexMap)
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

void Converter::SaveNodes(std::wofstream & os, const std::vector<MeshNode>& nodes)
{
	std::wstring helperHead = L"\n**********Nodes**********";

	os << helperHead;

	UINT i = 0;

	for (auto&x : nodes)
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

void Converter::SaveSubset(std::wofstream & os, const std::vector<Subset>& subsets)
{
	UINT i = 0;
	std::wstring subsetHeader = L"\n**********SubsetTable**********";
	os << subsetHeader;
	for (auto & x : subsets)
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

void Converter::SaveBoundingBox(std::wofstream & os, const DirectX::BoundingBox & box)
{
	std::wstring BoxHeader = L"\n**********BoundingBox**********";
	os << BoxHeader;

	const auto& center = box.Center;
	const auto& extents = box.Extents;

	std::wstring subinfo = L"\nCenter: " + std::to_wstring(center.x) + L" " + std::to_wstring(center.y) + L" " + std::to_wstring(center.x) +
		L"\nExtents: " + std::to_wstring(extents.x) + L" " + std::to_wstring(extents.y) + L" " + std::to_wstring(extents.x);

	os << subinfo;

	os << std::endl;
}

void Converter::SaveVertices(std::wofstream & os, const std::vector<SkinnedVertex>& vertices)
{
	std::wstring vertexHeader = L"\n**********Vertices**********";

	os << vertexHeader;

	for (auto&m : vertices)
	{
		std::wstring weightInfo = L"\nWeightNum: " + std::to_wstring((int)m.BoneWeights.w);
		std::wstring weight;
		for (int i = 0; i < (int)m.BoneWeights.w; ++i)
		{
			switch (i)
			{
			case 0:
				weight += L" Index" + std::to_wstring(i) + L" " + std::to_wstring(m.BoneIndices[i]) + L" " + std::to_wstring(m.BoneWeights.x);
				break;
			case 1:
				weight += L" Index" + std::to_wstring(i) + L" " + std::to_wstring(m.BoneIndices[i]) + L" " + std::to_wstring(m.BoneWeights.y);
				break;
			case 2:
				weight += L" Index" + std::to_wstring(i) + L" " + std::to_wstring(m.BoneIndices[i]) + L" " + std::to_wstring(m.BoneWeights.z);
				break;
			case 3:
				weight += L" Index" + std::to_wstring(i) + L" " + std::to_wstring(m.BoneIndices[i]) + L" " + std::to_wstring(m.BoneWeights.w);
				break;
			default:
				break;
			}
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

void Converter::SaveIndices(std::wofstream & os, const std::vector<DWORD>& indices)
{
	std::wstring indexHeader = L"\n**********Index**********\n";

	os << indexHeader;

	for (UINT i = 0; i < (UINT)(indices.size() / 3); ++i)
	{
		std::uint32_t i0 = indices[i * 3 + 0];
		std::uint32_t i1 = indices[i * 3 + 1];
		std::uint32_t i2 = indices[i * 3 + 2];

		std::wstring indexInfo = std::to_wstring(i0) + L" " + std::to_wstring(i1) + L" " + std::to_wstring(i2);
		os << indexInfo << std::endl;
	}
}
