#pragma once
#include "Plane_Object.h"

class Background : public Plane_Object
{
public:
	Background();
public:
	virtual void	SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight);
	void			SetInversePos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& MaxWidth, const FLOAT& MaxHeight);
public:
	bool			Frame() override;
	bool			Scroll(const FLOAT& pos) override;
	bool			CanScrollLeft();
	bool			CanScrollRight();
private:
	void			BasicSet(const FLOAT& rtLeft, const FLOAT& MaxWidth);
private:
	D3DXVECTOR2		m_MaxSize;
};

using BackgroundPTR = std::shared_ptr<Background>;
