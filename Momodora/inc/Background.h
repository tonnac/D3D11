#pragma once
#include "Plane_Object.h"

class Background : public Plane_Object
{
public:
	Background();
public:
	void SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight);
public:
	bool Frame() override;
private:
	void TexPosUpdate();
private:
	const FLOAT m_fWidth;
	const FLOAT m_fHeight;
	D2D1_RECT_F	m_MaxSize;
};