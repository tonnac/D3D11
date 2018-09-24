#pragma once
#include "Plane_Object.h"
#include "Sprite.h"

class Character : public Plane_Object
{
public:
	Character();
public:
	void	SetPos(ID3D11Device * pDevice,const D3DXVECTOR2& Centerpos, const D3DXVECTOR4& DrawVec);
	bool	Frame() override;
	bool	PostRender(ID3D11DeviceContext* pContext) override;
private:
	FLOAT	m_fScale;
	bool	isDebug;
	ID3D11PixelShader * m_pShader;
	Sprite*				m_pSprite;
};