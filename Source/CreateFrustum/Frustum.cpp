#include "Frustum.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

XMVECTOR Plane::CreatePlane(FXMVECTOR v0, FXMVECTOR v1, FXMVECTOR v2)
{
	XMVECTOR vEdge0 = XMVectorSubtract(v1, v0);
	XMVECTOR vEdge1 = XMVectorSubtract(v2, v0);

	XMVECTOR vNormal = XMVector3Cross(vEdge0, vEdge1);
	XMStoreFloat3(&mNormal, vNormal);
	mDistance = -XMVectorGetX(XMVector3Dot(v0, vNormal));

	XMVECTOR retV = XMVectorSet(mNormal.x, mNormal.y, mNormal.z, mDistance);
	return retV;
}



void Frustum::Initialize(ID3D11Device * pDevice, const std::tstring & ShaderFile, DirectX::FXMMATRIX view, DirectX::FXMMATRIX proj)
{
	UpdateFrustum(view, proj);
	CreateVertex(pDevice);
	CreateIndex(pDevice);
	CreateShader(pDevice, ShaderFile);
}

void Frustum::Initialize(ID3D11Device * pDevice, const std::tstring & ShaderFile, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj)
{
	XMMATRIX viewM = XMLoadFloat4x4(&view);
	XMMATRIX projM = XMLoadFloat4x4(&proj);
	UpdateFrustum(viewM, projM);
	CreateVertex(pDevice);
	CreateIndex(pDevice);
	CreateShader(pDevice, ShaderFile);
}


void Frustum::UpdateFrustum(FXMMATRIX view, FXMMATRIX proj)
{
	XMMATRIX InvViewProj = XMMatrixInverse(&XMMatrixDeterminant(view * proj), view * proj);
	
	mFrustum[0] = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	mFrustum[1] = XMFLOAT3(-1.0f, +1.0f, 0.0f);
	mFrustum[2] = XMFLOAT3(+1.0f, +1.0f, 0.0f);
	mFrustum[3] = XMFLOAT3(+1.0f, -1.0f, 0.0f);

	mFrustum[4] = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	mFrustum[5] = XMFLOAT3(-1.0f, +1.0f, 1.0f);
	mFrustum[6] = XMFLOAT3(+1.0f, +1.0f, 1.0f);
	mFrustum[7] = XMFLOAT3(+1.0f, -1.0f, 1.0f);

	XMVECTOR f[8];
	for (int i = 0; i < 8; ++i)
	{
		f[i] = XMLoadFloat3(&mFrustum[i]);
	}

	for (int i = 0; i < 8; ++i)
	{
		f[i] = XMVector3TransformCoord(f[i], InvViewProj);
	}

	for (int i = 0; i < 8; ++i)
	{
		XMStoreFloat3(&mFrustum[i], f[i]);
	}

	mPlane[0].CreatePlane(f[0], f[1], f[2]);
	mPlane[1].CreatePlane(f[6], f[5], f[4]);

	mPlane[2].CreatePlane(f[2], f[6], f[7]);
	mPlane[3].CreatePlane(f[5], f[1], f[0]);

	mPlane[4].CreatePlane(f[1], f[5], f[6]);
	mPlane[5].CreatePlane(f[0], f[3], f[7]);

	mVertices[0].p = mFrustum[0];
	mVertices[1].p = mFrustum[1];
	mVertices[2].p = mFrustum[2];
	mVertices[3].p = mFrustum[3];

	mVertices[4].p = mFrustum[4];
	mVertices[5].p = mFrustum[7];
	mVertices[6].p = mFrustum[6];
	mVertices[7].p = mFrustum[5];

	mVertices[8].p = mFrustum[1];
	mVertices[9].p = mFrustum[5];
	mVertices[10].p = mFrustum[6];
	mVertices[11].p = mFrustum[2];
	
	mVertices[12].p = mFrustum[0];
	mVertices[13].p = mFrustum[3];
	mVertices[14].p = mFrustum[4];
	mVertices[15].p = mFrustum[7];
	
	mVertices[16].p = mFrustum[4];
	mVertices[17].p = mFrustum[5];
	mVertices[18].p = mFrustum[1];
	mVertices[19].p = mFrustum[0];
	
	mVertices[20].p = mFrustum[3];
	mVertices[21].p = mFrustum[2];
	mVertices[22].p = mFrustum[6];
	mVertices[23].p = mFrustum[7];
}

void Frustum::UpdateFrustum(const DirectX::XMFLOAT4X4 & view, const DirectX::XMFLOAT4X4 & proj)
{
	XMMATRIX viewM = XMLoadFloat4x4(&view);
	XMMATRIX projM = XMLoadFloat4x4(&proj);
	UpdateFrustum(viewM, projM);
}



bool Frustum::Render(ID3D11DeviceContext * pContext)
{
	pContext->UpdateSubresource(mVertexBuffer.Get(), 0, nullptr, mVertices.data(), 0, 0);

	pContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetInputLayout(mInputLayout.Get());

	pContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	pContext->DrawIndexed((UINT)mIndices.size(), 0, 0);
	return true;
}

