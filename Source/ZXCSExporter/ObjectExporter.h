#pragma once

#include "AnimationExporter.h"

class ZXCSExporter;

struct BipedVertex
{
	std::wstring mNodename;
	int			 mNodeIndex = -1;
	int			 mNumWeight = -1;

	std::map<float, int, std::greater<float>>	 mWeightList;
	std::map<float, int, std::greater<float>>::const_iterator iter;
};

class ObjectExporter
{
public:
	ObjectExporter(ZXCSExporter * exporter);

public:
	void LoadObject(INode ** nodearray, size_t arraysize, 
		std::vector<std::unique_ptr<ZXCSObject>>& objectlist,
		std::unordered_map<std::wstring, size_t>& nodeIndex);

private:
	void LoadMesh(INode* node, ZXCSObject* o);
	void BuildVBIB(ZXCSObject* mesh);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexWNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

private:
	void LoadBipedInfo(INode* node, std::vector<BipedVertex>& bipedes);

	void InputBipedes(VertexW& vertex, const BipedVertex& bipedes);
	void ExportPhysiqueData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* phyMod);
	void ExportSkinData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* skinMod);

private:
	ZXCSExporter* mExporter = nullptr;
};