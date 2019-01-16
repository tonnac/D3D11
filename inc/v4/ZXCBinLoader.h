#pragma once
#include "ZXCLoader.h"

class ZXCBinLoader : private ZXCLoader
{
	enum class VertexType
	{
		Vertex,
		SkinnedVertex
	};

public:
	ZXCBinLoader(FileInfo& fileInfo);

public:
	bool LoadBinary(bool isSkinned = true)
	{
		std::string FileName = std::string(mFileinfo.FileName.begin(), mFileinfo.FileName.end());

		std::ifstream fin(FileName.c_str(), std::ios::binary);

		if (!fin.is_open()) return false;

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

		mFileinfo.Materials.resize(numMaterials);
		mFileinfo.Nodes.resize(numNodes);
		mFileinfo.Subsets.resize(numSubsets);
		if (isSkinned)
		{
			mFileinfo.SkinVertices.resize(numVertices);
		}
		else
		{
			mFileinfo.Vertices.resize(numVertices);
		}
		mFileinfo.Indices.resize(numTriangles * 3);

		LoadMaterials(fin);
		LoadNodes(fin);
		LoadSubsets(fin);
		LoadBoundingBox(fin);
		if (isSkinned)
		{
			LoadVertices(fin, VertexType::SkinnedVertex);
		}
		else
		{
			LoadVertices(fin, VertexType::Vertex);
		}
		LoadIndices(fin);
		if (mFileinfo.skinInfo != nullptr)
			BuildDefaultAnimaions();

		return true;
	}


public:
	template<typename X>
	static void ReadBinary(std::ifstream& fin, X& dest, UINT size = sizeof(X))
	{
		fin.read(reinterpret_cast<char*>(&dest), size);
	}

	template<typename X>
	static void ReadBinary(std::ifstream& fin, X* dest, UINT size = sizeof(X))
	{
		LPVOID ptr = (LPVOID)dest;
		fin.read(reinterpret_cast<char*>(ptr), size);
	}

	static void ReadString(std::ifstream& fin, std::wstring& str)
	{
		int size;
		ReadBinary(fin, size);
		str.resize(size);
		ReadBinary(fin, str.data(), sizeof(wchar_t) * size);
	}

private:
	void LoadMaterials(std::ifstream& fin);
	void LoadNodes(std::ifstream&fin);
	void LoadSubsets(std::ifstream&fin);
	void LoadVertices(std::ifstream&fin, VertexType type)
	{
		switch (type)
		{
		case ZXCBinLoader::VertexType::Vertex:
			ReadBinary(fin, mFileinfo.Vertices.data(), (UINT)(sizeof(Vertex) * mFileinfo.Vertices.size()));
			break;
		case ZXCBinLoader::VertexType::SkinnedVertex:
			ReadBinary(fin, mFileinfo.SkinVertices.data(), (UINT)(sizeof(SkinnedVertex) * mFileinfo.SkinVertices.size()));
			break;
		default:
			break;
		}
	}
	void LoadIndices(std::ifstream&fin);
	void LoadBoundingBox(std::ifstream&fin);

private:
	void ReadPair(std::ifstream& fin, std::pair<int, std::wstring> pair)
	{
		ReadBinary(fin, pair.first);
		ReadString(fin, pair.second);
	}

	void ReadMaterial(std::ifstream& fin, ZXCSMaterial& materials)
	{
		ReadString(fin, materials.Name);
		ReadString(fin, materials.ClassName);
		ReadBinary(fin, materials.Ambient);
		ReadBinary(fin, materials.Diffuse);
		ReadBinary(fin, materials.Specular);
		ReadBinary(fin, materials.Shininess);
		int size;
		ReadBinary(fin, size);
		for (int k = 0; k < size; ++k)
		{
			std::pair<int, std::wstring> texmap;
			ReadBinary(fin, texmap.first);
			ReadString(fin, texmap.second);
			materials.TexMap.insert(texmap);
		}
		ReadBinary(fin, size);
		materials.SubMaterial.resize(size);
		for (int k = 0; k < size; ++k)
		{
			ReadMaterial(fin, materials.SubMaterial[k]);
		}
	}

	void ReadNodes(std::ifstream& fin, MeshNode& node)
	{
		ReadString(fin, node.NodeName);
		ReadString(fin, node.ParentName);
		ReadBinary(fin, node.ParentIndex, sizeof(MeshNode) - (sizeof(MeshNode::NodeName) + sizeof(MeshNode::ParentName) + sizeof(MeshNode::Ritem)));
	}
};