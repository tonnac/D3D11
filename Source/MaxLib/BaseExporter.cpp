#include "BaseExporter.h"

BaseExporter::BaseExporter()
{
	std::locale::global(std::locale("Korean"));
}

bool BaseExporter::Initialize(Interface* maxinterface, const MCHAR* Filename, const MCHAR* Longdesc, bool isBinary)
{
	mMaxInterface = maxinterface;
	mRootNode = mMaxInterface->GetRootNode();
	mOutData.Version = Longdesc;
	mIsBinary = isBinary;

	if (mRootNode == nullptr && maxinterface == nullptr) return false;

	mOutData.Filename = Filename;
	mOutData.Filename.assign(mOutData.Filename, mOutData.Filename.find_last_of('\\') + 1, mOutData.Filename.length());
	mInterval = mMaxInterface->GetAnimRange();

	return true;
}

bool BaseExporter::Run()
{
	LoadScene();
	LoadNode(mRootNode);
	LoadMaterial();
	LoadObjects();

	BuildOutObjects();
	BuildVBIB();
	BuildSubset();
	IBReorder();
	BuildBoundingBox();

	CreateWriter();

	if (!mWriter->Savefile()) return false;

	return true;
}

void BaseExporter::LoadScene()
{
	auto & x = mOutData.SceneInfo;

	x.FrameSpeed = GetFrameRate();
	x.TickperFrame = GetTicksPerFrame();
	x.FirstFrame = mInterval.Start() / x.TickperFrame;
	x.LastFrame = mInterval.End() / x.TickperFrame;
}

void BaseExporter::LoadNode(INode * node)
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

void BaseExporter::AddObject(INode * node)
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

void BaseExporter::AddMaterial(INode * node)
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

void BaseExporter::BuildOutObjects()
{
	for (const auto& p : mObjects)
	{
		OutputObject oO = *p;
		mOutData.OutObjects.push_back(oO);
	}
}

void BaseExporter::LoadObjects()
{
	mNodesLoader = std::make_unique<NodesLoader>(this);
	mNodesLoader->LoadObject(mMaxObject, mObjects, mNodeIndex);
}

void BaseExporter::CreateWriter()
{
	if (mIsBinary)
		mWriter = std::make_unique<BinWriter>(mOutData);
	else
		mWriter = std::make_unique<Writer>(mOutData);
}

void BaseExporter::BuildSubset()
{
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
				subset.VertexStart = mNumVertices;
				subset.FaceCount = (UINT)(x->mIndices[k.first].size()) / 3;
				subset.FaceStart = mNumIndices;
				mNumVertices += subset.VertexCount;
				mNumIndices += subset.FaceCount;
				mOutData.Subsets.push_back(subset);
			}
		}
	}
}

