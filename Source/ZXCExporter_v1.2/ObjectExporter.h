#pragma once

#include "Header.h"

class ZXCExporter;

class ObjectExporter
{
public:
	ObjectExporter(ZXCExporter * exporter);

public:
	void LoadObject(std::unordered_map<std::wstring, INode*>& nodes,
		std::vector<std::unique_ptr<ZXCObject>>& objectlist,
		std::unordered_map<std::wstring, size_t>& nodeIndex);

private:
	void LoadMesh(INode* node, ZXCObject* o);
	void BuildVBIB(ZXCObject* mesh);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

private:
	ZXCExporter* mExporter = nullptr;

};