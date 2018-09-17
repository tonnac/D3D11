#pragma once
#include "Object.h"

class PlaneObject : public Object
{
public:
	PlaneObject() = default;
public:
	bool				Frame() override;
private:
	void CreateVertexBuffer(ID3D11Device* pDevice) override;
	void CreateIndexBuffer(ID3D11Device* pDevice) override;
};