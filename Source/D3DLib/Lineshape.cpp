#include "Shape.h"
#include <DirectXColors.h>

using namespace DirectX;

LineShape::LineShape()
{
	
}

void LineShape::CreateInputLayout(ID3DBlob * vertexblob)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dUtil::CreateInputLayout(m_pDevice, (DWORD)vertexblob->GetBufferSize(),
		vertexblob->GetBufferPointer(), layout, (UINT)std::size(layout),
		mDxObject->m_pInputLayout.GetAddressOf());
}

void LineShape::BuildRenderItem(const std::tstring & textureFile)
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometries.get();
	rItem->World = MathHelper::Identity4x4();
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->IndexCount = rItem->Geo->DrawArgs["default"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs["default"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs["default"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	if (!textureFile.empty())
		d3dUtil::CreateShaderResourceView(m_pDevice, textureFile.c_str(), rItem->ShaderResourceView.GetAddressOf());
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem.push_back(rItem.get());
	S_RItem.SaveMiscItem(rItem);
}

void LineShape::BuildGeometry()
{
	std::vector<VertexC> vertices;
	std::vector<DWORD> indices;

	vertices =
	{
		VertexC({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)}),
		VertexC({XMFLOAT3(1.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)})
	};

	indices =
	{ 0, 1 };

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	CreateCPUBuffer(vertices.data(), indices.data(), vbByteSize, ibByteSize, sizeof(VertexC));

	Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob = nullptr;
	if (mDxObject->m_pVertexShader == nullptr)
	{
		d3dUtil::LoadVertexShaderFile(m_pDevice, L"line.hlsl", nullptr, mDxObject->m_pVertexShader.GetAddressOf(), "VS", vertexBlob.GetAddressOf());
		CreateInputLayout(vertexBlob.Get());
	}
	if (mDxObject->m_pPixelShader == nullptr)
		d3dUtil::LoadPixelShaderFile(m_pDevice, L"line.hlsl", nullptr, mDxObject->m_pPixelShader.GetAddressOf(), "PS");

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometries->DrawArgs["default"] = sub;
}

bool LineShape::Draw(ID3D11DeviceContext* pContext, XMFLOAT3 vStart, XMFLOAT3 vEnd, XMFLOAT4 vColor)
{
	m_LineVertexList =
	{
		VertexC({XMFLOAT3(vStart), XMFLOAT4(vColor)}),
		VertexC({XMFLOAT3(vEnd), XMFLOAT4(vColor)})
	};

	pContext->UpdateSubresource(mGeometries->VertexBuffer.Get(), 0, nullptr, m_LineVertexList.data(), 0, 0);

	return Render(pContext);
}





DirectionShape::DirectionShape()
{
}

void DirectionShape::CreateInputLayout(ID3DBlob * vertexblob)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dUtil::CreateInputLayout(m_pDevice, (DWORD)vertexblob->GetBufferSize(),
		vertexblob->GetBufferPointer(), layout, (UINT)std::size(layout), mDxObject->m_pInputLayout.GetAddressOf());
}

void DirectionShape::BuildGeometry()
{
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

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	CreateCPUBuffer(vertices.data(), indices.data(), vbByteSize, ibByteSize, sizeof(VertexC));

	Microsoft::WRL::ComPtr<ID3DBlob> vertexBlob = nullptr;
	if (mDxObject->m_pVertexShader == nullptr)
	{
		d3dUtil::LoadVertexShaderFile(m_pDevice, L"line.hlsl", nullptr, mDxObject->m_pVertexShader.GetAddressOf(), "VS", vertexBlob.GetAddressOf());
		CreateInputLayout(vertexBlob.Get());
	}
	if (mDxObject->m_pPixelShader == nullptr)
		d3dUtil::LoadPixelShaderFile(m_pDevice, L"line.hlsl", nullptr, mDxObject->m_pPixelShader.GetAddressOf(), "PS");

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometries->DrawArgs["default"] = sub;
}

void DirectionShape::BuildRenderItem(const std::tstring & textureFile)
{
	std::unique_ptr<RenderItem> rItem = std::make_unique<RenderItem>();
	rItem->Geo = mGeometries.get();
	rItem->World = MathHelper::Identity4x4();
	rItem->TexTransform = MathHelper::Identity4x4();
	rItem->IndexCount = rItem->Geo->DrawArgs["default"].IndexCount;
	rItem->StartIndexLocation = rItem->Geo->DrawArgs["default"].StartIndexLocation;
	rItem->BaseVertexLocation = rItem->Geo->DrawArgs["default"].BaseVertexLocation;
	rItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	if (!textureFile.empty())
		d3dUtil::CreateShaderResourceView(m_pDevice, textureFile.c_str(), rItem->ShaderResourceView.GetAddressOf());
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(ObjectConstants), rItem->ConstantBuffer.GetAddressOf());
	mRenderItem.push_back(rItem.get());
	S_RItem.SaveMiscItem(rItem);
}
