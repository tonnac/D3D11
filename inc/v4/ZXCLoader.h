#pragma once
#include <DirectXMath.h>
#include "RenderItemStorage.h"
#include "MaterialStorage.h"
#include "SkinnedData.h"

//class Mesh;

enum class ObjectType : unsigned char
{
	MESH,
	BONE,
	DUMMY,
	BIPED
};

struct MeshNode
{
	std::wstring NodeName;
	std::wstring ParentName;
	int ParentIndex = -1;
	ObjectType Type = ObjectType::MESH;
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT3 Translations;
	DirectX::XMFLOAT4 RotationQuat;
	DirectX::XMFLOAT4 ScaleQuat;
	DirectX::XMFLOAT3 Scale;
	DirectX::BoundingBox box;
	std::vector<RenderItem*> Ritem;
};

struct ZXCSMaterial
{
	std::wstring Name = std::wstring();
	std::wstring ClassName = std::wstring();

	DirectX::XMFLOAT3 Ambient = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 Diffuse = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 Specular = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float Shininess = 0.0f;

	std::map<int, std::wstring> TexMap;
	std::vector<ZXCSMaterial> SubMaterial;
};

struct Subset
{
	int NodeIndex;
	int MtrlRef;
	int SubMtlID;

	UINT VertexStart;
	UINT VertexCount;
	UINT FaceStart;
	UINT FaceCount;
};

struct FileInfo
{
	std::tstring FileName;
	std::tstring TexPath;
	std::vector<Vertex> Vertices;
	std::vector<SkinnedVertex> SkinVertices;
	std::vector<DWORD> Indices;
	std::vector<Subset> Subsets;
	std::vector<ZXCSMaterial> Materials;
	std::vector<MeshNode> Nodes;
	DirectX::BoundingBox Box;
	SkinnedData* skinInfo = nullptr;
};

class ZXCLoader
{
public:
	ZXCLoader(FileInfo& fileInfo);

public:
	bool LoadZXC();
	bool LoadSkin();

protected:
	void ReadCommon(std::wifstream& fp);

	void ReadMaterial(std::wifstream& fp);
	void ReadNodes(std::wifstream& fp);

	template <typename X = Vertex>
	void ReadVertex(std::wifstream& fp);

	template <>
	void ReadVertex<SkinnedVertex>(std::wifstream& fp);

	void ReadIndices(std::wifstream& fp);
	void ReadSubsetTable(std::wifstream& fp);

	void ReadBoundingBox(std::wifstream&);

	void ReadOffsets(std::wifstream& fp, std::vector<DirectX::XMFLOAT4X4>& boneOffsets);

	void BuildDefaultAnimaions();
	void SetBoneOffsets(std::vector<DirectX::XMFLOAT4X4>& boneOffsets, const std::vector<MeshNode>& nodes);
	void BuildInitPos(std::vector<DirectX::XMFLOAT4X4>& initPos, const std::vector<MeshNode>& meshNodes);

	template<typename T = Vertex>
	bool LoadCommon();

protected:

	UINT mFirstFrame = 0;
	UINT mLastFrame = 0;
	UINT mFrameSpeed = 0;
	UINT mFrameTick = 0;

	UINT mNumMaterials = 0;
	UINT mNumNodes = 0;
	UINT mNumVertices = 0;
	UINT mNumTriangles = 0;
	UINT mNumSubSet = 0;

	FileInfo& mFileinfo;
};

template <typename X>
void ZXCLoader::ReadVertex(std::wifstream& fp)
{
	std::wstring ignore;
	fp >> ignore;

	auto& vertices = mFileinfo.Vertices;

	vertices.resize(mNumVertices);
	for (UINT i = 0; i < mNumVertices; ++i)
	{
		fp >> ignore >> vertices[i].p.x >> vertices[i].p.y >> vertices[i].p.z;
		fp >> ignore >> vertices[i].n.x >> vertices[i].n.y >> vertices[i].n.z;
		fp >> ignore >> vertices[i].c.x >> vertices[i].c.y >> vertices[i].c.z >> vertices[i].c.w;
		fp >> ignore >> vertices[i].t.x >> vertices[i].t.y;
		fp >> ignore >> vertices[i].Tangent.x >> vertices[i].Tangent.y >> vertices[i].Tangent.z;
	}
}

template <>
void ZXCLoader::ReadVertex<SkinnedVertex>(std::wifstream& fp)
{
	auto& vertices = mFileinfo.SkinVertices;

	std::wstring ignore;
	fp >> ignore;

	vertices.resize(mNumVertices);
	for (UINT i = 0; i < mNumVertices; ++i)
	{
		float weights[4] = { 0.0f };
		int boneIndices[4] = { 0, };

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
		vertices[i].BoneWeights = DirectX::XMFLOAT4(weights);

		vertices[i].BoneIndices[0] = (BYTE)boneIndices[0];
		vertices[i].BoneIndices[1] = (BYTE)boneIndices[1];
		vertices[i].BoneIndices[2] = (BYTE)boneIndices[2];
		vertices[i].BoneIndices[3] = (BYTE)boneIndices[3];
	}
}

template<typename T>
bool ZXCLoader::LoadCommon()
{
	std::wifstream fp(mFileinfo.FileName.c_str());

	if (!fp.is_open())
	{
		std::wstring message = mFileinfo.FileName + L" Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	ReadCommon(fp);
	ReadVertex<T>(fp);
	ReadIndices(fp);
	return true;
}

