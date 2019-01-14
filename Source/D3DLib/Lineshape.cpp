#include "Shape.h"
#include <DirectXColors.h>

using namespace DirectX;

LineShape::LineShape()
{
	
}

void LineShape::BuildRenderItem()
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometry;
	rItem->Mat = nullptr;
	rItem->World = MathHelper::Identity4x4();
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->IndexCount = rItem->Geo->DrawArgs[L"line"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs[L"line"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs[L"line"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem = rItem.get();
	S_RItem.SaveMiscItem(rItem);
}

void LineShape::BuildGeometry()
{
	if (S_Geometry[L"Line"] != nullptr)
	{
		mGeometry = S_Geometry[L"Line"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = L"Line";
	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

	std::vector<VertexC> vertices;
	std::vector<DWORD> indices;

	vertices =
	{
		VertexC({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)}),
		VertexC({XMFLOAT3(1.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)})
	};

	indices =
	{ 0, 1 };

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(VertexC);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize, sizeof(VertexC));

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs[L"line"] = sub;
}

bool LineShape::Render(ID3D11DeviceContext * pContext)
{
	UINT Offset = 0;
	pContext->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &mGeometry->VertexByteStride, &Offset);
	pContext->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	pContext->IASetPrimitiveTopology(mRenderItem->PrimitiveType);
	pContext->VSSetConstantBuffers(1, 1, mRenderItem->ConstantBuffer.GetAddressOf());
	pContext->DrawIndexedInstanced(mRenderItem->IndexCount, 1, mRenderItem->StartIndexLocation, mRenderItem->BaseVertexLocation, 0);
	return true;
}

bool LineShape::Draw(ID3D11DeviceContext* pContext, XMFLOAT3 vStart, XMFLOAT3 vEnd, XMFLOAT4 vColor)
{
	m_LineVertexList =
	{
		VertexC({XMFLOAT3(vStart), XMFLOAT4(vColor)}),
		VertexC({XMFLOAT3(vEnd), XMFLOAT4(vColor)})
	};

	pContext->UpdateSubresource(mGeometry->VertexBuffer.Get(), 0, nullptr, m_LineVertexList.data(), 0, 0);

	return Render(pContext);
}

DirectionShape::DirectionShape()
{
}

bool DirectionShape::Render(ID3D11DeviceContext * pContext)
{
	UINT Offset = 0;
	pContext->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &mGeometry->VertexByteStride, &Offset);
	pContext->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	pContext->IASetPrimitiveTopology(mRenderItem->PrimitiveType);
	pContext->VSSetConstantBuffers(1, 1, mRenderItem->ConstantBuffer.GetAddressOf());
	pContext->DrawIndexedInstanced(mRenderItem->IndexCount, 1, mRenderItem->StartIndexLocation, mRenderItem->BaseVertexLocation, 0);
	return true;
}

void DirectionShape::BuildGeometry()
{
	if (S_Geometry[L"Direction"] != nullptr)
	{
		mGeometry = S_Geometry[L"Direction"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = L"Direction";
	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

	std::vector<VertexC> vertices;
	std::vector<DWORD> indices;

	vertices =
	{
		VertexC({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)}),
		VertexC({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Green)}),
		VertexC({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Blue)}),
		VertexC({XMFLOAT3(1000.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)}),
		VertexC({XMFLOAT3(0.0f,1000.0f,0.0f), XMFLOAT4(DirectX::Colors::Green)}),
		VertexC({XMFLOAT3(0.0f,0.0f,1000.0f), XMFLOAT4(DirectX::Colors::Blue)}),
	};

	indices =
	{ 0, 3, 1, 4, 2, 5 };

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(VertexC);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize, sizeof(VertexC));

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs[L"direction"] = sub;
}

void DirectionShape::BuildRenderItem()
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometry;
	rItem->World = MathHelper::Identity4x4();
	rItem->Mat = nullptr;
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->IndexCount = rItem->Geo->DrawArgs[L"direction"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs[L"direction"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs[L"direction"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem = rItem.get();
	S_RItem.SaveMiscItem(rItem);
}

void DirectionShape::BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)
{
	return;
}