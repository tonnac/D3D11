#pragma once
#include "Plane_Object.h"

class EmptyScreen : public Plane_Object
{
public:
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
				const std::string& VSFunc = "VS", const std::string& PSFunc = "VS");
	bool Frame();
public:
	void SetPos(const D3DXVECTOR4& Vec) override;
private:

};