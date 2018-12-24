#include "..\MeshGeometry\Mesh.h"
#include "Mesh.h"

using namespace DirectX;

bool Mesh::LoadFile(const std::tstring & filename, ID3D11Device * device)
{
	m_pDevice = device;
	ZXCLoader loader;

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::map<std::pair<UINT, int>, std::vector<std::pair<int, ZXCLoader::Subset>>> subsets;
	std::map<std::pair<UINT, int>, std::vector<std::pair<UINT, std::wstring>>> materials;
	std::vector<MeshNode> nodes;
	SkinnedData skininfo;
	if (!loader.LoadZXC(filename, vertices, indices, subsets, materials, nodes, skininfo))
		false;

	mGeometries = std::make_unique<MeshGeometry>();
	mSkinnedInst = std::make_unique<SkinnedModelInstance>();

	mSkinnedInst->FinalTransforms.resize(nodes.size());
	mSkinnedInst->SkinnedInfo = new SkinnedData;
	*(mSkinnedInst->SkinnedInfo) = skininfo;
	mSkinnedInst->ClipName = "default";

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	CreateCPUBuffer(vertices.data(), indices.data(), vbByteSize, ibByteSize);

	d3dUtil::CreateVertexBuffer(m_pDevice,
		mGeometries->VertexBufferByteSize,
		mGeometries->VertexBufferCPU->GetBufferPointer(),
		mGeometries->VertexBuffer.GetAddressOf());

	d3dUtil::CreateIndexBuffer(m_pDevice,
		mGeometries->IndexBufferByteSize,
		mGeometries->IndexBufferCPU->GetBufferPointer(),
		mGeometries->IndexBuffer.GetAddressOf());

	mNodeList.resize(nodes.size());
	for (UINT i = 0; i < (UINT)nodes.size(); ++i)
	{
		mNodeList[i] = std::make_unique<MeshNode>();
		*(mNodeList[i].get()) = nodes[i];
	}

	UINT c = -1;
	for (auto&x : subsets)
	{
		for (auto&k : x.second)
		{
			UINT nodeIndex = x.first.first;
			int mtrlRef = x.first.second;
			int mtlID = k.first;

			SubmeshGeometry submesh;
			std::string name = std::to_string(++c);

			std::unique_ptr<RenderItem> ritem = std::make_unique<RenderItem>();
			ritem->BaseVertexLocation = k.second.VertexStart;
			ritem->IndexCount = k.second.FaceCount * 3;
			ritem->StartIndexLocation = k.second.FaceStart * 3;
			ritem->Geo = mGeometries.get();
			ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			ritem->TexTransform = MathHelper::Identity4x4();
			ritem->World = mNodeList[nodeIndex]->World;

			auto matindex = std::pair<UINT, int>(mtrlRef, mtlID);

			std::wstring texPath = L"..\\..\\data\\tex\\";
			std::wstring texFile;
			if (!materials[matindex].empty())
			{
				texFile = materials[matindex][0].second;
			}
			else if (materials.size() == 1)
			{
				texFile = materials[std::pair<UINT, int>(mtrlRef, -1)][0].second;
			}
			else
			{
				texPath = std::wstring();
				texFile = std::wstring();
			}

			d3dUtil::CreateShaderResourceView(m_pDevice, texPath + texFile, ritem->ShaderResourceView.GetAddressOf());
			d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), ritem->ConstantBuffer.GetAddressOf());
			
			mGeometries->DrawArgs[name] = submesh;
			mNodeList[nodeIndex]->Ritem.push_back(ritem.get());
			if (mNodeList[nodeIndex]->Type == ObjectType::MESH)
				S_RItem.mOpaqueItem.push_back(ritem.get());
			else
				S_RItem.mMiscItem.push_back(ritem.get());
			S_RItem.mAllRitem.push_back(std::move(ritem));
		}
	}

	D3D11_BUFFER_DESC bufDesc = {};
	ZeroMemory(&bufDesc, sizeof(D3D11_BUFFER_DESC));

	bufDesc.ByteWidth = sizeof(XMFLOAT4X4) * (UINT)mNodeList.size();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufDesc.StructureByteStride = sizeof(XMFLOAT4X4);

	m_pDevice->CreateBuffer(&bufDesc, nullptr, mConstantbuffer.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.BufferEx.NumElements = (UINT)mNodeList.size();

	m_pDevice->CreateShaderResourceView(mConstantbuffer.Get(), &srvDesc, mBufferview.GetAddressOf());

	BuildDxObject(m_pDevice, L"shape.hlsl", nullptr);

	return true;
}

bool Mesh::Frame()
{
	XMFLOAT4X4 ppp[16];
	mSkinnedInst->UpdateSkinnedAnimation(g_fSecPerFrame);
	for (size_t i = 0; i < mSkinnedInst->FinalTransforms.size(); ++i)
	{
		XMMATRIX E = XMLoadFloat4x4(&mSkinnedInst->FinalTransforms[i]);
		XMStoreFloat4x4(&ppp[i], XMMatrixTranspose(E));
	}

	for (UINT i = 0; i < (UINT)mNodeList.size(); ++i)
	{
		for (auto&x : mNodeList[i]->Ritem)
		{
			x->World = mSkinnedInst->FinalTransforms[i];
		}
	}

	return true;
}

bool Mesh::Render(ID3D11DeviceContext * context)
{
	context->IASetVertexBuffers(0, 1, mGeometries->VertexBuffer.GetAddressOf(), &Stride, &offset);
	context->IASetIndexBuffer(mGeometries->IndexBuffer.Get(), mGeometries->IndexFormat, 0);
	if (mDxObject != nullptr)
		mDxObject->SetResource(context);
	UINT i = 0;
	if (!mNodeList.empty())
	{
		for (auto&x : mNodeList)
		{
			if (x->Type == ObjectType::MESH)
			{
				for (auto&k : x->Ritem)
				{
					context->IASetPrimitiveTopology(k->PrimitiveType);
					context->PSSetShaderResources(0, 1, k->ShaderResourceView.GetAddressOf());
					context->VSSetConstantBuffers(1, 1, k->ConstantBuffer.GetAddressOf());
					context->DrawIndexedInstanced(k->IndexCount, 1, k->StartIndexLocation, k->BaseVertexLocation, 0);
				}
			}
		}
	}
	return true;
}