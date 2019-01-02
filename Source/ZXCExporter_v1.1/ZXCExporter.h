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

private:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;

	SceneInfo	mSceneInfo;

	std::vector<Mtl*> mMaxMaterial;
	std::vector<INode*> mMaxObjects;
	std::queue<INode*> mQueue;

	std::vector<ZXCMaterial> mOutputMaterial;
	std::vector<std::unique_ptr<ZXCObject>> mOutputObjects;
	std::unique_ptr<AnimationClip> mAnimation = nullptr;
	std::unordered_map<std::wstring, size_t> mNodeIndex;

	std::wstring mVersion;
	std::wstring mFilename;

	std::unique_ptr<ZXCWriter> mWriter = nullptr;
	std::unique_ptr<ObjectExporter> mObjectExporter = nullptr;
};