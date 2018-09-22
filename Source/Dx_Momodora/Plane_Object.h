#pragma once
#include "Object.h"

class Plane_Object : public Object
{
public:
	Plane_Object() = default;
public:
	virtual bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
							const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
protected:
	virtual void			CreateIndexBuffer(ID3D11Device* pDevice);
protected:
	bool					PreRender(ID3D11DeviceContext* pContext) override;
	bool					PostRender(ID3D11DeviceContext* pContext) override;
protected:
	std::vector<DWORD>		m_indiciesList;
};