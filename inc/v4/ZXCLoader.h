#pragma once
#include <DirectXMath.h>
#include "RenderItemStorage.h"
#include "MaterialStorage.h"
#include "SkinnedData.h"

class Mesh;

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

class ZXCLoader
{
public:
	ZXCLoader() = default;

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

public:
	bool LoadZXC(
		const std::wstring & FileName,
		std::vector<Vertex>& vertices,
		std::vector<DWORD>& indices,
		std::vector<Subset>& subsets,
		std::vector<ZXCSMaterial>& materials,
		std::vector<MeshNode>& nodes,
		DirectX::BoundingBox& box);

	bool LoadSkin(
		const std::wstring & FileName,
		std::vector<SkinnedVertex>& vertices,
		std::vector<DWORD>& indices,
		std::vector<Subset>& subsets,
		std::vector<ZXCSMaterial>& materials,
		std::vector<MeshNode>& nodes,
		DirectX::BoundingBox& box,
		SkinnedData* skinInfo);

protected:
	void ReadCommon(std::wifstream& fp,
		std::vector<Subset>& subsets,
		std::vector<ZXCSMaterial>& materials,
		std::vector<MeshNode>& nodes,
		DirectX::BoundingBox& box);

	void ReadMaterial(std::wifstream& fp, std::vector<ZXCSMaterial>& materials);
	void ReadNodes(std::wifstream& fp, std::vector<MeshNode>& nodes);

	void ReadVertex(std::wifstream& fp, std::vector<Vertex>& vertices);
	void ReadVertex(std::wifstream& fp, std::vector<SkinnedVertex>& vertices);

	void ReadIndices(std::wifstream& fp, std::vector<DWORD>& indices);
	void ReadSubsetTable(std::wifstream& fp, std::vector<Subset>& subsets);

	void ReadBoundingBox(std::wifstream& fp, DirectX::BoundingBox& box);

	void ReadOffsets(std::wifstream& fp, std::vector<DirectX::XMFLOAT4X4>& boneOffsets);

	void BuildDefaultAnimaions(SkinnedData * skininfo, std::vector<MeshNode>& nodes);
	void SetBoneOffsets(std::vector<DirectX::XMFLOAT4X4>& boneOffsets, const std::vector<MeshNode>& nodes);
	void BuildInitPos(std::vector<DirectX::XMFLOAT4X4>& initPos, const std::vector<MeshNode>& meshNodes);

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
};