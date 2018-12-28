#include "Shape.h"
#include <DirectXColors.h>

using namespace DirectX;

Shape::Shape()
{
}

void Shape::Create(ID3D11Device* pDevice, const std::tstring& textureFile, const std::tstring& normalTex)
{
	m_pDevice = pDevice;

	BuildGeometry();
	BuildMaterials(textureFile, normalTex);
	BuildRenderItem(textureFile);
}

void Shape::BuildRenderItem(const std::tstring & textureFile)
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometry;
	rItem->World = MathHelper::Identity4x4();
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->Mat = MaterialStorage::GetStorage()->GetMaterial(L"Box");
	rItem->IndexCount = rItem->Geo->DrawArgs["box"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs["box"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs["box"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem = rItem.get();
	S_RItem.SaveRenderItem(rItem);
}

void Shape::BuildVBIB(LPVOID vertices, LPVOID indices, const UINT vbByteSize, const UINT ibByteSize, UINT vertexStride)
{
	D3DCreateBlob(vbByteSize, mGeometry->VertexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->VertexBufferCPU->GetBufferPointer(), vertices, vbByteSize);

	D3DCreateBlob(ibByteSize, mGeometry->IndexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->IndexBufferCPU->GetBufferPointer(), indices, ibByteSize);

	mGeometry->VertexBufferByteSize = vbByteSize;
	mGeometry->VertexByteStride = Stride = vertexStride;
	mGeometry->IndexBufferByteSize = ibByteSize;
	mGeometry->IndexFormat = DXGI_FORMAT_R32_UINT;

	d3dUtil::CreateVertexBuffer(m_pDevice,
		mGeometry->VertexBufferByteSize,
		mGeometry->VertexBufferCPU->GetBufferPointer(),
		mGeometry->VertexBuffer.GetAddressOf());

	d3dUtil::CreateIndexBuffer(m_pDevice,
		mGeometry->IndexBufferByteSize,
		mGeometry->IndexBufferCPU->GetBufferPointer(),
		mGeometry->IndexBuffer.GetAddressOf());
}

bool Shape::Frame()
{
	return true;
}

bool Shape::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &Stride, &offset);
	pContext->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	if (mRenderItem != nullptr)
	{
		pContext->IASetPrimitiveTopology(mRenderItem->PrimitiveType);
		if(mRenderItem->Mat != nullptr)
			mRenderItem->Mat->SetResource(pContext);
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
	if (S_Geometry["Box"] != nullptr)
	{
		mGeometry = S_Geometry["Box"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = "Box";
	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

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

	BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize);

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs["box"] = sub;
}

void BoxShape::BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)
{
	MaterialStorage * storage = MaterialStorage::GetStorage();
	if (storage->GetMaterial(L"Box") != nullptr)
		return;

	std::unique_ptr<Material> mat = std::make_unique<Material>();
	mat->Name = L"Box";
	mat->Ambient = XMFLOAT3(0.25f, 0.25f, 0.25f);
	mat->Specular = XMFLOAT3(0.9f, 0.9f, 0.9f);
	mat->Diffuse = XMFLOAT3(0.9f, 0.9f, 0.9f);
	mat->FresnelR0 = XMFLOAT3(0.03f, 0.03f, 0.03f);
	mat->MatTransform = MathHelper::Identity4x4();
	mat->Roughness = .25f;
	mat->ShaderResourceView = SrvStorage::GetStorage()->LoadSRV(textureFile);
	mat->NormalView = SrvStorage::GetStorage()->LoadSRV(normalTex);
	
	storage->StoreMaterial(mat);
}

void SkyBox::BuildGeometry()
{
	if (S_Geometry["SkyBox"] != nullptr)
	{
		mGeometry = S_Geometry["SkyBox"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = "SkyBox";
	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

	struct Vertex_
	{
		XMFLOAT3 Pos;
	};

	GeometryGenerator geoMesh;
	GeometryGenerator::MeshData sphere = geoMesh.CreateSphere(0.5f, 20, 20);

	std::vector<Vertex_> vertices;
	for (UINT i = 0; i < (UINT)sphere.Vertices.size(); ++i)
	{
		Vertex_ v;
		v.Pos = sphere.Vertices[i].Position;
		vertices.push_back(v);
	}

	std::vector<DWORD> indices;
	for (UINT i = 0; i < (UINT)sphere.Indices32.size(); ++i)
	{
		indices.push_back(sphere.Indices32[i]);
	}

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex_);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize, sizeof(Vertex_));

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs["sphere"] = sub;

	mGeometry->Name = "Sphere";
}

void SkyBox::BuildRenderItem(const std::tstring & textureFile)
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometry;
	XMMATRIX S = XMMatrixScaling(5000.0f, 5000.0f, 5000.0f);
	XMStoreFloat4x4(&rItem->World, S);
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->Mat = MaterialStorage::GetStorage()->GetMaterial(L"SkyBox");
	rItem->IndexCount = rItem->Geo->DrawArgs["sphere"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs["sphere"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem = rItem.get();
	S_RItem.SaveOpaqueItem(rItem);
}

void SkyBox::BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)
{
	MaterialStorage * storage = MaterialStorage::GetStorage();
	if (storage->GetMaterial(L"SkyBox") != nullptr)
		return;

	std::unique_ptr<Material> mat = std::make_unique<Material>();
	mat->Name = L"SkyBox";
	mat->Ambient = XMFLOAT3(0.25f, 0.25f, 0.25f);
	mat->Specular = XMFLOAT3(0.9f, 0.9f, 0.9f);
	mat->Diffuse = XMFLOAT3(0.9f, 0.9f, 0.9f);
	mat->FresnelR0 = XMFLOAT3(0.03f, 0.03f, 0.03f);
	mat->MatTransform = MathHelper::Identity4x4();
	mat->Roughness = .25f;
	mat->ShaderResourceView = SrvStorage::GetStorage()->LoadSRV(textureFile);
	mat->NormalView = SrvStorage::GetStorage()->LoadSRV(normalTex);

	storage->StoreMaterial(mat);
}

void GridShape::BuildGeometry()
{
	if (S_Geometry["Quad"] != nullptr)
	{
		mGeometry = S_Geometry["Quad"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = "Quad";
	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

	GeometryGenerator geoMesh;
	GeometryGenerator::MeshData grid = geoMesh.CreateGrid(20.0f, 30.0f, 60, 40);

	std::vector<Vertex> vertices;
	for (UINT i = 0; i < (UINT)grid.Vertices.size(); ++i)
	{
		Vertex v;
		v.p = grid.Vertices[i].Position;
		v.n = grid.Vertices[i].Normal;
		v.t = grid.Vertices[i].TexC;
		v.c = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertices.push_back(v);
	}

	std::vector<DWORD> indices;
	for (UINT i = 0; i < (UINT)grid.Indices32.size(); ++i)
	{
		indices.push_back(grid.Indices32[i]);
	}

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize);

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs["grid"] = sub;

	mGeometry->Name = "Grid";
}

void GridShape::BuildRenderItem(const std::tstring & textureFile)
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometry;
	rItem->World = MathHelper::Identity4x4();
	XMMATRIX S = XMMatrixScaling(8.0f, 8.0f, 8.0f);
	XMStoreFloat4x4(&rItem->TexTransform, S);
	rItem->Mat = MaterialStorage::GetStorage()->GetMaterial(L"Grid");
	rItem->IndexCount = rItem->Geo->DrawArgs["grid"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs["grid"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs["grid"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem = rItem.get();
	S_RItem.SaveOpaqueItem(rItem);
}

void GridShape::BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)
{
	MaterialStorage * storage = MaterialStorage::GetStorage();
	if (storage->GetMaterial(L"Grid") != nullptr)
		return;

	std::unique_ptr<Material> mat = std::make_unique<Material>();
	mat->Name = L"Grid";
	mat->Ambient = XMFLOAT3(0.25f, 0.25f, 0.25f);
	mat->Specular = XMFLOAT3(0.9f, 0.9f, 0.9f);
	mat->Diffuse = XMFLOAT3(0.9f, 0.9f, 0.9f);
	mat->FresnelR0 = XMFLOAT3(0.03f, 0.03f, 0.03f);
	mat->MatTransform = MathHelper::Identity4x4();
	mat->Roughness = .25f;
	mat->ShaderResourceView = SrvStorage::GetStorage()->LoadSRV(textureFile);
	mat->NormalView = SrvStorage::GetStorage()->LoadSRV(normalTex);

	storage->StoreMaterial(mat);
}