void BaseExporter::BuildVBIB()
{
	for (auto & mesh : mObjects)
	{
		std::sort(mesh->mTriangles.begin(), mesh->mTriangles.end());
		for (int i = 0; i < (int)mesh->mTriangles.size(); ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				int mTlid = mesh->mTriangles[i].mSubMtrl;
				auto& vertices = mesh->mVertices[mTlid];
				auto& indices = mesh->mIndices[mTlid];
				int vNumber = [&](Vertex& rhs) -> int
				{
					for (int k = 0; k < (int)vertices.size(); ++k)
					{
						auto verts = vertices[k];
						if (verts == rhs)
						{
							return k;
						}
					}
					return -1;
				}(mesh->mTriangles[i].v[j]);

				if (vNumber == -1)
				{
					vertices.push_back(mesh->mTriangles[i].v[j]);
					indices.push_back((UINT)(vertices.size() - 1));
				}
				else
				{
					vertices[vNumber].Tangent += mesh->mTriangles[i].v[j].Tangent;
					vertices[vNumber].Bitangent += mesh->mTriangles[i].v[j].Bitangent;
					indices.push_back(vNumber);
				}
			}
		}

		auto & vertices = mOutData.Vertices;
		auto & Indices = mOutData.Indices;

		for (auto &x : mesh->mVertices)
		{
			for (auto & k : x.second)
			{
				k.Tangent = (k.Tangent - DotProd(k.Tangent, k.n) * k.n).FNormalize();

				if (DotProd(CrossProd(k.n, k.Tangent), k.Bitangent) < 0.0f)
				{
					k.Tangent *= -1;
				}

				vertices.push_back(OutVertex(k));
			}
		}

		for (auto &x : mesh->mIndices)
		{
			auto& indices = x.second;
			for (int i = 0; i < (int)indices.size() / 3; ++i)
			{
				for (int k = i; k >= 1; --k)
				{
					std::uint32_t n0 = indices[k * 3 + 0];
					std::uint32_t n1 = indices[k * 3 + 1];
					std::uint32_t n2 = indices[k * 3 + 2];

					if (n2 < n1 && n2 < n0)
					{
						std::swap(indices[k * 3 + 0], indices[k * 3 + 2]);
						std::swap(indices[k * 3 + 1], indices[k * 3 + 2]);
					}
					else if (n1 < n0 && n1 < n2)
					{
						std::swap(indices[k * 3 + 0], indices[k * 3 + 1]);
						std::swap(indices[k * 3 + 1], indices[k * 3 + 2]);
					}

					n0 = indices[k * 3 + 0];
					n1 = indices[k * 3 + 1];
					n2 = indices[k * 3 + 2];

					std::uint32_t p0 = indices[(k - 1) * 3 + 0];
					std::uint32_t p1 = indices[(k - 1) * 3 + 1];
					std::uint32_t p2 = indices[(k - 1) * 3 + 2];

					if (n0 < p0)
					{
						std::swap(indices[(k - 1) * 3 + 0], indices[k * 3 + 0]);
						std::swap(indices[(k - 1) * 3 + 1], indices[k * 3 + 1]);
						std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
						continue;
					}

					if (n0 == p0 && n1 < p1)
					{
						std::swap(indices[(k - 1) * 3 + 1], indices[k * 3 + 1]);
						std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
						continue;
					}

					if (n0 == p0 && n1 == p1 && n2 < p2)
					{
						std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
						continue;
					}
					break;
				}
			}
			Indices.insert(Indices.end(), indices.begin(), indices.end());
		}
	}
}

void BaseExporter::BuildBoundingBox()
{
	Point3 vMax = Point3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Point3 vMin = Point3(FLT_MAX, FLT_MAX, FLT_MAX);

	for (const auto& x : mOutData.Vertices)
	{
		ComparePoint3(vMax, x.p);
		ComparePoint3(vMin, x.p, false);
	}

	mOutData.Box.Center = (vMax + vMin) * 0.5f;
	mOutData.Box.Extents = (vMax - vMin) * 0.5f;
}

void BaseExporter::IBReorder()
{
	for (auto&x : mOutData.Subsets)
	{
		UINT start = x.VertexStart;
		UINT countstart = x.FaceStart;
		UINT count = x.FaceCount;
		for (UINT i = countstart * 3; i < (count + countstart) * 3; ++i)
		{
			mOutData.Indices[i] += start;
		}
		x.VertexStart = 0;
	}

}

void BaseExporter::ComparePoint3(Point3 & dest, const Point3 & src, bool isGreater)
{
	if (isGreater)
	{
		dest.x = max(dest.x, src.x);
		dest.y = max(dest.y, src.y);
		dest.z = max(dest.z, src.z);
	}
	else
	{
		dest.x = min(dest.x, src.x);
		dest.y = min(dest.y, src.y);
		dest.z = min(dest.z, src.z);
	}
}

void BaseExporter::LoadMaterial()
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
		mOutData.Materials.push_back(outMtl);
	}
}

void BaseExporter::LoadTexture(ZXCMaterial & zxcMtl, Mtl * srcMtl)
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