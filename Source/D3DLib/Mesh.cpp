#include "Mesh.h"
#include "DirectInput.h"

using namespace DirectX;

bool Mesh::LoadFile(const std::tstring & filename, const std::tstring& texfilepath, ID3D11Device * device)
{
	m_pDevice = device;

	std::tstring Ext(filename, filename.find_last_of(L".") + 1, filename.length());

	std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);

	if (Ext == L"SKN")
	{
		return LoadSkin(filename, texfilepath);
	}
	else if(Ext == L"ZXC")
	{
		return LoadZXC(filename, texfilepath);
	}
	else if(Ext == L"BIN")
	{
		return LoadZXCBin(filename, texfilepath);
	}
	else if(Ext == L"SBI")
	{
		return LoadSkinBin(filename, texfilepath);
	}
	else if (Ext == L"CLP")
	{
		return LoadClip(filename, texfilepath);
	}
	else
	{
		return LoadClipBin(filename, texfilepath);
	}
}

bool Mesh::LoadZXC(const std::tstring& filename, const std::tstring& texfilepath)
{
	ZXCLoader loader;

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::vector<ZXCLoader::Subset> subsets;
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	if (!loader.LoadZXC(filename, vertices, indices, subsets, materials, nodes, mBoundingBox))
		return false;
	
	Initialize(vertices, indices, subsets, materials, nodes, filename, texfilepath);

	return true;
}

bool Mesh::LoadSkin(const std::tstring& filename, const std::tstring& texfilepath)
{
	ZXCLoader loader;

	std::tstring file(filename, 0, filename.find_last_of(L"."));

	std::vector<SkinnedVertex> vertices;
	std::vector<DWORD> indices;
	std::vector<ZXCLoader::Subset> subsets;
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	SkinnedData skininfo;

	if (!loader.LoadSkin(filename, vertices, indices, subsets, materials, nodes, mBoundingBox, &skininfo))
		return false;

	mSkinnedInst = std::make_unique<SkinnedModelInstance>();
	mSkinInfo = std::make_unique<SkinnedData>();

	mSkinnedInst->FinalTransforms.resize(nodes.size());
	*(mSkinInfo.get()) = skininfo;
	mSkinnedInst->SkinnedInfo = mSkinInfo.get();
	mSkinnedInst->ClipName = L"default";

	Initialize(vertices, indices, subsets, materials, nodes, filename, texfilepath);

	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(SkinnedConstants), mConstantbuffer.GetAddressOf());

	return true;
}

bool Mesh::LoadZXCBin(const std::tstring & filename, const std::tstring & texfilepath)
{
	ZXCBinLoader loader;

	std::tstring file(filename, 0, filename.find_last_of(L"."));

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::vector<ZXCLoader::Subset> subsets;
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;

	if (!loader.LoadBinary(filename, vertices, indices, subsets, materials, nodes, mBoundingBox))
		return false;

	Initialize(vertices, indices, subsets, materials, nodes, filename, texfilepath);

	return true;
}

bool Mesh::LoadSkinBin(const std::tstring & filename, const std::tstring & texfilepath)
{
	ZXCBinLoader loader;

	std::tstring file(filename, 0, filename.find_last_of(L"."));

	std::vector<SkinnedVertex> vertices;
	std::vector<DWORD> indices;
	std::vector<ZXCLoader::Subset> subsets;
	std::vector<ZXCSMaterial> materials;
	std::vector<MeshNode> nodes;
	SkinnedData skininfo;

	if (!loader.LoadBinary(filename, vertices, indices, subsets, materials, nodes, mBoundingBox, &skininfo))
		return false;

	mSkinnedInst = std::make_unique<SkinnedModelInstance>();
	mSkinInfo = std::make_unique<SkinnedData>();

	mSkinnedInst->FinalTransforms.resize(nodes.size());
	*(mSkinInfo.get()) = skininfo;
	mSkinnedInst->SkinnedInfo = mSkinInfo.get();
	mSkinnedInst->ClipName = L"default";

	Initialize(vertices, indices, subsets, materials, nodes, filename, texfilepath);

	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(SkinnedConstants), mConstantbuffer.GetAddressOf());

	return true;
}

