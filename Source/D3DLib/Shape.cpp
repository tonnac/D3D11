#include "Shape.h"
#include <DirectXColors.h>

using namespace DirectX;

Shape::Shape()
{
}

void Shape::Create(ID3D11Device* pDevice, const std::tstring& shaderFile, const std::tstring& textureFile)
{
	m_pDevice = pDevice;

	mDxObject = std::make_unique<DxObj>();
	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

	BuildGeometry();
	BuildRenderItem(textureFile);

	d3dUtil::CreateVertexBuffer(m_pDevice,
		mGeometry->VertexBufferByteSize,
		mGeometry->VertexBufferCPU->GetBufferPointer(),
		mGeometry->VertexBuffer.GetAddressOf());

	d3dUtil::CreateIndexBuffer(m_pDevice,
		mGeometry->IndexBufferByteSize,
		mGeometry->IndexBufferCPU->GetBufferPointer(),
		mGeometry->IndexBuffer.GetAddressOf());

	BuildDxObject(m_pDevice, shaderFile, nullptr);
}

void Shape::BuildRenderItem(const std::tstring & textureFile)
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometry;
	rItem->World = MathHelper::Identity4x4();
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->IndexCount = rItem->Geo->DrawArgs["default"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs["default"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs["default"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	if (!textureFile.empty())
		d3dUtil::CreateShaderResourceView(m_pDevice, textureFile.c_str(), rItem->ShaderResourceView.GetAddressOf());
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem = rItem.get();
	S_RItem.SaveRenderItem(rItem);
}

void Shape::BuildDxObject(ID3D11Device * device, const std::tstring & filename, const D3D10_SHADER_MACRO * defines)
{
	if (mDxObject == nullptr)
		mDxObject = std::make_unique<DxObj>();
	Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob = nullptr;
	if (mDxObject->m_pVertexShader == nullptr)
	{
		d3dUtil::LoadVertexShaderFile(m_pDevice, filename.c_str(), defines, mDxObject->m_pVertexShader.GetAddressOf(), "VS", vertexBlob.GetAddressOf());
		CreateInputLayout(vertexBlob.Get());
	}
	if (mDxObject->m_pPixelShader == nullptr)
		d3dUtil::LoadPixelShaderFile(m_pDevice, filename.c_str(), defines, mDxObject->m_pPixelShader.GetAddressOf(), "PS");
}

void Shape::CreateCPUBuffer(LPVOID vertices, LPVOID indices, const UINT vbByteSize, const UINT ibByteSize, UINT vertexStride)
{
	D3DCreateBlob(vbByteSize, mGeometry->VertexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->VertexBufferCPU->GetBufferPointer(), vertices, vbByteSize);

	D3DCreateBlob(ibByteSize, mGeometry->IndexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->IndexBufferCPU->GetBufferPointer(), indices, ibByteSize);

	mGeometry->VertexBufferByteSize = vbByteSize;
	mGeometry->VertexByteStride = Stride = vertexStride;
	mGeometry->IndexBufferByteSize = ibByteSize;
	mGeometry->IndexFormat = DXGI_FORMAT_R32_UINT;
}

void Shape::CreateInputLayout(ID3DBlob * vertexblob)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 24	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 40	, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	d3dUtil::CreateInputLayout(m_pDevice,
		(DWORD)vertexblob->GetBufferSize(),
		vertexblob->GetBufferPointer(),
		layout,
		(UINT)std::size(layout),
		mDxObject->m_pInputLayout.GetAddressOf());
}


bool Shape::Frame()
{
	return true;
}

bool Shape::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &Stride, &offset);
	pContext->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);
	if (mDxObject != nullptr)
		mDxObject->SetResource(pContext);

	if (mRenderItem != nullptr)
	{
		pContext->IASetPrimitiveTopology(mRenderItem->PrimitiveType);
		pContext->PSSetShaderResources(0, 1, mRenderItem->ShaderResourceView.GetAddressOf());
		pContext->VSSetConstantBuffers(1, 1, mRenderItem->ConstantBuffer.GetAddressOf());
		pContext->DrawIndexedInstanced(mRenderItem->IndexCount, 1, mRenderItem->StartIndexLocation, mRenderItem->BaseVertexLocation, 0);
	}
	return true;
}