void Frustum::CreateVertex(ID3D11Device * pDevice)
{
	mVertices[0] = PC_VERTEX(mFrustum[1], XMFLOAT4(Colors::Black));
	mVertices[1] = PC_VERTEX(mFrustum[2], XMFLOAT4(Colors::Black));
	mVertices[2] = PC_VERTEX(mFrustum[3], XMFLOAT4(Colors::Black));
	mVertices[3] = PC_VERTEX(mFrustum[0], XMFLOAT4(Colors::Black));

	mVertices[4] = PC_VERTEX(mFrustum[6], XMFLOAT4(Colors::Cornsilk));
	mVertices[5] = PC_VERTEX(mFrustum[5], XMFLOAT4(Colors::Cornsilk));
	mVertices[6] = PC_VERTEX(mFrustum[4], XMFLOAT4(Colors::Cornsilk));
	mVertices[7] = PC_VERTEX(mFrustum[7], XMFLOAT4(Colors::Cornsilk));

	mVertices[8] = PC_VERTEX(mFrustum[5], XMFLOAT4(Colors::Purple));
	mVertices[9] = PC_VERTEX(mFrustum[1], XMFLOAT4(Colors::Purple));
	mVertices[10] = PC_VERTEX(mFrustum[0], XMFLOAT4(Colors::Purple));
	mVertices[11] = PC_VERTEX(mFrustum[4], XMFLOAT4(Colors::Purple));

	mVertices[12] = PC_VERTEX(mFrustum[2], XMFLOAT4(Colors::Olive));
	mVertices[13] = PC_VERTEX(mFrustum[6], XMFLOAT4(Colors::Olive));
	mVertices[14] = PC_VERTEX(mFrustum[7], XMFLOAT4(Colors::Olive));
	mVertices[15] = PC_VERTEX(mFrustum[3], XMFLOAT4(Colors::Olive));

	mVertices[16] = PC_VERTEX(mFrustum[5], XMFLOAT4(Colors::Yellow));
	mVertices[17] = PC_VERTEX(mFrustum[6], XMFLOAT4(Colors::Yellow));
	mVertices[18] = PC_VERTEX(mFrustum[2], XMFLOAT4(Colors::Yellow));
	mVertices[19] = PC_VERTEX(mFrustum[1], XMFLOAT4(Colors::Yellow));

	mVertices[20] = PC_VERTEX(mFrustum[0], XMFLOAT4(Colors::Pink));
	mVertices[21] = PC_VERTEX(mFrustum[3], XMFLOAT4(Colors::Pink));
	mVertices[22] = PC_VERTEX(mFrustum[7], XMFLOAT4(Colors::Pink));
	mVertices[23] = PC_VERTEX(mFrustum[4], XMFLOAT4(Colors::Pink));

	d3dUtil::CreateVertexBuffer(pDevice, (UINT)mVertices.size(), sizeof(PC_VERTEX), mVertices.data(), mVertexBuffer.GetAddressOf());

}

void Frustum::CreateIndex(ID3D11Device * pDevice)
{
	mIndices[0] = 0; mIndices[1] = 1; mIndices[2] = 2;
	mIndices[3] = 0; mIndices[4] = 2; mIndices[5] = 3;

	mIndices[6] = 4; mIndices[7] = 5; mIndices[8] = 6;
	mIndices[9] = 4; mIndices[10] = 6; mIndices[11] = 7;

	mIndices[12] = 8; mIndices[13] = 9; mIndices[14] = 10;
	mIndices[15] = 8; mIndices[16] = 10; mIndices[17] = 11;

	mIndices[18] = 12; mIndices[19] = 13; mIndices[20] = 14;
	mIndices[21] = 12; mIndices[22] = 14; mIndices[23] = 15;

	mIndices[24] = 16; mIndices[25] = 17; mIndices[26] = 18;
	mIndices[27] = 16; mIndices[28] = 18; mIndices[29] = 19;

	mIndices[30] = 20; mIndices[31] = 21; mIndices[32] = 22;
	mIndices[33] = 20; mIndices[34] = 22; mIndices[35] = 23;

	d3dUtil::CreateIndexBuffer(pDevice, (UINT)mIndices.size(), sizeof(UINT16), mIndices.data(), mIndexBuffer.GetAddressOf());
}

void Frustum::CreateShader(ID3D11Device * pDevice, const std::tstring & ShaderFile)
{
	ComPtr<ID3DBlob> mVertexBlob;
	d3dUtil::LoadVertexShaderFile(pDevice, ShaderFile.c_str(), mVertexShader.GetAddressOf(), "VS", mVertexBlob.GetAddressOf());
	d3dUtil::LoadPixelShaderFile(pDevice, ShaderFile.c_str(), mPixelShader.GetAddressOf());

	D3D11_INPUT_ELEMENT_DESC inputlayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0 ,0, D3D11_INPUT_PER_VERTEX_DATA},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0 ,12, D3D11_INPUT_PER_VERTEX_DATA},
	};

	d3dUtil::CreateInputLayout(pDevice, (DWORD)mVertexBlob->GetBufferSize(), mVertexBlob->GetBufferPointer(), inputlayout, (UINT)std::size(inputlayout), mInputLayout.GetAddressOf());

}
