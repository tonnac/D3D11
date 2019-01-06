#pragma once
#include "ObjectStd.h"
#include "FrameResource.h"
#include "GeometryStroage.h"
#include "MaterialStorage.h"

struct RenderItem
{
	RenderItem() = default;
	RenderItem(const RenderItem& rhs) = delete;

	std::wstring Name;

	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer = nullptr;

	MeshGeometry* Geo = nullptr;
	Material* Mat = nullptr;

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

private:
	std::vector<std::unique_ptr<RenderItem>> mAllRitem;
	std::vector<RenderItem*> mOpaqueItem;
	std::vector<RenderItem*> mMiscItem;
};

#define S_RItem RenderItemStorage::getInst()