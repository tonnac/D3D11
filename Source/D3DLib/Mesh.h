#pragma once

#include "Object.h"
#include "ZXCBinLoader.h"
#include "ClipBinLoader.h"
#include "FrameResource.h"
#include <cstdarg>

template <typename X = Vertex>
class Mesh : public Object<X>
{
public:
	Mesh<X>(ID3D11Device* device) : 
		Object<X>(device)
	{}

	using Object<X>::m_pDevice;
	using Object<X>::mGeometry;
	using Object<X>::BuildVBIB;
	using Object<X>::mBoundingBox;

public:
	virtual bool LoadFile(const std::tstring& filename, const std::tstring& texfilepath = std::tstring());

	virtual bool Frame()override;
	virtual bool Render(ID3D11DeviceContext* context)override;

	virtual void SetWorld(DirectX::FXMMATRIX world)override;
	virtual void SetWorld(const DirectX::XMFLOAT4X4& world)override;

	virtual bool Intersects(DirectX::FXMVECTOR& origin, DirectX::FXMVECTOR& dir, DirectX::CXMMATRIX& invView, float& tmin)override;

	bool DebugRender(ID3D11DeviceContext* context);

private:
	bool LoadZXC(const std::tstring& filename, const std::tstring& texfilepath);
	bool LoadZXCBin(const std::tstring& filename, const std::tstring& texfilepath);

	void BuildRenderItem(const std::vector<Subset>& subsets, const std::vector<ZXCSMaterial>& materials);
	void BuildMaterials(const std::tstring& texfilepath, const std::vector<ZXCSMaterial>& materials);

protected:
	void Initialize(FileInfo& fileInfo);
	UINT GetNodeSize()const;

	virtual FileInfo CreateFileInfo(const std::tstring& filename, const std::tstring& texfilepath);

private:
	std::vector<std::unique_ptr<MeshNode>> mNodeList;
	std::vector<RenderItem*> mDrawItem;
	std::vector<RenderItem*> mDebugItem;
};

template<typename X>
bool Mesh<X>::LoadFile(const std::tstring & filename, const std::tstring & texfilepath)
{
	std::tstring Ext(filename, filename.find_last_of(L".") + 1, filename.length());

	std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);

	if (Ext == L"ZXC")
	{
		return LoadZXC(filename, texfilepath);
	}
	else if (Ext == L"BIN")
	{
		return LoadZXCBin(filename, texfilepath);
	}
	return false;
}

template<typename X>
bool Mesh<X>::Frame()
{
	return true;
}

template<typename X>
bool Mesh<X>::Render(ID3D11DeviceContext* context)
{
	UINT Offset = 0;
	context->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &mGeometry->VertexByteStride, &Offset);
	context->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	for (auto&x : mDrawItem)
	{
		context->IASetPrimitiveTopology(x->PrimitiveType);
		x->Mat->SetResource(context);
		context->VSSetConstantBuffers(1, 1, x->ConstantBuffer.GetAddressOf());
		context->DrawIndexedInstanced(x->IndexCount, 1, x->StartIndexLocation, x->BaseVertexLocation, 0);
	}
	return true;
}

template<typename X>
void Mesh<X>::SetWorld(DirectX::FXMMATRIX world)
{
	for (auto&x : mDrawItem)
	{
		XMStoreFloat4x4(&x->World, world);
	}
}

template<typename X>
void Mesh<X>::SetWorld(const DirectX::XMFLOAT4X4& world)
{
	for (auto&x : mDrawItem)
	{
		x->World = world;
	}
}

template<typename X>
bool Mesh<X>::Intersects(DirectX::FXMVECTOR & origin, DirectX::FXMVECTOR & dir, DirectX::CXMMATRIX & invView, float & tmin)
{
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&mDrawItem[0]->World);
	DirectX::XMMATRIX invWorld = DirectX::XMMatrixInverse(&XMMatrixDeterminant(W), W);

	DirectX::XMMATRIX toLocal = DirectX::XMMatrixMultiply(invView, invWorld);

	DirectX::XMVECTOR rayOrigin = DirectX::XMVector3TransformCoord(origin, toLocal);
	DirectX::XMVECTOR rayDir = DirectX::XMVector3TransformNormal(dir, toLocal);

	rayDir = DirectX::XMVector3Normalize(rayDir);

	const auto vertices = (X*)mGeometry->VertexBufferCPU->GetBufferPointer();
	const auto& indices = (DWORD*)mGeometry->IndexBufferCPU->GetBufferPointer();
	UINT triCount = mGeometry->IndexBufferByteSize / sizeof(DWORD) / 3;

	tmin = MathHelper::Infinity;

	for (UINT i = 0; i < triCount; ++i)
	{
		UINT i0 = indices[i * 3 + 0];
		UINT i1 = indices[i * 3 + 1];
		UINT i2 = indices[i * 3 + 2];

		DirectX::XMVECTOR v0 = XMLoadFloat3(&vertices[i0].p);
		DirectX::XMVECTOR v1 = XMLoadFloat3(&vertices[i1].p);
		DirectX::XMVECTOR v2 = XMLoadFloat3(&vertices[i2].p);

		float t = 0.0f;
		if (DirectX::TriangleTests::Intersects(rayOrigin, rayDir, v0, v1, v2, t))
		{
			tmin = t;
			return true;
		}
	}
	tmin = 0.0f;
	return false;
}

