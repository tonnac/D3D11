#pragma once
#include "Plane_Object.h"

class Terrain : public Plane_Object
{
public:
	bool	InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void	SetPos(const D3DXVECTOR4& Drawvec) override;
public:
	bool	Frame() override;
public:
	bool	Scroll(const FLOAT& pos) override;
private:
	void	ComputeVertex();
	bool	PreRender(ID3D11DeviceContext* pContext);
private:
	FLOAT	m_fxDiffHalf;
	FLOAT	m_fyDiffHalf;
};