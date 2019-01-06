#include "ZXCLoader.h"
#include "DxState.h"
#include "Mesh.h"

using namespace DirectX;

bool ZXCLoader::LoadZXC(
	const std::wstring & FileName,
	std::vector<Vertex>& vertices,
	std::vector<DWORD>& indices,
	std::vector<Subset>& subsets,
	std::vector<ZXCSMaterial>& materials,
	std::vector<MeshNode>& nodes)
{
	std::wifstream fp(FileName.c_str());
	std::wstring ignore;

	UINT numMaterials;
	UINT numNodes;
	UINT numVertices;
	UINT numTriangles;
	UINT numSubSet;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L" Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	std::getline(fp, ignore);
	std::getline(fp, ignore);
	std::getline(fp, ignore);

	fp >> ignore >> numMaterials;
	fp >> ignore >> numNodes;
	fp >> ignore >> numVertices;
	fp >> ignore >> numTriangles;
	fp >> ignore >> numSubSet;

	materials.resize(numMaterials);
	nodes.resize(numNodes);

	ReadMaterial(fp, numMaterials, materials);
	ReadNodes(fp, numNodes, nodes);
	ReadSubsetTable(fp, numSubSet, subsets);
	ReadVertex(fp, numVertices, vertices);
	ReadIndices(fp, numTriangles, indices);

	return true;
}

bool ZXCLoader::LoadSkin(
	const std::wstring & FileName,
	std::vector<SkinnedVertex>& vertices,
	std::vector<DWORD>& indices,
	std::vector<Subset>& subsets,
	std::vector<ZXCSMaterial>& materials,
	std::vector<MeshNode>& nodes,
	SkinnedData* skinInfo)
{
	std::wifstream fp(FileName.c_str());
	std::wstring ignore;

	UINT numMaterials;
	UINT numNodes;
	UINT numVertices;
	UINT numTriangles;
	UINT numSubSet;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L"Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	std::getline(fp, ignore);
	std::getline(fp, ignore);
	std::getline(fp, ignore);

	fp >> ignore >> numMaterials;
	fp >> ignore >> numNodes;
	fp >> ignore >> numVertices;
	fp >> ignore >> numTriangles;
	fp >> ignore >> numSubSet;

	materials.resize(numMaterials);
	nodes.resize(numNodes);

	ReadMaterial(fp, numMaterials, materials);
	ReadNodes(fp, numNodes, nodes);
	ReadSubsetTable(fp, numSubSet, subsets);
	ReadVertex(fp, numVertices, vertices);
	ReadIndices(fp, numTriangles, indices);
	BuildDefaultAnimaions(skinInfo, nodes);
	return true;
}

void ZXCLoader::ReadMaterial(std::wifstream& fp, UINT numMaterials, std::vector<ZXCSMaterial>& materials)
{
	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < numMaterials; ++i)
	{
		UINT rootMapNum;
		UINT subMtlNum;
		fp >> ignore >> materials[i].Name;
		fp >> ignore >> materials[i].Diffuse.x >> materials[i].Diffuse.y >> materials[i].Diffuse.z;
		fp >> ignore >> materials[i].Specular.x >> materials[i].Specular.y >> materials[i].Specular.z;
		fp >> ignore >> materials[i].Ambient.x >> materials[i].Ambient.y >> materials[i].Ambient.z;
		fp >> ignore >> materials[i].Shininess;
		fp >> ignore >> rootMapNum;
		fp >> ignore >> subMtlNum;

		materials[i].SubMaterial.resize(subMtlNum);
		for (UINT j = 0; j < subMtlNum; ++j)
		{
			UINT subMapNum;
			UINT subNo;

			fp >> ignore >> materials[i].SubMaterial[j].Name;
			fp >> ignore >> materials[i].SubMaterial[j].Diffuse.x >> materials[i].SubMaterial[j].Diffuse.y >> materials[i].SubMaterial[j].Diffuse.z;
			fp >> ignore >> materials[i].SubMaterial[j].Specular.x >> materials[i].SubMaterial[j].Specular.y >> materials[i].SubMaterial[j].Specular.z;
			fp >> ignore >> materials[i].SubMaterial[j].Ambient.x >> materials[i].SubMaterial[j].Ambient.y >> materials[i].SubMaterial[j].Ambient.z;
			fp >> ignore >> materials[i].SubMaterial[j].Shininess;
			fp >> ignore >> subMapNum;

			for (UINT k = 0; k < subMapNum; ++k)
			{
				fp >> ignore >> subNo;
				fp >> ignore >> ignore;
				materials[i].SubMaterial[j].TexMap[subNo] = ignore;
			}
		}
		for (UINT j = 0; j < rootMapNum; ++j)
		{
			UINT subNo;

			fp >> ignore >> subNo;
			fp >> ignore >> ignore;
			materials[i].TexMap[subNo] = ignore;
		}
	}
}