template<typename X>
bool Mesh<X>::DebugRender(ID3D11DeviceContext * context)
{
	UINT Offset = 0;
	context->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &mGeometry->VertexByteStride, &Offset);
	context->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	for (auto&x : mDebugItem)
	{
		context->IASetPrimitiveTopology(x->PrimitiveType);
		context->VSSetConstantBuffers(1, 1, x->ConstantBuffer.GetAddressOf());
		context->DrawIndexedInstanced(x->IndexCount, 1, x->StartIndexLocation, x->BaseVertexLocation, 0);
	}
	return true;
}

template<typename X>
bool Mesh<X>::LoadZXC(const std::tstring & filename, const std::tstring & texfilepath)
{
	FileInfo fileInfo = CreateFileInfo(filename, texfilepath);

	ZXCLoader loader(fileInfo);

	if (!loader.LoadZXC())
		return false;

	Initialize(fileInfo);

	return true;
}

template<typename X>
bool Mesh<X>::LoadZXCBin(const std::tstring & filename, const std::tstring & texfilepath)
{
	FileInfo fileInfo = CreateFileInfo(filename, texfilepath);

	ZXCBinLoader loader(fileInfo);

	if (!loader.LoadBinary(false))
		return false;

	Initialize(fileInfo);

	return true;
}

template<typename X>
void Mesh<X>::BuildRenderItem(const std::vector<Subset>& subsets, const std::vector<ZXCSMaterial>& materials)
{
	for (auto&x : subsets)
	{
		UINT nodeIndex = x.NodeIndex;
		int mtrlRef = x.MtrlRef;
		int mtlID = x.SubMtlID;

		SubmeshGeometry submesh;

		std::unique_ptr<RenderItem> ritem = std::make_unique<RenderItem>();
		ritem->BaseVertexLocation = submesh.BaseVertexLocation = x.VertexStart;
		ritem->IndexCount = submesh.IndexCount = x.FaceCount * 3;
		ritem->StartIndexLocation = submesh.StartIndexLocation = x.FaceStart * 3;
		ritem->Geo = mGeometry;

		if (mtlID == -2 || mtlID > (int)materials[mtrlRef].SubMaterial.size())
		{
			ritem->Mat = nullptr;
		}
		else if (mtlID == -1)
		{
			ritem->Mat = MaterialStorage::GetStorage()->GetMaterial(materials[mtrlRef].Name);
		}
		else
		{
			ritem->Mat = MaterialStorage::GetStorage()->GetMaterial(materials[mtrlRef].SubMaterial[mtlID].Name);
		}

		ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ritem->TexTransform = MathHelper::Identity4x4();
		ritem->World = MathHelper::Identity4x4();

		d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), ritem->ConstantBuffer.GetAddressOf());

		mGeometry->DrawArgs[mNodeList[nodeIndex]->NodeName] = submesh;
		ritem->Name = mNodeList[nodeIndex]->NodeName;
		if (mNodeList[nodeIndex]->Type == ObjectType::MESH && ritem->Mat != nullptr)
		{
			mDrawItem.push_back(ritem.get());
			S_RItem.SaveOpaqueItem(ritem);
		}
		else
		{
			mDebugItem.push_back(ritem.get());
			S_RItem.SaveMiscItem(ritem);
		}
	}
}

