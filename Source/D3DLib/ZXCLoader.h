#pragma once
#include <DirectXMath.h>
#include "RenderItemStorage.h"
#include "MaterialStorage.h"

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
		std::vector<MeshNode>& nodes);

	bool LoadSkin(
		const std::wstring & FileName,
		std::vector<SkinnedVertex>& vertices,
		std::vector<DWORD>& indices,
		std::vector<Subset>& subsets,
		std::vector<ZXCSMaterial>& materials,
		std::vector<MeshNode>& nodes,
		SkinnedData& skinInfo);
private:
	void ReadScene(std::wifstream& fp);
	void ReadMaterial(std::wifstream& fp, UINT numMaterials, std::vector<ZXCSMaterial>& materials);
	void ReadNodes(std::wifstream& fp, UINT numHelpers, std::vector<MeshNode>& nodes);

	void ReadVertex(std::wifstream& fp, UINT numVertices, std::vector<Vertex>& vertices);
	void ReadVertex(std::wifstream& fp, UINT numVertices, std::vector<SkinnedVertex>& vertices);

	void ReadIndices(std::wifstream& fp, UINT numIndices, std::vector<DWORD>& indices);
	void ReadSubsetTable(std::wifstream& fp, UINT numSubsets, std::vector<Subset>& subsets);
	void ReadAnimationClips(std::wifstream& fp, UINT numBones, UINT numAnimationClips, std::unordered_map<std::string, AnimationClip>& animations, const std::vector<MeshNode>& meshNodes);
	void ReadBoneKeyframes(std::wifstream& fp, UINT numBones, UINT numKeyframe ,BoneAnimation& boneAnimation);

	void AdjustAnimations(AnimationClip& animations, const std::vector<MeshNode>& meshNodes);

	void SetBoneOffsets(std::vector<DirectX::XMFLOAT4X4>& boneOffsets, const std::vector<MeshNode>& nodes);

	UINT mFirstFrame = 0;
	UINT mLastFrame = 0;
	UINT mFrameSpeed = 0;
	UINT mFrameTick = 0;
};