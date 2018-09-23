#pragma once
#include "Object_Basic.h"

class Object
{
public:
	Object();
public:
	virtual bool				InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
								const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	virtual bool				Init();
	virtual bool				Frame();
	bool						Render(ID3D11DeviceContext* pContext);
	bool						Release();
	virtual void				SetPos(const D3DXVECTOR2& pos);
	virtual void				SetPos(const D3DXVECTOR4& DrawVec);
public:
	virtual void				Scroll(const FLOAT& pos);
protected:
	void						CreateVertexBuffer(ID3D11Device* pDevice);
	virtual void				CreateConstantBuffer(ID3D11Device* pDevice);
protected:
	void						SetPos(const D3DXVECTOR2& pos, const D3DXVECTOR4& rect);
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
protected:
	Object_Basic				m_Object;
	VS_CB						m_ConstantData;
	std::vector<P3_VERTEX>		m_VertexList;
	D2D1_RECT_F					m_rtCollision;
	D3DXVECTOR4					m_DrawVec;
	D3DXVECTOR2					m_Centerpos;
};