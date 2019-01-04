#include "ClipExporter.h"

ClipExporter::ClipExporter()
{
	std::locale::global(std::locale("Korean"));
}

bool ClipExporter::Initialize(Interface* maxinterface, const MCHAR* Filename, const MCHAR* Longdesc, bool isBinary)
{
	mMaxInterface = maxinterface;
	mRootNode = mMaxInterface->GetRootNode();
	mVersion = Longdesc;

	if (mRootNode == nullptr && maxinterface == nullptr) return false;

	mFilename = Filename;
	mFilename.assign(mFilename, mFilename.find_last_of('\\') + 1, mFilename.length());
	mInterval = mMaxInterface->GetAnimRange();

	return true;
}

bool ClipExporter::Run()
{
	LoadScene();
	LoadNode(mRootNode);

	mAnimation = AnimationExporter::LoadAnimation(mMaxObject, mNodeIndex, mInterval.Start(), mInterval.End());

	mWriter = std::make_unique<ClipWriter>(mVersion, mFilename, mSceneInfo, mAnimation.get());
	if (!mWriter->Savefile()) return false;

	return true;
}

void ClipExporter::LoadScene()
{
	mSceneInfo.FrameSpeed = GetFrameRate();
	mSceneInfo.TickperFrame = GetTicksPerFrame();
	mSceneInfo.FirstFrame = mInterval.Start() / mSceneInfo.TickperFrame;
	mSceneInfo.LastFrame = mInterval.End() / mSceneInfo.TickperFrame;
}

void ClipExporter::LoadNode(INode * node)
{
	if (node == nullptr) return;

	int ChildNum = node->NumberOfChildren();
	for (int i = 0; i < ChildNum; ++i)
	{
		INode* childNode = node->GetChildNode(i);
		mQueue.push(childNode);
	}

	if (node->Selected())
	{
		AddObject(node);
	}

	while (!mQueue.empty())
	{
		INode * fNode = mQueue.front();
		mQueue.pop();
		LoadNode(fNode);
	}
}

void ClipExporter::AddObject(INode * node)
{
	ObjectState os = node->EvalWorldState(mInterval.Start());

	if (os.obj == nullptr || os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
	{
		return;
	}

	std::wstring nodeName = node->GetName();
	switch (os.obj->SuperClassID())
	{
		
		case GEOMOBJECT_CLASS_ID:
		case HELPER_CLASS_ID:
		{
			auto iter = mMaxObject.find(nodeName);
			if (iter != mMaxObject.end())
			{
				nodeName += L'_';
			}
			mNodeIndex[nodeName] = mNodeIndex.size();
			mMaxObject.insert(std::make_pair(nodeName, node));
		}break;
	}
}