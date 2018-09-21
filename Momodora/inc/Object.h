#pragma once
#include "Object_Basic.h"

using InitPos = std::function<void(const D2D1_POINT_2F&, const D2D1_RECT_F&)>;

class Object
{
public:
	Object() = default;
public:
	virtual bool				InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath);
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
	bool						Render(ID3D11DeviceContext* pContext);
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual void				SetPos(const D2D1_POINT_2F& pos, const D2D1_RECT_F& rect);
public:
	InitPos						SetInitPos;
protected:
	void						CreateVertexBuffer(ID3D11Device* pDevice);
	void						CreateConstantBuffer(ID3D11Device* pDevice);
protected:
	D2D1_POINT_2F				ComputeCoord();
	D2D1_POINT_2F				ComputeCoord(const D2D1_POINT_2F& pos);
	void						ComputeCollision(const D2D1_POINT_2F& col);
protected:
	Object_Basic				m_Object;
	VS_CB						m_ConstantData;
	std::vector<RECT>			Sprite;
	D2D1_POINT_2F				m_Centerpos;
	D2D1_RECT_F					m_rtCollision;
	std::vector<P3_VERTEX>		m_VertexList;
};