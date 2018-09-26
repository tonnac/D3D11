#pragma once
#include "Plane_Object.h"
#include "Sprite.h"

class Effect : public Plane_Object
{
public:
	Effect();
public:
	virtual bool		InitSet		(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
									const std::string& VSFunc = "VS", const std::string& PSFunc = "PS") override;
	virtual bool		Frame		();
	virtual bool		PostRender	(ID3D11DeviceContext* pContext);
	virtual bool		Release		();
	void				setSpeed	(const FLOAT&);
	void				setIndex	(const int&);
	void				reverseDir	();
	void				reverseSet	();
protected:
	INT					m_iDir;
	bool				isDebug;
	ID3D11PixelShader * m_pShader;
	Sprite *			m_pEffectSprite;
};