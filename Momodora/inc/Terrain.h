#pragma once
#include "Player.h"

class Terrain : public Plane_Object
{
public:
	bool			InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
							const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void			SetPos(const D3DXVECTOR4& Pos) override;
public:
	bool			Scroll(const FLOAT& pos) override;
	virtual COL		Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize) override;
//	virtual COL		Collision(std::shared_ptr<Character> pObject);
	virtual COL		Collision(std::shared_ptr<Player> pPlayer);
private:
	bool			PreRender(ID3D11DeviceContext* pContext);
};

using TerrainPTR = std::shared_ptr<Terrain>;
