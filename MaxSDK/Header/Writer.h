#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class Writer
{
public:
	Writer(OutputData& outData);
public:
	virtual bool Savefile();

protected:
	void SaveMaterial(std::wofstream& os);
	void SaveNodes(std::wofstream& os);
	void SaveSubset(std::wofstream& os);
	void SaveBoundingBox(std::wofstream& os);
	virtual void SaveVertices(std::wofstream& os);
	void SaveIndices(std::wofstream& os);

protected:
	OutputData& mOutData;

	UINT mNumMaterials = 0;
	UINT mNumVertices = 0;
	UINT mNumTriangles = 0;
	UINT mNumNodes = 0;
	UINT mNumSubsets = 0;
};

