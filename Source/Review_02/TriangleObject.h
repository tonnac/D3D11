#pragma once
#include "Object.h"

class TriangleObject : public Object
{
public:
	TriangleObject() = default;
public:
	bool				Frame() override;
	bool				PostRender(ID3D11DeviceContext* pContext) override;
private:
	void CreateVertexBuffer(ID3D11Device* pDevice) override;
	void CreateIndexBuffer(ID3D11Device* pDevice) override;
};