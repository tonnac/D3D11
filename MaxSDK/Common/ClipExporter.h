#pragma once

#include "AnimationExporter.h"
#include "ClipBinary.h"

class ClipExporter
{
	friend class AnimationExporter;
public:
	ClipExporter();
	~ClipExporter() = default;

public:
	bool Initialize(Interface* maxinterface, const MCHAR* Filename, const MCHAR* Longdesc, bool isBinary = false);
	bool Run();

private:
	void LoadScene();
	void LoadNode(INode* node);

	void AddObject(INode* node);

private:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;

	SceneInfo	mSceneInfo;

	std::unordered_map<std::wstring, INode*> mMaxObject;
	std::queue<INode*> mQueue;

	std::vector<std::unique_ptr<ZXCObject>> mOutputObjects;
	std::unordered_map<std::wstring, size_t> mNodeIndex;

	std::wstring mVersion;
	std::wstring mFilename;

	std::unique_ptr<AnimationClip> mAnimation = nullptr;
		
	std::unique_ptr<ClipWriter> mWriter = nullptr;

	bool mIsBinary = false;
};