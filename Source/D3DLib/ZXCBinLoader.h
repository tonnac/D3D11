#pragma once
#include "ZXCLoader.h"

class ZXCBinLoader : public ZXCLoader
{
public:
	template<class T = Vertex>
	bool LoadBinary(
		const std::wstring & FileName,
		std::vector<T>& vertices,
		std::vector<DWORD>& indices,
		std::vector<Subset>& subsets,
		std::vector<ZXCSMaterial>& materials,
		std::vector<MeshNode>& nodes)
	{
		std::ifstream fin(FileName.c_str(), std::ios::binary);

		if (!fin.is_open()) return false;

		SceneInfo scene;
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

		materials.resize(numMaterials);
		nodes.resize(numNodes);
		subsets.resize(numSubsets);
		vertices.resize(numVertices);
		indices.resize(numTriangles * 3);

		LoadScene(fin, scene);
		LoadMaterials(fin, materials);
		LoadNodes(fin, nodes);
		LoadSubsets(fin, subsets);
		LoadVertices(fin, vertices);
		LoadIndices(fin, indices);

		return true;
	}

private:
	void LoadScene(std::ifstream& fin, SceneInfo& scene);
	void LoadMaterials(std::ifstream& fin, std::vector<ZXCSMaterial>& materials);
	void LoadNodes(std::ifstream&fin, std::vector<MeshNode>& nodes);
	void LoadSubsets(std::ifstream&fin, std::vector<Subset>& subsets);
	template<class T>
	void LoadVertices(std::ifstream&fin, std::vector<T>& vertices)
	{
		ReadBinary(fin, vertices.data(), (UINT)(sizeof(T) * vertices.size()));
	}
	void LoadIndices(std::ifstream&fin, std::vector<DWORD>& indices);

private:
	template<typename X>
	void ReadBinary(std::ifstream& fin, X& dest, UINT size = sizeof(X))
	{
		fin.read(reinterpret_cast<char*>(&dest), size);
	}

	template<typename X>
	void ReadBinary(std::ifstream& fin, X* dest, UINT size = sizeof(X))
	{
		LPVOID ptr = (LPVOID)dest;
		fin.read(reinterpret_cast<char*>(ptr), size);
	}

	void ReadString(std::ifstream& fin, std::wstring& str)
	{
		int size;
		ReadBinary(fin, size);
		str.resize(size);
		ReadBinary(fin, str.data(), sizeof(wchar_t) * size);
	}

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