bool Mesh::LoadClip(const std::tstring & filename, const std::tstring & texfilepath)
{
	std::unordered_map<std::wstring, AnimationClip> clips;

	ClipLoader loader;
	loader.LoadClip(filename, (UINT)mNodeList.size(), clips);

	std::wstring name;
	for (auto& x : clips)
	{
		name = x.first;
		
		auto& p = std::make_pair(x.first, x.second);
		mSkinInfo->AddAnimation(p);
		mSkinnedInst->Animations.push_back(name);
	}

	mSkinnedInst->setClipName(name);

	return true;
}

bool Mesh::LoadClipBin(const std::tstring & filename, const std::tstring & texfilepath)
{
	std::unordered_map<std::wstring, AnimationClip> clips;

	ClipBinLoader loader;
	loader.LoadClip(filename, (UINT)mNodeList.size(), clips);

	std::wstring name;
	for (auto& x : clips)
	{
		name = x.first;

		auto& p = std::make_pair(name, x.second);
		mSkinInfo->AddAnimation(p);
		mSkinnedInst->Animations.push_back(name);
	}

	mSkinnedInst->setClipName(name);

	return true;
}

void Mesh::BuildRenderItem(
	const std::vector<ZXCLoader::Subset>& subsets,
	const std::vector<ZXCSMaterial>& materials)
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
				mat->Specular = XMFLOAT3(1.0f, 1.0f, 1.0f);
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
			mat->Specular = XMFLOAT3(1.0f, 1.0f, 1.0f);
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

	if (S_Input.getKeyState(DIK_K) == Input::KEYSTATE::KEY_PUSH)
	{
		numDrawItem = (numDrawItem + 1) % (UINT)mDrawItem.size();
	}

	return true;
}

bool Mesh::DebugRender(ID3D11DeviceContext * context)
{
	if (mConstantbuffer != nullptr)
	{
		context->UpdateSubresource(mConstantbuffer.Get(), 0, nullptr, &mSkinnedConstants, 0, 0);
		context->VSSetConstantBuffers(3, 1, mConstantbuffer.GetAddressOf());
	}
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

	UINT i = 0;

	for (auto&x : mDrawItem)
	{
		//if (i != numDrawItem)
		//{
		//	++i;
		//	continue;
		//}
		context->IASetPrimitiveTopology(x->PrimitiveType);
		x->Mat->SetResource(context);
		context->VSSetConstantBuffers(1, 1, x->ConstantBuffer.GetAddressOf());
		context->DrawIndexedInstanced(x->IndexCount, 1, x->StartIndexLocation, x->BaseVertexLocation, 0);
		++i;
	}
	
	return true;
}

void Mesh::SetWorld(FXMMATRIX world)
{
	XMStoreFloat4x4(&mWorld, world);
	if (!mDrawItem.empty())
	{
		for (auto&x : mDrawItem)
		{
			XMStoreFloat4x4(&x->World, world);
		}
	}
}

void Mesh::SetWorld(const XMFLOAT4X4 & world)
{
	mWorld = world;
	if (!mDrawItem.empty())
	{
		for (auto&x : mDrawItem)
		{
			x->World = world;
		}
	}
}

bool Mesh::Intersects(FXMVECTOR& origin, FXMVECTOR& dir, DirectX::CXMMATRIX& invView, float& tmin)
{
	XMMATRIX W = XMLoadFloat4x4(&mWorld);
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	XMVECTOR rayOrigin = XMVector3TransformCoord(origin, toLocal);
	XMVECTOR rayDir = XMVector3TransformNormal(dir, toLocal);

	rayDir = XMVector3Normalize(rayDir);

	auto vertices = (Vertex*)mGeometry->VertexBufferCPU->GetBufferPointer();
	const auto& indices = (DWORD*)mGeometry->IndexBufferCPU->GetBufferPointer();
	UINT triCount = mGeometry->IndexBufferByteSize / sizeof(DWORD) / 3;

	tmin = MathHelper::Infinity;

	for (UINT i = 0; i < triCount; ++i)
	{
		UINT i0 = indices[i * 3 + 0];
		UINT i1 = indices[i * 3 + 1];
		UINT i2 = indices[i * 3 + 2];

		XMVECTOR v0 = XMLoadFloat3(&vertices[i0].p);
		XMVECTOR v1 = XMLoadFloat3(&vertices[i1].p);
		XMVECTOR v2 = XMLoadFloat3(&vertices[i2].p);

		float t = 0.0f;
		if (TriangleTests::Intersects(rayOrigin, rayDir, v0, v1, v2, t))
		{
			tmin = t;
			return true;
		}
	}
	tmin = 0.0f;
	return false;
}