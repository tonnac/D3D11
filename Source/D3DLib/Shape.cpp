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

void Shape::BuildVBIB(LPVOID vertices, LPVOID indices, const UINT vbByteSize, const UINT ibByteSize, UINT vertexStride)
{
	D3DCreateBlob(vbByteSize, mGeometry->VertexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->VertexBufferCPU->GetBufferPointer(), vertices, vbByteSize);

	D3DCreateBlob(ibByteSize, mGeometry->IndexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->IndexBufferCPU->GetBufferPointer(), indices, ibByteSize);

	mGeometry->VertexBufferByteSize = vbByteSize;
	mGeometry->VertexByteStride = vertexStride;
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
	UINT Offset = 0;
	pContext->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &mGeometry->VertexByteStride, &Offset);
	pContext->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	pContext->IASetPrimitiveTopology(mRenderItem->PrimitiveType);
	mRenderItem->Mat->SetResource(pContext);
	pContext->VSSetConstantBuffers(1, 1, mRenderItem->ConstantBuffer.GetAddressOf());
	pContext->DrawIndexedInstanced(mRenderItem->IndexCount, 1, mRenderItem->StartIndexLocation, mRenderItem->BaseVertexLocation, 0);

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

	GeometryGenerator geomesh;
	GeometryGenerator::MeshData mesh = geomesh.CreateBox(10, 10, 10, 0);
	vertices.resize(mesh.Vertices.size());

	for (UINT i = 0; i < (UINT)mesh.Vertices.size(); ++i)
	{
		vertices[i].p = mesh.Vertices[i].Position;
		vertices[i].n = mesh.Vertices[i].Normal;
		vertices[i].t = mesh.Vertices[i].TexC;
		vertices[i].Tangent = mesh.Vertices[i].TangentU;
		vertices[i].c = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

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

	for (UINT i = 0; i < (UINT)mesh.Indices32.size(); ++i)
	{
		indices[i] = mesh.Indices32[i];
	}

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
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(MaterialData), mat->MaterialBuffer.GetAddressOf());
	
	storage->StoreMaterial(mat);
}

void BoxShape::BuildRenderItem(const std::tstring & textureFile)
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
	S_RItem.SaveOpaqueItem(rItem);
}


void SkyBox::BuildGeometry()
{
	if (S_Geometry["Sphere"] != nullptr)
	{
		mGeometry = S_Geometry["Sphere"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = "Sphere";
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
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(MaterialData), mat->MaterialBuffer.GetAddressOf());

	storage->StoreMaterial(mat);
}

void GridShape::BuildGeometry()
{
	if (S_Geometry["Grid"] != nullptr)
	{
		mGeometry = S_Geometry["Grid"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = "Grid";
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
		v.Tangent = grid.Vertices[i].TangentU;
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
//	rItem->World = MathHelper::Identity4x4();
	XMMATRIX S = XMMatrixScaling(16.0f, 16.0f, 16.0f);
	XMMATRIX S1 = XMMatrixScaling(25.0f, 25.0f, 25.0f);
	XMStoreFloat4x4(&rItem->World, S1);
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
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(MaterialData), mat->MaterialBuffer.GetAddressOf());

	storage->StoreMaterial(mat);
}

void SphereShape::BuildGeometry()
{
	if (S_Geometry["SphereV3"] != nullptr)
	{
		mGeometry = S_Geometry["SphereV3"];
		return;
	}

	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
	geo->Name = "SphereV3";
	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

	GeometryGenerator geoMesh;
	GeometryGenerator::MeshData sphere = geoMesh.CreateSphere(0.5f, 20, 20);

	std::vector<Vertex> vertices;
	vertices.resize(sphere.Vertices.size());
	for (UINT i = 0; i < (UINT)sphere.Vertices.size(); ++i)
	{
		vertices[i].p = sphere.Vertices[i].Position;
		vertices[i].n = sphere.Vertices[i].Normal;
		vertices[i].t = sphere.Vertices[i].TexC;
		vertices[i].Tangent = sphere.Vertices[i].TangentU;
		vertices[i].c = XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);
	}

	std::vector<DWORD> indices;
	indices.resize(sphere.Indices32.size());
	for (UINT i = 0; i < (UINT)sphere.Indices32.size(); ++i)
	{
		indices[i] = sphere.Indices32[i];
	}

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize);

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs["sphere"] = sub;
}

void SphereShape::BuildRenderItem(const std::tstring & textureFile)
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometry;

	XMMATRIX W = XMMatrixScaling(40.0f, 40.0f, 40.0f);
	XMMATRIX T = XMMatrixTranslation(0.0f, 15.0f, -45.0f);
	XMStoreFloat4x4(&rItem->World, W * T);
//	rItem->World = MathHelper::Identity4x4();
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->Mat = MaterialStorage::GetStorage()->GetMaterial(L"Sphere");
	rItem->IndexCount = rItem->Geo->DrawArgs["sphere"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs["sphere"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem = rItem.get();
	S_RItem.SaveOpaqueItem(rItem);
}

void SphereShape::BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)
{
	MaterialStorage * storage = MaterialStorage::GetStorage();
	if (storage->GetMaterial(L"Sphere") != nullptr)
		return;

	std::unique_ptr<Material> mat = std::make_unique<Material>();
	mat->Name = L"Sphere";
	mat->Ambient = XMFLOAT3(0.25f, 0.25f, 0.25f);
	mat->Specular = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mat->Diffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mat->FresnelR0 = XMFLOAT3(0.2f, 0.2f, 0.2f);
	mat->MatTransform = MathHelper::Identity4x4();
	mat->Roughness = .125f;
	mat->ShaderResourceView = SrvStorage::GetStorage()->LoadSRV(textureFile);
	mat->NormalView = SrvStorage::GetStorage()->LoadSRV(normalTex);
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(MaterialData), mat->MaterialBuffer.GetAddressOf());

	storage->StoreMaterial(mat);
}