void ZXCLoader::ReadNodes(std::wifstream& fp, UINT numHelpers, std::vector<MeshNode>& nodes)
{
	std::wstring ignore;

	fp >> ignore;
	for (UINT i = 0; i < numHelpers; ++i)
	{
		UINT Index;
		UINT ParentNum;

		fp >> ignore >> ignore >> Index;
		nodes[Index].NodeName = ignore;
		fp >> ignore >> ignore >> ParentNum;
		nodes[Index].ParentName = ignore;
		fp >> ignore >> ignore;
		
		if (ignore.find(L"Bone") != std::wstring::npos)
		{
			nodes[Index].Type = ObjectType::BONE;
		}
		else if (ignore.find(L"Dummy") != std::wstring::npos)
		{
			nodes[Index].Type = ObjectType::DUMMY;
		}
		else if (ignore.find(L"Biped") != std::wstring::npos)
		{
			nodes[Index].Type = ObjectType::BIPED;
		}
		else
		{
			nodes[Index].Type = ObjectType::MESH;
		}

		nodes[Index].ParentIndex = ParentNum;

		fp >> ignore;
		fp >> ignore >> nodes[Index].World._11 >> nodes[Index].World._12 >> nodes[Index].World._13 >> nodes[Index].World._14;
		fp >> ignore >> nodes[Index].World._21 >> nodes[Index].World._22 >> nodes[Index].World._23 >> nodes[Index].World._24;
		fp >> ignore >> nodes[Index].World._31 >> nodes[Index].World._32 >> nodes[Index].World._33 >> nodes[Index].World._34;
		fp >> ignore >> nodes[Index].World._41 >> nodes[Index].World._42 >> nodes[Index].World._43 >> nodes[Index].World._44;
		fp >> ignore >> nodes[Index].Translations.x >> nodes[Index].Translations.y >> nodes[Index].Translations.z;
		fp >> ignore >> nodes[Index].RotationQuat.x >> nodes[Index].RotationQuat.y >> nodes[Index].RotationQuat.z >> nodes[Index].RotationQuat.w;
		fp >> ignore >> nodes[Index].ScaleQuat.x >> nodes[Index].ScaleQuat.y >> nodes[Index].ScaleQuat.z >> nodes[Index].ScaleQuat.w;
		fp >> ignore >> nodes[Index].Scale.x >> nodes[Index].Scale.y >> nodes[Index].Scale.z;
		fp >> ignore >> nodes[Index].box.Center.x >> nodes[Index].box.Center.y >> nodes[Index].box.Center.z;
		fp >> ignore >> nodes[Index].box.Extents.x >> nodes[Index].box.Extents.y >> nodes[Index].box.Extents.z;
	}
}

void ZXCLoader::ReadVertex(std::wifstream & fp, UINT numVertices, std::vector<Vertex>& vertices)
{
	std::wstring ignore;
	fp >> ignore;

	vertices.resize(numVertices);
	for (UINT i = 0; i < numVertices; ++i)
	{
		fp >> ignore >> vertices[i].p.x >> vertices[i].p.y >> vertices[i].p.z;
		fp >> ignore >> vertices[i].n.x >> vertices[i].n.y >> vertices[i].n.z;
		fp >> ignore >> vertices[i].c.x >> vertices[i].c.y >> vertices[i].c.z >> vertices[i].c.w;
		fp >> ignore >> vertices[i].t.x >> vertices[i].t.y;
		fp >> ignore >> vertices[i].Tangent.x >> vertices[i].Tangent.y >> vertices[i].Tangent.z;
	}
}

void ZXCLoader::ReadVertex(std::wifstream& fp, UINT numVertices, std::vector<SkinnedVertex>& vertices)
{
	std::wstring ignore;
	fp >> ignore;

	vertices.resize(numVertices);
	for (UINT i = 0; i < numVertices; ++i)
	{
		float weights[4] = { 0.0f };
		int boneIndices[4] = {0,};

		fp >> ignore >> vertices[i].p.x >> vertices[i].p.y >> vertices[i].p.z;
		fp >> ignore >> vertices[i].n.x >> vertices[i].n.y >> vertices[i].n.z;
		fp >> ignore >> vertices[i].c.x >> vertices[i].c.y >> vertices[i].c.z >> vertices[i].c.w;
		fp >> ignore >> vertices[i].t.x >> vertices[i].t.y;
		fp >> ignore >> vertices[i].Tangent.x >> vertices[i].Tangent.y >> vertices[i].Tangent.z;
		fp >> ignore >> weights[3];

		for (int k = 0; k < (int)weights[3]; ++k)
		{
			if (k == 3)
			{
				fp >> ignore >> boneIndices[k] >> ignore;
				break;
			}
			fp >> ignore >> boneIndices[k] >> weights[k];
		}
		vertices[i].BoneWeights = XMFLOAT4(weights);

		vertices[i].BoneIndices[0] = (BYTE)boneIndices[0];
		vertices[i].BoneIndices[1] = (BYTE)boneIndices[1];
		vertices[i].BoneIndices[2] = (BYTE)boneIndices[2];
		vertices[i].BoneIndices[3] = (BYTE)boneIndices[3];
	}
}

