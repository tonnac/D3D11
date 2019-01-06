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
	NodesLoader(BaseExporter * exporter, bool isSkinned = false);

public:
	void LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
		std::vector<std::unique_ptr<ZXCObject>>& objectlist,
		std::vector<D3D_MATRIX>& offsets,
		std::unordered_map<std::wstring, size_t>& nodeIndex);
private:
	void LoadMesh(INode* node, ZXCObject* o);
	template<typename X>
	void LoadVertex(const Mesh& mesh, const Matrix3& tm, X* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);
	template<typename X>
	void LoadTexCoord(const Mesh& mesh, const Matrix3& tm, X* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);
	template<typename X>
	void LoadColor(const Mesh& mesh, const Matrix3& tm, X* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);
	template<typename X>
	void LoadNormal(Mesh& mesh, const Matrix3& tm, X* t, int index, std::uint32_t i0, std::uint32_t i1, std::uint32_t i2);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

	void InputOffsets(INode* node, D3D_MATRIX& matrix);

private:
	void LoadBipedInfo(INode* node, std::vector<BipedVertex>& bipedes, const std::wstring& name);

	void InputBipedes(SkinnedVertex& vertex, const BipedVertex& bipedes);
	void ExportPhysiqueData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* phyMod, const std::wstring& name);
	void ExportSkinData(INode* node, std::vector<BipedVertex>& bipedes, Modifier* skinMod, const std::wstring& name);

	void GetNodeName(INode* node, std::wstring& name);

private:
	bool mIsSkinned = false;
	BaseExporter* mExporter = nullptr;
};