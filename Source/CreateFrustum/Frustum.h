#pragma once

#include "d3dUtil.h"
#include "Shape.h"

struct Plane
{
	Plane()
	{};
	Plane(const DirectX::XMFLOAT4& rhs)
	{
		mNormal.x = rhs.x;
		mNormal.y = rhs.y;
		mNormal.z = rhs.z;
		mDistance = rhs.w;
	}
	Plane(const DirectX::XMFLOAT3& normal, float distance)
	{
		mNormal = normal;
		mDistance = distance;
	}
	Plane(float a, float b, float c, float d)
	{
		mNormal.x = a;
		mNormal.y = b;
		mNormal.z = c;
		mDistance = d;
	}
	Plane(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2)
	{
		CreatePlane(v0, v1, v2);
	}
	float operator[](int index)
	{
		switch (index)
		{
		case 0:
			return mNormal.x;
		case 1:
			return mNormal.y;
		case 2:
			return mNormal.z;
		case 3:
			return mDistance;
		default:
			return mDistance;
			break;
		}
	};
	DirectX::XMVECTOR CreatePlane(DirectX::FXMVECTOR v0, DirectX::FXMVECTOR v1, DirectX::FXMVECTOR v2);
	DirectX::XMFLOAT3 mNormal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float mDistance = 0.0f;
};

class Frustum
{
public:
	Frustum() = default;
public:
	void Initialize(ID3D11Device* pDevice, const std::tstring& ShaderFile, DirectX::FXMMATRIX view, DirectX::FXMMATRIX proj);
	void Initialize(ID3D11Device* pDevice, const std::tstring& ShaderFile, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);

	void UpdateFrustum(DirectX::FXMMATRIX view, DirectX::FXMMATRIX proj);
	void UpdateFrustum(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& proj);

	bool Render(ID3D11DeviceContext* pContext);

private:
	void CreateVertex(ID3D11Device * pDevice);
	void CreateIndex(ID3D11Device * pDevice);
	void CreateShader(ID3D11Device * pDevice, const std::tstring& ShaderFile);

private:
	
	std::array<PC_VERTEX, 24> mVertices;
	std::array<UINT16, 36> mIndices;
	std::array<DirectX::XMFLOAT3, 8> mFrustum;
	std::array<Plane, 6> mPlane;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

	UINT stride = sizeof(PC_VERTEX);
	UINT offset = 0;
};