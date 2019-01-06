#pragma once

#include "ZXCWriter.h"
#include "BinWriter.h"
#include "NodesLoader.h"

class BaseExporter
{
	friend class NodesLoader;
public:
	BaseExporter();
	~BaseExporter() = default;

public:
	bool Initialize(Interface* maxinterface, const MCHAR* Filename ,const MCHAR* Longdesc, bool isBinary = false);
	bool Run();

private:
	void LoadScene();
	void LoadNode(INode* node);
	void LoadMaterial();
	void LoadTexture(ZXCMaterial& zxcMtl, Mtl* srcMtl);

	void AddObject(INode* node);
	void AddMaterial(INode* node);

	void BuildOutObjects();

	virtual void LoadObjects();
	virtual void CreateWriter();
	virtual void BuildSubset();
	virtual void BuildVBIB();

protected:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;

	SceneInfo	mSceneInfo;

	std::vector<Mtl*> mMaxMaterial;
	std::unordered_map<std::wstring, INode*> mMaxObject;
	std::queue<INode*> mQueue;

	std::vector<std::unique_ptr<ZXCObject>> mObjects;
	std::unordered_map<std::wstring, size_t> mNodeIndex;
	std::vector<D3D_MATRIX> mOffsets;

	std::wstring mVersion;
	std::wstring mFilename;

	std::vector<OutVertex> mVertices;
	std::vector<OutSkinned> mSkinnedVertices;

	std::vector<ZXCMaterial> mOutputMaterial;
	std::vector<std::uint32_t> mIndices;
	std::vector<Subset> mSubsets;
	std::vector<OutputObject> mOutObjects;

	std::unique_ptr<Writer> mWriter = nullptr;
	std::unique_ptr<NodesLoader> mNodesLoader = nullptr;

	bool mIsBinary = false;
};