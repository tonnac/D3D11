#include "BaseExporter.h"

BaseExporter::BaseExporter()
{
	std::locale::global(std::locale("Korean"));
}

bool BaseExporter::Initialize(Interface* maxinterface, const MCHAR* Filename, const MCHAR* Longdesc, bool isBinary)
{
	mMaxInterface = maxinterface;
	mRootNode = mMaxInterface->GetRootNode();
	mVersion = Longdesc;
	mIsBinary = isBinary;

	if (mRootNode == nullptr && maxinterface == nullptr) return false;

	mFilename = Filename;
	mFilename.assign(mFilename, mFilename.find_last_of('\\') + 1, mFilename.length());
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


	CreateWriter();

	if (!mWriter->Savefile()) return false;

	return true;
}

void BaseExporter::LoadScene()
{
	mSceneInfo.FrameSpeed = GetFrameRate();
	mSceneInfo.TickperFrame = GetTicksPerFrame();
	mSceneInfo.FirstFrame = mInterval.Start() / mSceneInfo.TickperFrame;
	mSceneInfo.LastFrame = mInterval.End() / mSceneInfo.TickperFrame;
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
		mOutObjects.push_back(oO);
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
		mWriter = std::make_unique<BinWriter<>>(mVersion, mFilename, mOutputMaterial, mOutObjects, mVertices, mIndices, mSubsets);
	else
		mWriter = std::make_unique<ZXCWriter>(mVersion, mFilename, mOutputMaterial, mOutObjects, mVertices, mIndices, mSubsets);
}

void BaseExporter::BuildSubset()
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
					indices.push_back(vNumber);
				}
			}
		}

		for (auto & x : mesh->mVertices)
		{
			auto & k = mesh->mIndices[x.first];

			for (UINT i = 0; i < (UINT)k.size() / 3; ++i)
			{
				std::uint32_t i0 = k[i * 3 + 0];
				std::uint32_t i1 = k[i * 3 + 1];
				std::uint32_t i2 = k[i * 3 + 2];

				auto& v0 = x.second[i0].p;
				auto& v1 = x.second[i1].p;
				auto& v2 = x.second[i2].p;

				auto& t0 = x.second[i0].t;
				auto& t1 = x.second[i1].t;
				auto& t2 = x.second[i2].t;

				Point3 e0 = v1 - v0;
				Point3 e1 = v2 - v0;

				Point2 UV0 = t1 - t0;
				Point2 UV1 = t2 - t0;

				float a = UV0.x;
				float b = UV0.y;
				float c = UV1.x;
				float d = UV1.y;

				float r = 1.0f / (a * d - b * c);

				Point3 Tangent;
				Point3 Bitangent;

				if (fabsf(a * d - b * c) < Epsilon)
				{
					Tangent = Point3(1.0f, 0.0f, 0.0f);
					Bitangent = Point3(0.0f, 1.0f, 0.0f);
				}
				else
				{
					Tangent = r * (d * e0 - b * e1);
					Bitangent = r * (a * e1 - c * e0);
				}

				x.second[i0].Tangent += Tangent;
				x.second[i0].Bitangent += Bitangent;

				////////

				e0 = v0 - v1;
				e1 = v2 - v1;

				UV0 = t0 - t1;
				UV1 = t2 - t1;

				a = UV0.x;
				b = UV0.y;
				c = UV1.x;
				d = UV1.y;

				r = 1.0f / (a * d - b * c);

				if (fabsf(a * d - b * c) < Epsilon)
				{
					Tangent = Point3(1.0f, 0.0f, 0.0f);
					Bitangent = Point3(0.0f, 1.0f, 0.0f);
				}
				else
				{
					Tangent = r * (d * e0 - b * e1);
					Bitangent = r * (a * e1 - c * e0);
				}

				x.second[i1].Tangent += Tangent;
				x.second[i1].Bitangent += Bitangent;

				////////

				e0 = v0 - v2;
				e1 = v1 - v2;

				UV0 = t0 - t2;
				UV1 = t1 - t2;

				a = UV0.x;
				b = UV0.y;
				c = UV1.x;
				d = UV1.y;

				r = 1.0f / (a * d - b * c);

				if (fabsf(a * d - b * c) < Epsilon)
				{
					Tangent = Point3(1.0f, 0.0f, 0.0f);
					Bitangent = Point3(0.0f, 1.0f, 0.0f);
				}
				else
				{
					Tangent = r * (d * e0 - b * e1);
					Bitangent = r * (a * e1 - c * e0);
				}

				x.second[i2].Tangent += Tangent;
				x.second[i2].Bitangent += Bitangent;
			}
		}

		auto & vertices = mVertices;
		auto & Indices = mIndices;

		for (auto &x : mesh->mVertices)
		{
			for (auto & k : x.second)
			{
				k.Tangent = k.Tangent.Normalize();
				k.Bitangent = k.Bitangent.Normalize();

				k.Tangent = k.Tangent - DotProd(k.Tangent, k.n) * k.n;

				if (DotProd(CrossProd(k.n, k.Tangent), k.Bitangent) < 0.0f)
				{
					k.Tangent *= -1;
				}

				OutVertex oV = k;
				vertices.push_back(oV);
			}
		}

		for (auto &x : mesh->mIndices)
		{
			auto& indices = x.second;
			for (int i = 0; i < (int)indices.size() / 3; ++i)
			{
				std::uint32_t i0 = indices[i * 3 + 0];
				std::uint32_t i1 = indices[i * 3 + 1];
				std::uint32_t i2 = indices[i * 3 + 2];

				for (int k = i; k >= 1; --k)
				{
					std::uint32_t n0 = indices[k * 3 + 0];
					std::uint32_t n1 = indices[k * 3 + 1];
					std::uint32_t n2 = indices[k * 3 + 2];

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
		mOutputMaterial.push_back(outMtl);
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