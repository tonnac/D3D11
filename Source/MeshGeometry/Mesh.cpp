#include "..\MeshGeometry\Mesh.h"
#include "Mesh.h"

using namespace DirectX;

bool Mesh::LoadFile(const std::tstring & filename, ID3D11Device * device)
{
	m_pDevice = device;
	ZXCLoader loader;

	std::tstring Ext(filename, filename.find_last_of(L".") + 1, filename.length());

	std::vector<Vertex> vertices;
	std::vector<SkinnedVertex> vertices0;
	std::vector<DWORD> indices;
	std::map<std::pair<UINT, int>, std::vector<std::pair<int, ZXCLoader::Subset>>> subsets;
	std::map<std::pair<UINT, int>, std::vector<std::pair<UINT, std::wstring>>> materials;
	std::vector<MeshNode> nodes;
	SkinnedData skininfo;
	if (Ext == L"ZXCS")
	{
		if (!loader.LoadZXC(filename, vertices0, indices, subsets, materials, nodes, skininfo))
			false;
	}
	else
	{
		if (!loader.LoadZXC(filename, vertices, indices, subsets, materials, nodes, skininfo))
			false;
	}

	mSkinnedInst = std::make_unique<SkinnedModelInstance>();

	mSkinnedInst->FinalTransforms.resize(nodes.size());
	mSkinnedInst->SkinnedInfo = new SkinnedData;
	*(mSkinnedInst->SkinnedInfo) = skininfo;
	mSkinnedInst->ClipName = "default";

	const UINT vbByteSize = (UINT)vertices0.size() * sizeof(SkinnedVertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	CreateCPUBuffer(vertices0.data(), indices.data(), vbByteSize, ibByteSize, sizeof(SkinnedVertex));

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
			ritem->BaseVertexLocation = submesh.BaseVertexLocation = k.second.VertexStart;
			ritem->IndexCount = submesh.IndexCount = k.second.FaceCount * 3;
			ritem->StartIndexLocation = submesh.StartIndexLocation = k.second.FaceStart * 3;
			ritem->Geo = mGeometries.get();
			ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			ritem->TexTransform = MathHelper::Identity4x4();
			//ritem->World = mNodeList[nodeIndex]->World;
			XMStoreFloat4x4(&ritem->World, XMMatrixIdentity());

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

	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(SkinnedConstants), mConstantbuffer.GetAddressOf());

	const D3D10_SHADER_MACRO skinnedDefines[] =
	{
		"SKINNED", "1",
		NULL, NULL
	};
	BuildDxObject(m_pDevice, L"shape.hlsl", skinnedDefines);

	return true;
}

bool Mesh::Frame()
{
	mSkinnedInst->UpdateSkinnedAnimation(g_fSecPerFrame);

	std::copy(
		std::begin(mSkinnedInst->FinalTransforms),
		std::end(mSkinnedInst->FinalTransforms),
		&mSkinnedConstants.BoneTransforms[0]
	);

	return true;
}

bool Mesh::Render(ID3D11DeviceContext * context)
{
	context->UpdateSubresource(mConstantbuffer.Get(), 0, nullptr, &mSkinnedConstants, 0, 0);
	context->IASetVertexBuffers(0, 1, mGeometries->VertexBuffer.GetAddressOf(), &Stride, &offset);
	context->IASetIndexBuffer(mGeometries->IndexBuffer.Get(), mGeometries->IndexFormat, 0);
	context->VSSetConstantBuffers(2, 1, mConstantbuffer.GetAddressOf());
	if (mDxObject != nullptr)
		mDxObject->SetResource(context);

	if (!mNodeList.empty())
	{
		for (auto&x : mNodeList)
		{
			if (x->Type == ObjectType::MESH)
			{
				for (auto&k : x->Ritem)
				{
					if (k->ShaderResourceView == nullptr)
						continue;
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

void Mesh::CreateInputLayout(ID3DBlob * vertexblob)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 24	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 40	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 48	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDICES"	, 0, DXGI_FORMAT_R8G8B8A8_UINT		, 0, 64	, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dUtil::CreateInputLayout(m_pDevice,
		(DWORD)vertexblob->GetBufferSize(),
		vertexblob->GetBufferPointer(),
		layout,
		(UINT)std::size(layout),
		mDxObject->m_pInputLayout.GetAddressOf());
}
