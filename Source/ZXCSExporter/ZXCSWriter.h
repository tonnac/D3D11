#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class ZXCSWriter
{
public:
	ZXCSWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCSMaterial>& material,
		std::vector<std::unique_ptr<ZXCSObject>>& object,
		AnimationClip * clip);
public:
	bool Savefile();

private:
	void WriteScene(std::wofstream& os);
	void WriteMaterial(std::wofstream& os);
	void WriteMesh(std::wofstream& os);
	void WriteHelper(std::wofstream& os);
	void WriteVertexW(std::wofstream& os);
	void WriteAnimations(std::wofstream& os);

	std::wstring Savetime();

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;

	const SceneInfo& mSceneInfo;
	const std::vector<ZXCSMaterial>& mMaterial;
	std::vector<std::unique_ptr<ZXCSObject>> mObjects;
	AnimationClip * mAnimation = nullptr;
};

