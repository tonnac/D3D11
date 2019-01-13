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
	virtual void BuildBoundingBox();

	void IBReorder();

protected:
	void ComparePoint3(Point3& dest, const Point3& src, bool isGreater = true);

protected:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;
	
	std::queue<INode*> mQueue;

	std::vector<Mtl*> mMaxMaterial;
	std::unordered_map<std::wstring, INode*> mMaxObject;
	std::vector<std::unique_ptr<ZXCObject>> mObjects;
	std::unordered_map<std::wstring, size_t> mNodeIndex;

	OutputData mOutData;

	std::unique_ptr<Writer> mWriter = nullptr;
	std::unique_ptr<NodesLoader> mNodesLoader = nullptr;

	UINT mNumVertices = 0;
	UINT mNumIndices = 0;
	bool mIsBinary = false;
};