#pragma once
#include "Object.h"

class Plane_Object : public Object
{
public:
	Plane_Object() = default;
public:
	virtual bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath) override;
	virtual bool			Init() override;
	virtual bool			Frame() override;
public:
	void					VertexAdd(const P3_VERTEX& vertex);
protected:
	void					CreateIndexBuffer(ID3D11Device* pDevice);
public:
	bool					PreRender(ID3D11DeviceContext* pContext) override;
	bool					PostRender(ID3D11DeviceContext* pContext) override;
public:
	Object_Basic*			getBasic();
	ID3D11Buffer*			getVertexBuffer();
	P3_VERTEX*				getVertex();
protected:
	std::vector<DWORD>		m_indiciesList;
};