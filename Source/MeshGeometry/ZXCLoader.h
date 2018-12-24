#pragma once
#include <DirectXMath.h>
#include "SkinnedData.h"
#include "RenderItemStorage.h"

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
	std::string NodeName;
	std::string ParentName;
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

class ZXCLoader
{
public:
	ZXCLoader() = default;

	struct Subset
	{
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
		std::map<std::pair<UINT, int>, std::vector<std::pair<int, Subset>>>& subsets,
		std::map<std::pair<UINT, int>, std::vector<std::pair<UINT, std::wstring>>>& materials,
		std::vector<MeshNode>& nodes,
		SkinnedData& skinInfo);

	bool LoadZXC(
		const std::wstring & FileName,
		std::vector<SkinnedVertex>& vertices,
		std::vector<DWORD>& indices,
		std::map<std::pair<UINT, int>, std::vector<std::pair<int, Subset>>>& subsets,
		std::map<std::pair<UINT, int>, std::vector<std::pair<UINT, std::wstring>>>& materials,
		std::vector<MeshNode>& nodes,
		SkinnedData& skinInfo);
private:
	void ReadScene(std::wifstream& fp);
	void ReadMaterial(std::wifstream& fp, UINT numMaterials, std::map<std::pair<UINT, int>, std::vector<std::pair<UINT, std::wstring>>>& materials);
	void ReadMesh(std::wifstream& fp, UINT numMeshes, std::vector<MeshNode>& nodes);
	void ReadHelper(std::wifstream& fp, UINT numHelpers, std::vector<MeshNode>& nodes);

	void ReadVertex(std::wifstream& fp, UINT numVertices, std::vector<Vertex>& vertices);
	void ReadVertex(std::wifstream& fp, UINT numVertices, std::vector<SkinnedVertex>& vertices);

	void ReadIndex(std::wifstream& fp, UINT numIndices, std::vector<DWORD>& indices);
	void ReadSubsetTable(std::wifstream& fp, UINT numSubsets, std::map<std::pair<UINT, int>, std::vector<std::pair<int, Subset>>>& subsets);
	void ReadAnimationClips(std::wifstream& fp, UINT numBones, UINT numAnimationClips, std::unordered_map<std::string, AnimationClip>& animations, const std::vector<MeshNode>& meshNodes);
	void ReadBoneKeyframes(std::wifstream& fp, UINT numBones, UINT numKeyframe ,BoneAnimation& boneAnimation);

	void AdjustAnimations(AnimationClip& animations, const std::vector<MeshNode>& meshNodes);

	void SetBoneOffsets(std::vector<DirectX::XMFLOAT4X4>& boneOffsets, const std::vector<MeshNode>& nodes);

	UINT mFirstFrame = 0;
	UINT mLastFrame = 0;
	UINT mFrameSpeed = 0;
	UINT mFrameTick = 0;
};