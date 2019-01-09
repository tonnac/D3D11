#include "SkinExporter.h"
#include "SkinWriter.h"
#include "BinWriter.h"


void SkinExporter::LoadObjects()
{
	mNodesLoader = std::make_unique<NodesLoader>(this, true);
	mNodesLoader->LoadObject(mMaxObject, mObjects, mOffsets, mNodeIndex);
}

void SkinExporter::CreateWriter()
{
	if(mIsBinary)
		mWriter = std::make_unique<BinWriter<OutSkinned>>(mVersion, mFilename, mOutputMaterial, mOutObjects, mSkinnedVertices, mIndices, mSubsets, mOffsets);
	else
		mWriter = std::make_unique<SkinWriter>(mVersion, mFilename, mOutputMaterial, mOutObjects, mSkinnedVertices, mIndices, mSubsets, mOffsets);
}
       
void SkinExporter::BuildSubset()
{
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
				subset.VertexStart = mNumVertices;
				subset.FaceCount = (UINT)(x->mIndices[k.first].size()) / 3;
				subset.FaceStart = mNumIndices;
				mNumVertices += subset.VertexCount;
				mNumIndices += subset.FaceCount;
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
					vertices[vNumber].Tangent += mesh->mSkinTri[i].v[j].Tangent;
					vertices[vNumber].Bitangent += mesh->mSkinTri[i].v[j].Bitangent;
					indices.push_back(vNumber);
				}
			}
		}

		auto & vertices = mSkinnedVertices;
		auto & Indices = mIndices;

		for (auto &x : mesh->mSkinVertices)
		{
			for (auto & k : x.second)
			{
				k.Tangent = (k.Tangent - DotProd(k.Tangent, k.n) * k.n).FNormalize();

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
