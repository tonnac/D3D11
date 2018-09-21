#pragma once
#include "Plane_Object.h"

class Background : public Plane_Object
{
public:
	Background();
public:
	void SetPos(const D2D1_POINT_2F& pos, const D2D1_RECT_F& rect) override;
	bool InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath) override;
public:
	bool Frame()override;
private:
	D2D1_RECT_F	m_rtDraw;
};