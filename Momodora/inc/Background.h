#pragma once
#include "Plane_Object.h"

class Background : public Plane_Object
{
public:
	Background();
public:
	void SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight);
	void SetInversePos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight);
public:
	bool Frame() override;
	bool Scroll(const FLOAT& pos) override;
private:
	void TexPosUpdate();
private:
	D2D1_RECT_F	m_MaxSize;
};