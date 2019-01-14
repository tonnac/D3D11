#pragma once

#include "Header.h"

struct BipedVertex
{
	std::wstring mNodename;
	int			 mNodeIndex = -1;
	int			 mNumWeight = -1;

	std::multimap<float, int, std::greater<float>>	 mWeightList;
	std::multimap<float, int, std::greater<float>>::const_iterator iter;
};


class BaseExporter;

class NodesLoader
{
public:
	NodesLoader(BaseExporter * exporter);

public:
	void LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
		std::vector<std::unique_ptr<ZXCObject>>& objectlist,
		std::unordered_map<std::wstring, size_t>& nodeIndex);
private:
	void LoadMesh(INode* node, ZXCObject* o);
	void LoadVertex(const Mesh& mesh, const Matrix3& tm, VertexTri* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);
	void LoadTexCoord(const Mesh& mesh, const Matrix3& tm, VertexTri* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);
	void LoadColor(const Mesh& mesh, const Matrix3& tm, VertexTri* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);
	void LoadNormal(Mesh& mesh, const Matrix3& tm, VertexTri* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);
	void LoadBiNormal(Mesh& mesh, VertexTri* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

//	void InputOffsets(INode* node, D3D_MATRIX& matrix);

private:
#ifdef SKINNED
	void LoadBipedInfo(INode* node, std::vector<BipedVertex>& bipedes, const std::wstring& name);

	void InputBipedes(Vertex& vertex, const BipedVertex& bipedes);
	void ExportPhysiqueData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* phyMod, const std::wstring& name);
	void ExportSkinData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* skinMod, const std::wstring& name);

	void GetNodeName(INode* node, std::wstring& name);
#endif
private:
	BaseExporter* mExporter = nullptr;
};