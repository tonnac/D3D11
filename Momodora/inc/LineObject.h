#pragma once
#include "Plane_Object.h"

class LineObject : public Plane_Object
{
public:
	bool	InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void	setVertex(const D2D1_RECT_F& Rt);
	bool	Render(ID3D11DeviceContext* pContext);
private:
};