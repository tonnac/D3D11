#pragma once

#include "AnimationExporter.h"

class ZXCSExporter;

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
	ObjectExporter(ZXCSExporter * exporter);

public:
	void LoadObject(std::unordered_map<std::wstring, INode*>& nodes, 
		std::vector<std::unique_ptr<ZXCSObject>>& objectlist,
		std::unordered_map<std::wstring, size_t>& nodeIndex);

private:
	void LoadMesh(INode* node, ZXCSObject* o);
	void BuildVBIB(ZXCSObject* mesh);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexWNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

private:
	int FindIndex(int faceIndex, int vertIndex, int faceVertIndex);

	void LoadBipedInfo(INode* node, std::vector<BipedVertex>& bipedes, const std::wstring& name);

	void InputBipedes(VertexW& vertex, const BipedVertex& bipedes);
	void ExportPhysiqueData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* phyMod, const std::wstring& name);
	void ExportSkinData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* skinMod, const std::wstring& name);

	void GetNodeName(INode* node, std::wstring& name);
private:
	ZXCSExporter* mExporter = nullptr;

	
};