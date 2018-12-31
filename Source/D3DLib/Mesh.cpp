#include "Mesh.h"
#include "DirectInput.h"

using namespace DirectX;

bool Mesh::LoadFile(const std::tstring & filename, const std::tstring& texfilepath, ID3D11Device * device)
{
	m_pDevice = device;

	std::tstring Ext(filename, filename.find_last_of(L".") + 1, filename.length());
	if (Ext == L"ZXCS")
	{
		return LoadZXCS(filename, texfilepath);
	}
	else
	{
		return LoadZXC(filename, texfilepath);
	}
}

bool Mesh::LoadZXC(const std::tstring& filename, const std::tstring& texfilepath)
{
	ZXCLoader loader;

	std::tstring file(filename, 0, filename.find_last_of(L".") - 1);

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::map<std::pair<UINT, int>, std::vector<std::pair<int, ZXCLoader::Subset>>> subsets;
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	SkinnedData skininfo;
	if (!loader.LoadZXC(filename, vertices, indices, subsets, materials, nodes, skininfo))
		return false;

	if (skininfo.BoneCount() > 0)
	{
		mSkinnedInst = std::make_unique<SkinnedModelInstance>();
		mSkinInfo = std::make_unique<SkinnedData>();
		*(mSkinInfo.get()) = skininfo;
		mSkinnedInst->SkinnedInfo = mSkinInfo.get();
		mSkinnedInst->ClipName = "default";
		mSkinnedInst->FinalTransforms.resize(nodes.size());
	}
	
	std::string name = std::string(file.begin(), file.end());

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

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

		BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize);
	}

	mNodeList.resize(nodes.size());
	for (UINT i = 0; i < (UINT)nodes.size(); ++i)
	{
		mNodeList[i] = std::make_unique<MeshNode>();
		*(mNodeList[i].get()) = nodes[i];
	}

	BuildRenderItem(subsets, materials);

	return true;
}

bool Mesh::LoadZXCS(const std::tstring& filename, const std::tstring& texfilepath)
{
	ZXCLoader loader;

	std::tstring file(filename, 0, filename.find_last_of(L"."));

	std::vector<SkinnedVertex> vertices;
	std::vector<DWORD> indices;
	std::map<std::pair<UINT, int>, std::vector<std::pair<int, ZXCLoader::Subset>>> subsets;
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	SkinnedData skininfo;

	if (!loader.LoadZXCS(filename, vertices, indices, subsets, materials, nodes, skininfo))
		return false;

	BuildMaterials(texfilepath, materials);

	if (skininfo.BoneCount() > 0)
	{
		mSkinnedInst = std::make_unique<SkinnedModelInstance>();
		mSkinInfo = std::make_unique<SkinnedData>();

		mSkinnedInst->FinalTransforms.resize(nodes.size());
		*(mSkinInfo.get()) = skininfo;
		mSkinnedInst->SkinnedInfo = mSkinInfo.get();
		mSkinnedInst->ClipName = "default";
	}

	std::string name = std::string(file.begin(), file.end());
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

		const UINT vbByteSize = (UINT)vertices.size() * sizeof(SkinnedVertex);
		const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

		BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize, sizeof(SkinnedVertex));
	}

	mNodeList.resize(nodes.size());
	for (UINT i = 0; i < (UINT)nodes.size(); ++i)
	{
		mNodeList[i] = std::make_unique<MeshNode>();
		*(mNodeList[i].get()) = nodes[i];
	}

	BuildRenderItem(subsets, materials);

	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(SkinnedConstants), mConstantbuffer.GetAddressOf());

	return true;
}

void Mesh::BuildRenderItem(
	const std::map<std::pair<UINT, int>, std::vector<std::pair<int, ZXCLoader::Subset>>>& subsets,
	const std::vector<ZXCSMaterial>& materials)
{
	for (auto&x : subsets)
	{
		for (auto&k : x.second)
		{
			UINT nodeIndex = x.first.first;
			int mtrlRef = x.first.second;
			int mtlID = k.first;

			SubmeshGeometry submesh;

			std::unique_ptr<RenderItem> ritem = std::make_unique<RenderItem>();
			ritem->BaseVertexLocation = submesh.BaseVertexLocation = k.second.VertexStart;
			ritem->IndexCount = submesh.IndexCount = k.second.FaceCount * 3;
			ritem->StartIndexLocation = submesh.StartIndexLocation = k.second.FaceStart * 3;
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

			std::string name = mNodeList[nodeIndex]->NodeName;
			mGeometry->DrawArgs[name] = submesh;
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
}

void Mesh::BuildMaterials(const std::tstring& texfilepath, const std::vector<ZXCSMaterial>& materials)
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
				mat->FresnelR0 = XMFLOAT3(0.03f, 0.03f, 0.03f);
				mat->MatTransform = MathHelper::Identity4x4();
				mat->Ambient = m.Ambient;
				mat->Diffuse = m.Diffuse;
				mat->Specular = m.Specular;
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
			mat->FresnelR0 = XMFLOAT3(0.03f, 0.03f, 0.03f);
			mat->MatTransform = MathHelper::Identity4x4();
			mat->Ambient = m.Ambient;
			mat->Diffuse = m.Diffuse;
			mat->Specular = m.Specular;
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

bool Mesh::Frame()
{
	if (mSkinnedInst != nullptr)
	{
		mSkinnedInst->UpdateSkinnedAnimation(g_fSecPerFrame);

		std::copy(
			std::begin(mSkinnedInst->FinalTransforms),
			std::end(mSkinnedInst->FinalTransforms),
			&mSkinnedConstants.BoneTransforms[0]
		);
	}

	mSkinnedConstants.BoneTransforms[0] = MathHelper::Identity4x4();

	return true;
}

bool Mesh::Render(ID3D11DeviceContext * context)
{
	if (mConstantbuffer != nullptr)
	{
		context->UpdateSubresource(mConstantbuffer.Get(), 0, nullptr, &mSkinnedConstants, 0, 0);
		context->VSSetConstantBuffers(3, 1, mConstantbuffer.GetAddressOf());
	}
	UINT Offset = 0;
	context->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &mGeometry->VertexByteStride, &Offset);
	context->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	for (auto&x : mDrawItem)
	{
		//if (x->ShaderResourceView == nullptr)
		//	continue;
		context->IASetPrimitiveTopology(x->PrimitiveType);
		x->Mat->SetResource(context);
		context->VSSetConstantBuffers(1, 1, x->ConstantBuffer.GetAddressOf());
		context->DrawIndexedInstanced(x->IndexCount, 1, x->StartIndexLocation, x->BaseVertexLocation, 0);
	}
	
	return true;
}

void Mesh::SetWorld(DirectX::FXMMATRIX world)
{
	if (!mDrawItem.empty())
	{
		for (auto&x : mDrawItem)
		{
			XMStoreFloat4x4(&x->World, world);
		}
	}
}

void Mesh::SetWorld(const DirectX::XMFLOAT4X4 & world)
{
	if (!mDrawItem.empty())
	{
		for (auto&x : mDrawItem)
		{
			x->World = world;
		}
	}
}