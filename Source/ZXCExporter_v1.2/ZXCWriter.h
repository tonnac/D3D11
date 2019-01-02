#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class ZXCWriter
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
	ZXCWriter(const std::wstring& ExporterVersion,
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
	std::vector<Subset> mSubset;
	std::vector<std::unique_ptr<ZXCObject>> mObjects;
};

