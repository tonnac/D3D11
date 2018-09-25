#pragma once
#include "Plane_Object.h"
#include "Sprite.h"

class State;
using StateList = std::map<std::tstring, State*>;

class Character : public Plane_Object
{
public:
	Character();
public:
	void				SetPos(ID3D11Device * pDevice,const D3DXVECTOR2& Centerpos);
	bool				Init() override;
	bool				Frame() override;
	bool				PostRender(ID3D11DeviceContext* pContext) override;
	bool				Release() override;
private:
	FLOAT				m_fScale;
	bool				isDebug;
	ID3D11PixelShader * m_pShader;
	State*				m_pCurrentState;
	StateList			m_StateList;
};