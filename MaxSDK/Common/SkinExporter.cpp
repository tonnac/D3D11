#include "SkinExporter.h"
#include "SkinWriter.h"
#include "BinWriter.h"


void SkinExporter::LoadObjects()
{
	mNodesLoader = std::make_unique<NodesLoader>(this, true);
	mNodesLoader->LoadObject(mMaxObject, mObjects, mNodeIndex);
}

void SkinExporter::CreateWriter()
{
	if(mIsBinary)
		mWriter = std::make_unique<BinWriter<OutSkinned>>(mVersion, mFilename, mSceneInfo,
			mOutputMaterial, mOutObjects, mSkinnedVertices, mIndices, mSubsets);
	else
		mWriter = std::make_unique<SkinWriter>(mVersion, mFilename, mSceneInfo,
			mOutputMaterial, mOutObjects, mSkinnedVertices, mIndices, mSubsets);
}
       
void SkinExporter::BuildSubset()
{
	static UINT vertices = 0;
	static UINT indices = 0;
	for (auto& x : mObjects)
	{
		if (!x->mSkinVertices.empty())
		{
			for (auto&k : x->mSkinVertices)
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

void SkinExporter::BuildVBIB()
{
	for (auto & mesh : mObjects)
	{
		std::sort(mesh->mSkinTri.begin(), mesh->mSkinTri.end());
		for (int i = 0; i < (int)mesh->mSkinTri.size(); ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				int mTlid = mesh->mSkinTri[i].mSubMtrl;
				auto& vertices = mesh->mSkinVertices[mTlid];
				auto& indices = mesh->mIndices[mTlid];
				int vNumber = [&](SkinnedVertex& rhs) -> int
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
				}(mesh->mSkinTri[i].v[j]);

				if (vNumber == -1)
				{
					vertices.push_back(mesh->mSkinTri[i].v[j]);
					indices.push_back((UINT)(vertices.size() - 1));
				}
				else
				{
					indices.push_back(vNumber);
				}
			}
		}

		for (auto & x : mesh->mSkinVertices)
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

		auto & vertices = mSkinnedVertices;
		auto & Indices = mIndices;

		for (auto &x : mesh->mSkinVertices)
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

				OutSkinned oV(k);
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

				std::uint32_t Min = min(min(i0, i1), i2);

				if (Min == i1)
				{
					std::swap(indices[i * 3 + 0], indices[i * 3 + 1]);
					std::swap(indices[i * 3 + 1], indices[i * 3 + 2]);
				}
				else if (Min == i2)
				{
					std::swap(indices[i * 3 + 0], indices[i * 3 + 2]);
					std::swap(indices[i * 3 + 1], indices[i * 3 + 2]);
				}

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