BoxShape::BoxShape(bool isDice) : mIsDice(isDice)
{
}

void BoxShape::BuildGeometry()
{
	std::vector<Vertex> vertices;

	vertices.resize(24);
	vertices[0] = Vertex(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	vertices[1] = Vertex(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	vertices[2] = Vertex(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	vertices[3] = Vertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
	// µÞ¸é
	vertices[4] = Vertex(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	vertices[5] = Vertex(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	vertices[6] = Vertex(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	vertices[7] = Vertex(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	vertices[8] = Vertex(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	vertices[9] = Vertex(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	vertices[10] = Vertex(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	vertices[11] = Vertex(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// ¿ÞÂÊ
	vertices[12] = Vertex(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	vertices[13] = Vertex(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	vertices[14] = Vertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	vertices[15] = Vertex(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// À­¸é
	vertices[16] = Vertex(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	vertices[17] = Vertex(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	vertices[18] = Vertex(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	vertices[19] = Vertex(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// ¾Æ·§¸é
	vertices[20] = Vertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	vertices[21] = Vertex(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	vertices[22] = Vertex(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	vertices[23] = Vertex(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	if (mIsDice)
	{
		vertices[0].t = { 0.76f, 0.66f };
		vertices[1].t = { 0.76f, 0.34f };
		vertices[2].t = { 1.0f, 0.34f };
		vertices[3].t = { 1.0f, 0.66f };

		vertices[4].t = { 0.5f, 0.33f };
		vertices[5].t = { 0.25f, 0.33f };
		vertices[6].t = { 0.25f, 0.0f };
		vertices[7].t = { 0.5f, 0.0f };

		vertices[8].t = { 0.51f, 0.66f };
		vertices[9].t = { 0.51f, 0.34f };
		vertices[10].t = { 0.75f, 0.34f };
		vertices[11].t = { 0.75f, 0.66f };

		vertices[12].t = { 0.5f, 0.66f };
		vertices[13].t = { 0.25f, 0.66f };
		vertices[14].t = { 0.25f, 0.33f };
		vertices[15].t = { 0.5f, 0.33f };

		vertices[16].t = { 0.26f, 1.0f };
		vertices[17].t = { 0.26f, 0.67f };
		vertices[18].t = { 0.5f, 0.67f };
		vertices[19].t = { 0.5f, 1.0f };

		vertices[20].t = { 0.0f, 0.66f };
		vertices[21].t = { 0.0f, 0.33f };
		vertices[22].t = { 0.25f, 0.33f };
		vertices[23].t = { 0.25f, 0.66f };
	}

	std::vector<DWORD> indices;

	indices.resize(36);

	int iIndex = 0;
	indices[iIndex++] = 0; 	indices[iIndex++] = 1; 	indices[iIndex++] = 2; 	indices[iIndex++] = 0;	indices[iIndex++] = 2; 	indices[iIndex++] = 3;
	indices[iIndex++] = 4; 	indices[iIndex++] = 5; 	indices[iIndex++] = 6; 	indices[iIndex++] = 4;	indices[iIndex++] = 6; 	indices[iIndex++] = 7;
	indices[iIndex++] = 8; 	indices[iIndex++] = 9; 	indices[iIndex++] = 10; indices[iIndex++] = 8;	indices[iIndex++] = 10; indices[iIndex++] = 11;
	indices[iIndex++] = 12; indices[iIndex++] = 13; indices[iIndex++] = 14; indices[iIndex++] = 12;	indices[iIndex++] = 14; indices[iIndex++] = 15;
	indices[iIndex++] = 16; indices[iIndex++] = 17; indices[iIndex++] = 18; indices[iIndex++] = 16;	indices[iIndex++] = 18; indices[iIndex++] = 19;
	indices[iIndex++] = 20; indices[iIndex++] = 21; indices[iIndex++] = 22; indices[iIndex++] = 20;	indices[iIndex++] = 22; indices[iIndex++] = 23;

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	CreateCPUBuffer(vertices.data(), indices.data(), vbByteSize, ibByteSize);

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs["default"] = sub;
}