#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class Writer
{
public:
	Writer(const OutputData& outData);
public:
	virtual bool Savefile() = 0;

protected:

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;

	const std::vector<ZXCMaterial>& mMaterial;
	const std::vector<Subset>& mSubsets;
	const std::vector<OutputObject>& mObjects;
	const std::vector<std::uint32_t>& mIndices;

	UINT mNumMaterials = 0;
	UINT mNumVertices = 0;
	UINT mNumTriangles = 0;
	UINT mNumNodes = 0;
	UINT mNumSubsets = 0;
};

