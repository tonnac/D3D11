#include "ZXCExporter.h"

ZXCExporter::ZXCExporter()
{
	std::locale::global(std::locale("Korean"));
}

bool ZXCExporter::Initialize(Interface* maxinterface, const MCHAR* Filename, const MCHAR* Longdesc)
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

bool ZXCExporter::Run()
{
	LoadScene();
	LoadNode(mRootNode);
	LoadMaterial();

	mObjectExporter = std::make_unique<ObjectExporter>(this);	
	mObjectExporter->LoadObject(mMaxObject, mObjects, mNodeIndex, mVertices, mIndices);

	BuildOutObjects();
	BuildSubset();

	mWriter = std::make_unique<ZXCWriter>(mVersion, mFilename, mSceneInfo, 
		mOutputMaterial, mOutObjects, mVertices, mIndices, mSubsets);
	if (!mWriter->Savefile()) return false;

	return true;
}

void ZXCExporter::LoadScene()
{
	mSceneInfo.FrameSpeed = GetFrameRate();
	mSceneInfo.TickperFrame = GetTicksPerFrame();
	mSceneInfo.FirstFrame = mInterval.Start() / mSceneInfo.TickperFrame;
	mSceneInfo.LastFrame = mInterval.End() / mSceneInfo.TickperFrame;
}

void ZXCExporter::LoadNode(INode * node)
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

void ZXCExporter::AddObject(INode * node)
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

void ZXCExporter::AddMaterial(INode * node)
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

void ZXCExporter::BuildOutObjects()
{
	for (const auto& p : mObjects)
	{
		OutputObject oO = *p;
		mOutObjects.push_back(oO);
	}
}

void ZXCExporter::BuildSubset()
{
	static UINT vertices = 0;
	static UINT indices = 0;
	for (auto& x : mObjects)
	{
		if (!x->mVertices.empty())
		{
			for (auto&k : x->mVertices)
			{
				Subset subset;
				subset.MtrlRef = x->mMaterialRef;
				subset.NodeIndex = (int)x->mNodeName.second;
				subset.SubMtlID = k.first;
				subset.VertexCount = (UINT)k.second.size();
				subset.VertexStart = vertices;
				subset.FaceCount = (UINT)(x->mIndices[k.first].size()) / 3;
				subset.FaceStart = indices;
				vertices += subset.VertexCount;
				indices += subset.FaceCount;
				mSubsets.push_back(subset);
			}
		}
	}
}

void ZXCExporter::LoadMaterial()
{
	for (int i = 0; i < (int)mMaxMaterial.size(); ++i)
	{
		Mtl* mtl = mMaxMaterial[i];

		ZXCMaterial outMtl;
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

				ZXCMaterial outSubMtl;

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

void ZXCExporter::LoadTexture(ZXCMaterial & zxcMtl, Mtl * srcMtl)
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

		std::pair<int, std::wstring> texMap;
		texMap.first = i;

		if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
		{
			TSTR mapName = ((BitmapTex *)tex)->GetMapName();
			TSTR fullName;
			SplitPathFile(mapName, &fullName, &mapName);
			texMap.second = mapName;
		}
		zxcMtl.TexMap.insert(texMap);
	}
}