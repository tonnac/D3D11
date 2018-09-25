#pragma once
#include "Effect.h"

class KahoAttack : public Effect
{
public:
	virtual bool	Frame() override;
protected:
	INT				m_iValidIndex;
};

class KahoAttack1 : public KahoAttack
{
public:
	void			SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos) override;
};

class KahoAttack2 : public KahoAttack
{
public:
	void			SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos) override;
};

class KahoAttack3 : public KahoAttack
{
public:
	void			SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos) override;
};

class KahoAirAttack : public KahoAttack
{
public:
	void			SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos) override;
};

//class KahoBowAttack : public Effect
//{
//public:
//	void		SetPos(ID3D11Device * pDevice, const D3DXVECTOR2& Centerpos) override;
//	bool		Frame() override;
//};