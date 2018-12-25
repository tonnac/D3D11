#pragma once
#include "d3dUtil.h"

struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	DirectX::BoundingBox Bounds;
	DirectX::BoundingSphere Sphere;
};

struct MeshGeometry
{
	std::string Name;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R32_UINT;
	UINT IndexBufferByteSize = 0;

	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;
};

class GeometryStroage : public Singleton<GeometryStroage>
{
	friend class Singleton<GeometryStroage>;
private:
	GeometryStroage() = default;

public:
	void SaveGeometry(std::unique_ptr<MeshGeometry>& geometry);

	MeshGeometry* operator[] (int index);
private:
	std::vector<std::unique_ptr<MeshGeometry>> mGeometries;
};

#define S_Geometry GeometryStroage::getInst()