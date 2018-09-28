#pragma once
#include "Plane_Object.h"
#include "Character.h"

class Terrain : public Plane_Object
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
							const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void			SetPos(const D3DXVECTOR4& Pos) override;
public:
	bool			Scroll(const FLOAT& pos) override;
	virtual COL		Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize) override;
	virtual COL		Collision(std::shared_ptr<Character> pObject);
private:
	bool			PreRender(ID3D11DeviceContext* pContext);
};