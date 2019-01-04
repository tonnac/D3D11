#pragma once

#include "EffectForm.h"

// EffectPant

class EffectPant : public CDockablePane
{
	DECLARE_DYNAMIC(EffectPant)

public:
	EffectForm* m_Effect;

public:
	EffectPant();
	virtual ~EffectPant();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


