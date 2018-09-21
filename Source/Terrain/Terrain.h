#pragma once
#include "Plane_Object.h"

class Terrain : public Plane_Object
{
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void SetPos(const D2D1_RECT_F& rt) override;
	void SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom);
public:
	void CreateIndexBuffer(ID3D11Device* pDevice) override;
	virtual bool PreRender(ID3D11DeviceContext* pContext);
private:
	void VertexUpdate();
};