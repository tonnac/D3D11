#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class ZXCWriter
{
public:
	ZXCWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCMaterial>& material,
		const std::vector<OutputObject>& object, 
		const std::vector<OutVertex>& vertices,
		const std::vector<std::uint32_t>& indices,
		const std::vector<Subset>& subsets);
public:
	bool Savefile();

private:
	void SaveScene(std::ofstream& os);
	void SaveMaterial(std::ofstream& os);
	void SaveNodes(std::ofstream& os);
	void SaveSubset(std::ofstream& os);
	void SaveVertices(std::ofstream& os);
	void SaveIndices(std::ofstream& os);

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;

	const SceneInfo& mSceneInfo;
	const std::vector<ZXCMaterial>& mMaterial;
	const std::vector<Subset> mSubsets;
	const std::vector<OutputObject>& mObjects;
	const std::vector<OutVertex>& mVertices;
	const std::vector<std::uint32_t>& mIndices;
};

