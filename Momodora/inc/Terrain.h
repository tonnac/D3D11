#pragma once
#include "Plane_Object.h"

class Character;

class Terrain : public Plane_Object
{
public:
	bool	InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void	SetPos(const D3DXVECTOR4& Pos) override;
public:
	bool	Scroll(const FLOAT& pos) override;
	COL		Collision(Object* pObject, FLOAT* ColSize = nullptr) override;
	COL		Collision(Character* pObject);
private:
	bool	PreRender(ID3D11DeviceContext* pContext);
};