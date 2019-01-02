#pragma once

#include "ObjectExporter.h"
#include "ZXCWriter.h"

class ZXCExporter
{
	friend class ObjectExporter;
public:
	ZXCExporter();
	~ZXCExporter() = default;

public:
	bool Initialize(Interface* maxinterface, const MCHAR* Filename ,const MCHAR* Longdesc);
	bool Run();

private:
	void LoadScene();
	void LoadNode(INode* node);
	void LoadMaterial();
	void LoadTexture(ZXCMaterial& zxcMtl, Mtl* srcMtl);

	void AddObject(INode* node);
	void AddMaterial(INode* node);

	void BuildOutObjects();
	void BuildSubset();

private:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;

	SceneInfo	mSceneInfo;

	std::vector<Mtl*> mMaxMaterial;
	std::unordered_map<std::wstring, INode*> mMaxObject;
	std::queue<INode*> mQueue;

	std::vector<ZXCMaterial> mOutputMaterial;
	std::vector<std::unique_ptr<ZXCObject>> mObjects;
	std::vector<OutputObject> mOutObjects;
	std::unordered_map<std::wstring, size_t> mNodeIndex;

	std::wstring mVersion;
	std::wstring mFilename;

	std::vector<OutVertex> mVertices;
	std::vector<std::uint32_t> mIndices;
	std::vector<Subset> mSubsets;

	std::unique_ptr<ZXCWriter> mWriter = nullptr;
	std::unique_ptr<ObjectExporter> mObjectExporter = nullptr;
};