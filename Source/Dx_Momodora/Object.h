#pragma once
#include "Object_Basic.h"

enum class COL
{
	NONE,
	LEFT,
	TOP,
	RIGHT,
	BOTTOM
};

COL& operator +=(COL& lhs, const COL& rhs);

class Object
{
public:
	Object();
public:
	virtual bool				InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
								const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				Render(ID3D11DeviceContext* pContext);
	virtual bool				Release();
	virtual void				SetTexPos(const D3DXVECTOR4& DrawVec);
	virtual void				SetCenterPos(const D3DXVECTOR2& Centerpos);
	virtual void				SetPos(const D3DXVECTOR4& Pos);
public:
	virtual COL					Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize);
	virtual bool				Scroll(const FLOAT& pos);
public:
	void						MoveCenterPos(const D3DXVECTOR2& vec);
public:
	D3DXVECTOR2					getCenterPos() const;
	D2D1_RECT_F					getCollisionRT() const;
protected:
	void						CreateVertexBuffer(ID3D11Device* pDevice);
	virtual void				CreateConstantBuffer(ID3D11Device* pDevice);
protected:
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
protected:
	Object_Basic				m_Object;
	VS_CB						m_ConstantData;
	std::vector<P3_VERTEX>		m_VertexList;
	D2D1_RECT_F					m_rtCollision;
	D3DXVECTOR2					m_Centerpos;
};