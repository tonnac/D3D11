#pragma once
#include "Plane_Object.h"

class Terrain : public Plane_Object
{
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void SetPos(const D3DXVECTOR4& DrawVec) override;
public:
	bool Frame() override;
public:
	void Scroll(const FLOAT& pos) override;
protected:
	void CreateIndexBuffer(ID3D11Device* pDevice) override;
private:
	void ConvertScreenCoord();
	void ComputeVertex();
	bool PreRender(ID3D11DeviceContext* pContext);

};