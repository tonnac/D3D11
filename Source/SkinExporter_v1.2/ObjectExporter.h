#pragma once

#include "Header.h"

class SkinExporter;

struct BipedVertex
{
	std::wstring mNodename;
	int			 mNodeIndex = -1;
	int			 mNumWeight = -1;

	std::multimap<float, int, std::greater<float>>	 mWeightList;
	std::multimap<float, int, std::greater<float>>::const_iterator iter;
};

class ObjectExporter
{
public:
	ObjectExporter(SkinExporter * exporter);

public:
	void LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
		std::vector<std::unique_ptr<ZXCObject>>& objectlist,
		std::unordered_map<std::wstring, size_t>& nodeIndex,
		std::vector<OutVertex>& vertices,
		std::vector<std::uint32_t>& indices);

private:
	void LoadMesh(INode* node, ZXCObject* o);
	void BuildVBIB(ZXCObject* mesh, std::vector<OutVertex>& vertices, std::vector<std::uint32_t>& indices);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

private:
	void LoadBipedInfo(INode* node, std::vector<BipedVertex>& bipedes, const std::wstring& name);

	void InputBipedes(Vertex& vertex, const BipedVertex& bipedes);
	void ExportPhysiqueData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* phyMod, const std::wstring& name);
	void ExportSkinData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* skinMod, const std::wstring& name);

	void GetNodeName(INode* node, std::wstring& name);

private:
	SkinExporter* mExporter = nullptr;
};