void ZXCLoader::ReadIndices(std::wifstream & fp, UINT numIndices, std::vector<DWORD>& indices)
{
	std::wstring ignore;
	fp >> ignore;
	indices.resize(numIndices * 3);
	for (UINT i = 0; i < numIndices; ++i)
	{
		fp >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}
}

void ZXCLoader::ReadSubsetTable(std::wifstream & fp, UINT numSubsets, std::vector<Subset>& subsets)
{
	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < numSubsets; ++i)
	{
		Subset subset;
		fp >> ignore >> ignore >> subset.NodeIndex;
		fp >> ignore >> subset.MtrlRef;
		fp >> ignore >> subset.SubMtlID;
		fp >> ignore >> subset.VertexStart;
		fp >> ignore >> subset.VertexCount;
		fp >> ignore >> subset.FaceStart;
		fp >> ignore >> subset.FaceCount;

		subsets.push_back(subset);
	}
}

void ZXCLoader::ReadOffsets(std::wifstream & fp, std::vector<DirectX::XMFLOAT4X4>& boneOffsets)
{
	std::wstring ignore;

	fp >> ignore;

	for (UINT i = 0; i < (UINT)boneOffsets.size(); ++i)
	{
		fp >> ignore;
		fp >> ignore >> boneOffsets[i]._11 >> boneOffsets[i]._12 >> boneOffsets[i]._13 >> boneOffsets[i]._14;
		fp >> ignore >> boneOffsets[i]._21 >> boneOffsets[i]._22 >> boneOffsets[i]._23 >> boneOffsets[i]._24;
		fp >> ignore >> boneOffsets[i]._31 >> boneOffsets[i]._32 >> boneOffsets[i]._33 >> boneOffsets[i]._34;
		fp >> ignore >> boneOffsets[i]._41 >> boneOffsets[i]._42 >> boneOffsets[i]._43 >> boneOffsets[i]._44;
	}
}

void ZXCLoader::BuildInitPos(std::vector<DirectX::XMFLOAT4X4>& initPos, const std::vector<MeshNode>& meshNodes)
{
	for (UINT i = 0; i < (UINT)meshNodes.size(); ++i)
	{
		XMMATRIX W = XMLoadFloat4x4(&meshNodes[i].World);
		XMMATRIX InvW = XMMatrixIdentity();
		if (meshNodes[i].ParentIndex != -1)
		{
			int pIndex = meshNodes[i].ParentIndex;
			XMMATRIX PW = XMLoadFloat4x4(&meshNodes[pIndex].World);
			InvW = XMMatrixInverse(&XMMatrixDeterminant(PW), PW);
		}

		W = XMMatrixMultiply(W, InvW);

		XMStoreFloat4x4(&initPos[i], W);
	}
}

void ZXCLoader::BuildDefaultAnimaions(SkinnedData* skinInfo, std::vector<MeshNode>& nodes)
{
	AnimationClip clip;
	clip.BoneAnimations.resize((UINT)nodes.size());

	std::vector<int> boneHierarchy;
	std::vector<XMFLOAT4X4> initPos;
	std::vector<XMFLOAT4X4> boneOffsets;
	std::unordered_map<std::wstring, AnimationClip> animations;

	boneHierarchy.resize((UINT)nodes.size());
	initPos.resize((UINT)nodes.size());
	boneOffsets.resize((UINT)nodes.size());
	
	animations[L"default"] = clip;
	BuildInitPos(initPos, nodes);

//	ReadOffsets(fp, boneOffsets);
	SetBoneOffsets(boneOffsets, nodes);
	for (UINT i = 0; i < (UINT)boneHierarchy.size(); ++i)
	{
		boneHierarchy[i] = nodes[i].ParentIndex;
	}
	skinInfo->Set(boneHierarchy, boneOffsets, initPos, animations);
}

void ZXCLoader::SetBoneOffsets(std::vector<DirectX::XMFLOAT4X4>& boneOffsets, const std::vector<MeshNode>& nodes)
{
	boneOffsets.resize(nodes.size());

	for (UINT i = 0; i < (UINT)boneOffsets.size(); ++i)
	{
		XMMATRIX W = XMLoadFloat4x4(&nodes[i].World);
		XMMATRIX InvW = XMMatrixInverse(&XMMatrixDeterminant(W), W);
		XMStoreFloat4x4(&boneOffsets[i], InvW);
	}
}
