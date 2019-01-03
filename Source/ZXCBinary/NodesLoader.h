#pragma once

#include "..\..\MaxSDK\Common\Header.h"

class BaseExporter;

class NodesLoader
{
public:
	NodesLoader(BaseExporter * exporter);

public:
	void LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
		std::vector<std::unique_ptr<ZXCObject>>& objectlist,
		std::unordered_map<std::wstring, size_t>& nodeIndex,
		std::vector<OutVertex>& vertices,
		std::vector<std::uint32_t>& indices);

protected:
	void LoadMesh(INode* node, ZXCObject* o);
	void BuildVBIB(ZXCObject* mesh, std::vector<OutVertex>& vertices, std::vector<std::uint32_t>& indices);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

protected:
	BaseExporter* mExporter = nullptr;
};