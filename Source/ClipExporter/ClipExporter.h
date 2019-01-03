#pragma once

#include "ObjectExporter.h"
#include "ClipWriter.h"

class ClipExporter
{
	friend class ObjectExporter;
public:
	ClipExporter();
	~ClipExporter() = default;

public:
	bool Initialize(Interface* maxinterface, const MCHAR* Filename ,const MCHAR* Longdesc);
	bool Run();

private:
	void LoadScene();
	void LoadNode(INode* node);
	void LoadMaterial();
	void LoadTexture(ZXCSMaterial& ZXCSMtl, Mtl* srcMtl);

	void AddObject(INode* node);
	void AddMaterial(INode* node);

private:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;

	SceneInfo	mSceneInfo;

	std::vector<Mtl*> mMaxMaterial;
	std::unordered_map<std::wstring, INode*> mMaxObject;
	std::queue<INode*> mQueue;

	std::vector<ZXCSMaterial> mOutputMaterial;
	std::vector<std::unique_ptr<ZXCSObject>> mOutputObjects;
	std::unordered_map<std::wstring, size_t> mNodeIndex;

	std::wstring mVersion;
	std::wstring mFilename;

	std::unique_ptr<ClipWriter> mWriter = nullptr;
	std::unique_ptr<ObjectExporter> mObjectExporter = nullptr;
};