#pragma once
#include "Object.h"

class Plane_Object : public Object
{
public:
	Plane_Object() = default;
public:
	virtual bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath) override;
	virtual void			SetPos(const D2D1_POINT_2F& pos, const D2D1_RECT_F& rect);
	virtual bool			Init() override;
	virtual bool			Frame() override;
protected:
	void					CreateIndexBuffer(ID3D11Device* pDevice);
public:
	bool					PreRender(ID3D11DeviceContext* pContext) override;
	bool					PostRender(ID3D11DeviceContext* pContext) override;
protected:
	std::vector<DWORD>		m_indiciesList;
};