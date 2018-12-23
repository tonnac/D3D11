#pragma once
#include "d3dUtil.h"
#include "ObjectStd.h"
#include "MathHelper.h"

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

struct RenderItem
{
	RenderItem() = default;
	RenderItem(const RenderItem& rhs) = delete;

	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();

	DirectX::XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer = nullptr;
	MeshGeometry* Geo = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	DirectX::BoundingBox Bounds;

	UINT IndexCount = 0;
	UINT InstanceCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};

class RenderItemStorage : public Singleton<RenderItemStorage>
{
	friend class Singleton<RenderItemStorage>;
private:
	RenderItemStorage() = default;
public:
	void SaveRenderItem(std::unique_ptr<RenderItem>& ritem);
	void SaveOpaqueItem(std::unique_ptr<RenderItem>& ritem);
	void SaveMiscItem(std::unique_ptr<RenderItem>& ritem);

	void UpdateObjectCBs(ID3D11DeviceContext * context);
public:
	std::vector<std::unique_ptr<RenderItem>> mAllRitem;
	std::vector<RenderItem*> mOpaqueItem;
	std::vector<RenderItem*> mMiscItem;
};

#define S_RItem RenderItemStorage::getInst()