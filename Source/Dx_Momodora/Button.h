#pragma once
#include "Plane_Object.h"

class Button : public Plane_Object
{
public:
	Button();
public:
	bool	InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
					const std::string& VSFunc = "VS", const std::string& PSFunc = "PS");
	void	SetPos(const D3DXVECTOR2& CenterPos, const D2D1_POINT_2F& Size);
	void	AddVertex(const P3_VERTEX& vertex);
	void	pushIndecies();
	void	CreateConstant(ID3D11Device* pDevice);
public:
	bool	Init() override;
	bool	Frame() override;
	bool	Release();
private:
	void	DivideButton(const D3DXVECTOR2& CenterPos, const D2D1_POINT_2F& Size);
	void	CreateButtonVertex();
private:
	int iIndex;
	FLOAT fWidth;
	FLOAT fHeight;
	Button* pButton[9];
};