#include "ZXCSExporter.h"

ZXCSExporter::ZXCSExporter()
{
	std::locale::global(std::locale("Korean"));
}

bool ZXCSExporter::Initialize(Interface* maxinterface, const MCHAR* Filename, const MCHAR* Longdesc)
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

bool ZXCSExporter::Run()
{
	LoadScene();
	LoadNode(mRootNode);
	LoadMaterial();

	mObjectExporter = std::make_unique<ObjectExporter>(this);	
	mObjectExporter->LoadObject(mMaxObject, mOutputObjects, mNodeIndex);

	mAnimation = AnimationExporter::LoadAnimation(mMaxObject, mNodeIndex, mInterval.Start(), mInterval.End());

	mWriter = std::make_unique<ZXCSWriter>(mVersion, mFilename, mSceneInfo, 
		mOutputMaterial, mOutputObjects, mAnimation.get());
	if (!mWriter->Savefile()) return false;

	return true;
}

void ZXCSExporter::LoadScene()
{
	mSceneInfo.FrameSpeed = GetFrameRate();
	mSceneInfo.TickperFrame = GetTicksPerFrame();
	mSceneInfo.FirstFrame = mInterval.Start() / mSceneInfo.TickperFrame;
	mSceneInfo.LastFrame = mInterval.End() / mSceneInfo.TickperFrame;
}

void ZXCSExporter::LoadNode(INode * node)
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
		AddMaterial(node);
	}

	while (!mQueue.empty())
	{
		INode * fNode = mQueue.front();
		mQueue.pop();
		LoadNode(fNode);
	}
}

void ZXCSExporter::AddObject(INode * node)
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

void ZXCSExporter::AddMaterial(INode * node)
{
	Mtl* mtl = node->GetMtl();
	if (mtl == nullptr)
	{
		return;
	}

	for (int i = 0; i < mMaxMaterial.size(); ++i)
	{
		if (mMaxMaterial[i] == mtl)
		{
			return;
		}
	}
	mMaxMaterial.push_back(mtl);
}


void ZXCSExporter::LoadMaterial()
{
	for (int i = 0; i < (int)mMaxMaterial.size(); ++i)
	{
		Mtl* mtl = mMaxMaterial[i];

		ZXCSMaterial outMtl;
		TSTR className;

		mtl->GetClassName(className);
		outMtl.Name = mtl->GetName();
		std::ReplaceString(outMtl.Name);
		outMtl.ClassName = className;
		outMtl.Ambient = mtl->GetAmbient();
		outMtl.Diffuse = mtl->GetDiffuse();
		outMtl.Specular = mtl->GetSpecular();
		outMtl.Shininess = mtl->GetShininess();
		outMtl.SubMaterial.resize(mtl->NumSubMtls());

		if (!outMtl.SubMaterial.empty())
		{
			for (int k = 0; k < (int)outMtl.SubMaterial.size(); ++k)
			{
				Mtl* subMtl = mtl->GetSubMtl(k);
				
				ZXCSMaterial outSubMtl;

				TSTR subClassname;
				outSubMtl.Name = subMtl->GetName();
				outSubMtl.Ambient = subMtl->GetAmbient();
				outSubMtl.Diffuse = subMtl->GetDiffuse();
				outSubMtl.Specular = subMtl->GetSpecular();
				outSubMtl.Shininess = subMtl->GetShininess();

				std::ReplaceString(outSubMtl.Name);
				subMtl->GetClassName(subClassname);
				outSubMtl.ClassName = subClassname;

				if (subMtl != nullptr)
				{
					LoadTexture(outSubMtl, subMtl);
				}
				outMtl.SubMaterial[k] = outSubMtl;
			}
		}
		else
		{
			LoadTexture(outMtl, mtl);
		}
		mOutputMaterial.push_back(outMtl);
	}
}

void ZXCSExporter::LoadTexture(ZXCSMaterial & ZXCSMtl, Mtl * srcMtl)
{
	for (int i = 0; i < srcMtl->NumSubTexmaps(); ++i)
	{
		Texmap* tex = srcMtl->GetSubTexmap(i);
		if (tex == nullptr)	continue;

		if (srcMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
		{
			StdMat * std = (StdMat*)srcMtl;
			if (!std->MapEnabled(i)) continue;
		}

		ZXCSTexmap texMap;
		texMap.SubNo = i;

		if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
		{
			TSTR mapName = ((BitmapTex *)tex)->GetMapName();
			TSTR fullName;
			SplitPathFile(mapName, &fullName, &mapName);
			texMap.Filename = mapName;
		}
		ZXCSMtl.TexMap.push_back(texMap);
	}
}