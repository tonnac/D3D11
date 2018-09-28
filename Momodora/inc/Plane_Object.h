#pragma once
#include "Object.h"


class Plane_Object : public Object
{
public:
	Plane_Object();
	~Plane_Object();
public:
	virtual bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
							const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	virtual void			SetTexPos(const D3DXVECTOR4& DrawVec) override;
public:
	bool					Frame() override;
public:
	virtual COL				Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize) override;
protected:
	virtual void			CreateIndexBuffer(ID3D11Device* pDevice);
protected:
	bool					PreRender(ID3D11DeviceContext* pContext) override;
	bool					PostRender(ID3D11DeviceContext* pContext) override;
protected:
	FLOAT					m_fScale;
	std::vector<DWORD>		m_indiciesList;
	D3DXVECTOR2				m_LengthDiff;
};