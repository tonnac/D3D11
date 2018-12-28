#include "RenderItemStorage.h"

using namespace DirectX;

void RenderItemStorage::SaveRenderItem(std::unique_ptr<RenderItem>& ritem)
{
	mAllRitem.push_back(std::move(ritem));
}

void RenderItemStorage::SaveOpaqueItem(std::unique_ptr<RenderItem>& ritem)
{
	mOpaqueItem.push_back(ritem.get());
	mAllRitem.push_back(std::move(ritem));
}

void RenderItemStorage::SaveMiscItem(std::unique_ptr<RenderItem>& ritem)
{
	mMiscItem.push_back(ritem.get());
	mAllRitem.push_back(std::move(ritem));
}

void RenderItemStorage::UpdateObjectCBs(ID3D11DeviceContext * context)
{
	for (auto&x : mAllRitem)
	{
		ObjectConstants obj;

		XMMATRIX texTransform = XMLoadFloat4x4(&x->TexTransform);
		XMMATRIX World = XMLoadFloat4x4(&x->World);
		XMMATRIX WorldInv = XMMatrixInverse(&XMMatrixDeterminant(World), World);

		XMStoreFloat4x4(&obj.TexTransform, XMMatrixTranspose(texTransform));
		XMStoreFloat4x4(&obj.World, XMMatrixTranspose(World));
		XMStoreFloat4x4(&obj.WorldInv, XMMatrixTranspose(WorldInv));

		context->UpdateSubresource(x->ConstantBuffer.Get(), 0, nullptr, &obj, 0, 0);
	}
}
