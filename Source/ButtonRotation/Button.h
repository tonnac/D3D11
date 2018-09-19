#pragma once
#include "Object.h"

class Button : public Object
{
public:
	Button();
public:
	void	SetPos(const D2D1_POINT_2F& CenterPos, const D2D1_POINT_2F& Size);
	void	AddVertex(const P3_VERTEX& vertex);
	void	pushIndecies();
public:
	bool	Init() override;
	bool	Release() override;
private:
	void	CreateButtonVertex();
private:
	int iIndex;
	FLOAT fWidth;
	FLOAT fHeight;
	Button* pButton[9];
};