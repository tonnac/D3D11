#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class ClipWriter
{
	struct Subset
	{
		int NodeIndex;
		int MtrlRef;
		int SubMtlID;

		UINT VertexStart;
		UINT VertexCount;
		UINT FaceStart;
		UINT FaceCount;
	};
public:
	ClipWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCSMaterial>& material,
		std::vector<std::unique_ptr<ZXCSObject>>& object);
public:
	bool Savefile();

private:
	void WriteScene(std::wofstream& os);
	void WriteMaterial(std::wofstream& os);
	void WriteMesh(std::wofstream& os);
	void WriteHelper(std::wofstream& os);
	void WriteSubset(std::wofstream& os);
	void WriteVertexW(std::wofstream& os);
	void BuildSubset(UINT& meshes, UINT& helpers, UINT& subsets, UINT& vertices, UINT& indices);

	std::wstring Savetime();

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;

	const SceneInfo& mSceneInfo;
	const std::vector<ZXCSMaterial>& mMaterial;
	std::vector<std::unique_ptr<ZXCSObject>> mObjects;
	std::vector<Subset> mSubset;
};

