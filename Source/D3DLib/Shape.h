#pragma once
#include "Object.h"
#include "GeometryStroage.h"

template<typename X>
class Shape : public Object<X>
{
public:
	Shape() {};
	virtual ~Shape() {};

	using Object<X>::mGeometry;
	using Object<X>::m_pDevice;

public:
	void Create(ID3D11Device* pDevice, const std::tstring& textureFile = std::tstring(), const std::tstring& normalTex = std::tstring())
	{
		m_pDevice = pDevice;

		BuildGeometry();
		BuildMaterials(textureFile, normalTex);
		BuildRenderItem();
	}

	virtual bool Frame()override
	{
		return true;
	}
	virtual bool Render(ID3D11DeviceContext* pContext)override
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

	virtual void SetWorld(DirectX::FXMMATRIX world)override
	{
		XMStoreFloat4x4(&mRenderItem->World, world);
	}
	virtual void SetWorld(const DirectX::XMFLOAT4X4& world)override
	{
		
		mRenderItem->World = world;
	}
	virtual bool Intersects(DirectX::FXMVECTOR& origin, DirectX::FXMVECTOR& dir, DirectX::CXMMATRIX& invView, float& tmin)override
	{
		DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&mRenderItem->World);
		DirectX::XMMATRIX invWorld = DirectX::XMMatrixInverse(&XMMatrixDeterminant(W), W);

		DirectX::XMMATRIX toLocal = DirectX::XMMatrixMultiply(invView, invWorld);

		DirectX::XMVECTOR rayOrigin = DirectX::XMVector3TransformCoord(origin, toLocal);
		DirectX::XMVECTOR rayDir = DirectX::XMVector3TransformNormal(dir, toLocal);

		rayDir = DirectX::XMVector3Normalize(rayDir);

		const auto vertices = (X*)mGeometry->VertexBufferCPU->GetBufferPointer();
		const auto indices = (DWORD*)mGeometry->IndexBufferCPU->GetBufferPointer();

		UINT triSize = mGeometry->IndexBufferByteSize / sizeof(DWORD) / 3;

		float t = MathHelper::Infinity;

		for (UINT i = 0; i < triSize; ++i)
		{
			DWORD i0 = indices[i * 3 + 0];
			DWORD i1 = indices[i * 3 + 1];
			DWORD i2 = indices[i * 3 + 2];

			DirectX::XMVECTOR v0 = XMLoadFloat3(&vertices[i0].p);
			DirectX::XMVECTOR v1 = XMLoadFloat3(&vertices[i1].p);
			DirectX::XMVECTOR v2 = XMLoadFloat3(&vertices[i2].p);

			if (DirectX::TriangleTests::Intersects(rayOrigin, rayDir, v0, v1, v2, t))
			{
				tmin = t;
				return true;
			}
		}
		return false;
	}

protected:
	virtual void BuildGeometry() = 0;
	virtual void BuildRenderItem() = 0;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex) {};

protected:
	RenderItem* mRenderItem = nullptr;

};

class BoxShape : public Shape<Vertex>
{
public:
	BoxShape(bool isDice = false);
	virtual ~BoxShape() = default;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem()override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;
private:
	bool mIsDice = false;
};

class LineShape : public Shape<VertexC>
{
public:
	LineShape();
	virtual ~LineShape() = default;
protected:
	virtual void	BuildRenderItem()override;
	virtual void	BuildGeometry()override;

public:
	virtual	bool	Render(ID3D11DeviceContext* pContext)override;
	bool			Draw(ID3D11DeviceContext* pContext, DirectX::XMFLOAT3 vStart, DirectX::XMFLOAT3 vEnd, DirectX::XMFLOAT4 vColor);

protected:
	std::array<VertexC, 2> m_LineVertexList;
};

class PlaneShape : public Shape<Vertex>
{
public:
	PlaneShape() = default;
	virtual ~PlaneShape() = default;

public:
	virtual bool Render(ID3D11DeviceContext* pContext)override;

protected:
	virtual void BuildGeometry()override;
};

class DirectionShape : public Shape<VertexC>
{
public:
	DirectionShape();
	virtual ~DirectionShape() = default;

public:
	virtual bool Render(ID3D11DeviceContext* pContext)override;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem()override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;

protected:
	std::array<VertexC, 6> m_LineVertexList;
};

class SkyBox : public Shape<VertexP>
{
public:
	SkyBox() = default;
	~SkyBox() = default;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem()override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;
};

class GridShape : public Shape<Vertex>
{
public:
	GridShape() = default;
	~GridShape() = default;

public:
	void SetProperties(float width, float height, std::uint32_t rowNum, std::uint32_t colNum);

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem()override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;

private:
	float mWidth = 0.0f;
	float mHeight = 0.0f;
	std::uint32_t mRowNum = 0;
	std::uint32_t mColNum = 0;
};

class SphereShape : public Shape<Vertex>
{
public:
	SphereShape() = default;
	~SphereShape() = default;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem()override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;
};