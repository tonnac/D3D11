#include "HelperViewer.h"

HelperViewer::HelperViewer(ID3D11Device* device, const std::tstring & fileName)
	: md3Device(device)
{
	std::unique_ptr<MeshGeometry> geo = std::make_unique<MeshGeometry>();

	mGeometry = geo.get();
	S_Geometry.SaveGeometry(geo);

	Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob = nullptr;
	d3dUtil::LoadVertexShaderFile(md3Device, fileName.c_str(), nullptr, mDebugShader.m_pVertexShader.GetAddressOf(), "VS", vertexBlob.GetAddressOf());
	CreateInputLayout(vertexBlob.Get());
	d3dUtil::LoadPixelShaderFile(md3Device, fileName.c_str(), nullptr, mDebugShader.m_pPixelShader.GetAddressOf());
	d3dUtil::LoadGeometryShaderFile(md3Device, fileName.c_str(), nullptr, mDebugShader.m_pGeometryShader.GetAddressOf());
}

void HelperViewer::BuildVBIB()
{
	const UINT vbByteSize = sizeof(HelperViewer::Vertex) * (UINT)mVertices.size();
	const UINT ibByteSize = sizeof(DWORD) * (UINT)mIndices.size();

	D3DCreateBlob(vbByteSize, mGeometry->VertexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->VertexBufferCPU->GetBufferPointer(), mVertices.data(), vbByteSize);

	D3DCreateBlob(ibByteSize, mGeometry->IndexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->IndexBufferCPU->GetBufferPointer(), mIndices.data(), ibByteSize);

	mGeometry->VertexBufferByteSize = vbByteSize;
	mGeometry->VertexByteStride = sizeof(HelperViewer::Vertex);
	mGeometry->IndexBufferByteSize = ibByteSize;
	mGeometry->IndexFormat = DXGI_FORMAT_R32_UINT;

	d3dUtil::CreateVertexBuffer(md3Device,
		mGeometry->VertexBufferByteSize,
		mGeometry->VertexBufferCPU->GetBufferPointer(),
		mGeometry->VertexBuffer.GetAddressOf());

	d3dUtil::CreateIndexBuffer(md3Device,
		mGeometry->IndexBufferByteSize,
		mGeometry->IndexBufferCPU->GetBufferPointer(),
		mGeometry->IndexBuffer.GetAddressOf());
}

void HelperViewer::AddRenderItem(MeshNode * node)
{
	static DWORD index = 0;

	Vertex v;
	v.Center = node->box.Center;
	v.Max = node->box.Extents;

	mVertices.push_back(v);

	mIndices.push_back(index++);

	SubmeshGeometry submesh;

	std::unique_ptr<RenderItem> ritem = std::make_unique<RenderItem>();
	ritem->BaseVertexLocation = submesh.BaseVertexLocation = 0;
	ritem->StartIndexLocation = submesh.StartIndexLocation = index - 1;
	ritem->IndexCount = submesh.IndexCount = 1;
	ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	ritem->TexTransform = MathHelper::Identity4x4();
	ritem->World = MathHelper::Identity4x4();
	ritem->Geo = mGeometry;

	d3dUtil::CreateConstantBuffer(md3Device, 1, sizeof(ObjectConstants), ritem->ConstantBuffer.GetAddressOf());

	mGeometry->DrawArgs[node->NodeName] = submesh;
	
	mDebugItem.push_back(ritem.get());
	node->Ritem.push_back(ritem.get());
	S_RItem.SaveMiscItem(ritem);
}

bool HelperViewer::Render(ID3D11DeviceContext * context)
{
	UINT Stride = sizeof(HelperViewer::Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &Stride, &offset);
	context->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);
	mDebugShader.SetResource(context);
	for (auto&x : mDebugItem)
	{
		context->IASetPrimitiveTopology(x->PrimitiveType);
		context->VSSetConstantBuffers(1, 1, x->ConstantBuffer.GetAddressOf());
		context->DrawIndexedInstanced(x->IndexCount, 1, x->StartIndexLocation, x->BaseVertexLocation, 0);
	}

	return true;
}

void HelperViewer::CreateInputLayout(ID3DBlob * vertexBlob)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"CENTER", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"MAX"	 , 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dUtil::CreateInputLayout(md3Device,
		(DWORD)vertexBlob->GetBufferSize(),
		vertexBlob->GetBufferPointer(),
		layout,
		(UINT)std::size(layout),
		mDebugShader.m_pInputLayout.GetAddressOf());

}