template<typename X>
void Mesh<X>::BuildMaterials(const std::tstring & texfilepath, const std::vector<ZXCSMaterial>& materials)
{
	SrvStorage* srvStorage = SrvStorage::GetStorage();

	for (UINT i = 0; i < (UINT)materials.size(); ++i)
	{
		if (!materials[i].SubMaterial.empty())
		{
			for (UINT k = 0; k < (UINT)materials[i].SubMaterial.size(); ++k)
			{
				auto m = materials[i].SubMaterial[k];
				if (m.TexMap.empty())
				{
					continue;
				}
				std::unique_ptr<Material> mat = std::make_unique<Material>();
				mat->Name = m.Name;
				mat->FresnelR0 = DirectX::XMFLOAT3(0.03f, 0.03f, 0.03f);
				mat->MatTransform = MathHelper::Identity4x4();
				mat->Ambient = m.Ambient;
				mat->Diffuse = m.Diffuse;
				mat->Specular = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
				mat->Roughness = 1.0f - m.Shininess;

				auto iter = m.TexMap.find(8);
				if (iter != m.TexMap.end())
				{
					std::wstring texFile = texfilepath;
					texFile += m.TexMap[8];
					mat->NormalView = srvStorage->LoadSRV(texFile);
				}

				std::wstring texFile = texfilepath;
				texFile += m.TexMap[1];
				mat->ShaderResourceView = srvStorage->LoadSRV(texFile);

				d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(MaterialData), mat->MaterialBuffer.GetAddressOf());

				MaterialStorage::GetStorage()->StoreMaterial(mat);
			}
		}
		else
		{
			auto m = materials[i];

			std::unique_ptr<Material> mat = std::make_unique<Material>();
			mat->Name = m.Name;
			mat->FresnelR0 = DirectX::XMFLOAT3(0.03f, 0.03f, 0.03f);
			mat->MatTransform = MathHelper::Identity4x4();
			mat->Ambient = m.Ambient;
			mat->Diffuse = m.Diffuse;
			mat->Specular = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
			mat->Roughness = 1.0f - m.Shininess;

			auto iter = m.TexMap.find(8);
			if (iter != m.TexMap.end())
			{
				std::wstring texFile = texfilepath;
				texFile += m.TexMap[8];
				mat->NormalView = srvStorage->LoadSRV(texFile);
			}

			std::wstring texFile = texfilepath;
			texFile += m.TexMap[1];
			mat->ShaderResourceView = srvStorage->LoadSRV(texFile);

			d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(MaterialData), mat->MaterialBuffer.GetAddressOf());

			MaterialStorage::GetStorage()->StoreMaterial(mat);
		}
	}
}

template<typename X>
void Mesh<X>::Initialize(FileInfo & fileInfo)
{
	BuildMaterials(fileInfo.TexPath, fileInfo.Materials);

	std::wstring name(fileInfo.FileName, 0, fileInfo.FileName.find_last_of('.'));

	if (S_Geometry[name] != nullptr)
	{
		mGeometry = S_Geometry[name];
	}
	else
	{
		std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();
		geo->Name = name;
		mGeometry = geo.get();
		S_Geometry.SaveGeometry(geo);

		const UINT ibByteSize = (UINT)fileInfo.Indices.size() * sizeof(DWORD);

		if (fileInfo.SkinVertices.empty())
		{
			const UINT vbByteSize = (UINT)fileInfo.Vertices.size() * sizeof(Vertex);

			BuildVBIB(fileInfo.Vertices.data(), fileInfo.Indices.data(), vbByteSize, ibByteSize);
		}
		else
		{
			const UINT vbByteSize = (UINT)fileInfo.SkinVertices.size() * sizeof(SkinnedVertex);

			BuildVBIB(fileInfo.SkinVertices.data(), fileInfo.Indices.data(), vbByteSize, ibByteSize, sizeof(SkinnedVertex));
		}
	}

	mNodeList.resize(fileInfo.Nodes.size());
	for (UINT i = 0; i < (UINT)fileInfo.Nodes.size(); ++i)
	{
		mNodeList[i] = std::make_unique<MeshNode>();
		*(mNodeList[i].get()) = fileInfo.Nodes[i];
	}

	BuildRenderItem(fileInfo.Subsets, fileInfo.Materials);

	mBoundingBox = fileInfo.Box;
}

template<typename X>
UINT Mesh<X>::GetNodeSize() const
{
	return (UINT)mNodeList.size();
}

template<typename X>
FileInfo Mesh<X>::CreateFileInfo(const std::tstring & filename, const std::tstring & texfilepath)
{
	FileInfo fileInfo;
	fileInfo.FileName = filename;
	fileInfo.TexPath = texfilepath;
	return fileInfo;
}