#pragma once

#include "LightForm.h"

// LightPane

class LightPane : public CDockablePane
{
	DECLARE_DYNAMIC(LightPane)

public:
	LightPane();
	virtual ~LightPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


