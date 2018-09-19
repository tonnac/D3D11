#pragma once
#include "Core.h"
#include "PlaneObject.h"
#include "TriangleObject.h"


class Sample : public Core
{
public:
	bool				Init() override;
	bool				Frame() override;
	bool				Render() override;
	bool				Release() override;
	void				CreateRasterizer();
private:
	ID3D11RasterizerState * m_pRasterizer;
	ID3D11RasterizerState * m_pWireFrame;
	PlaneObject				m_object;
};