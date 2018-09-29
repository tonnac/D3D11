#pragma once
#include "Bar.h"

class ProgressBar : public Bar
{
public:
	ProgressBar();
public:
	void		SetTexPos(const D3DXVECTOR4& DrawVec) override;
	bool		Frame() override;
public:
	void		setPercent(FLOAT* per);
private:
	FLOAT		m_MaxWidth;
	FLOAT*		m_Percent;
};