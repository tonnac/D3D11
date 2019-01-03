#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class SkinWriter
{
public:
	SkinWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCMaterial>& material,
		std::vector<std::unique_ptr<ZXCObject>>& object);
public:
	bool Savefile();

private:
	void WriteScene(std::wofstream& os);
	void WriteMaterial(std::wofstream& os);
	void WriteMesh(std::wofstream& os);
	void WriteHelper(std::wofstream& os);
	void WriteSubset(std::wofstream& os);
	void WriteVertex(std::wofstream& os);
	void BuildSubset(UINT& meshes, UINT& helpers, UINT& subsets, UINT& vertices, UINT& indices);

	std::wstring Savetime();

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;

	const SceneInfo& mSceneInfo;
	const std::vector<ZXCMaterial>& mMaterial;
	std::vector<std::unique_ptr<ZXCObject>> mObjects;
	std::vector<Subset> mSubset;
};

