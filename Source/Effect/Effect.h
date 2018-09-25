#pragma once
#include "Plane_Object.h"
#include "Sprite.h"

class Effect : public Plane_Object
{
public:
	Effect();
public:
	virtual void		SetPos		(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos);
	virtual bool		Frame		();
	virtual bool		PostRender	(ID3D11DeviceContext* pContext);
	void				setSpeed	(const FLOAT&);
	void				setIndex	(const int&);
protected:
	bool				isDebug;
	ID3D11PixelShader * m_pShader;
	Sprite *			m_pEffectSprite;
};