#pragma once


// ToolPane

#include "DeDlg.h"

class ToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(ToolPane)

public:
	DeDlg *mDeDlg = nullptr;

public:
	ToolPane();
	virtual ~ToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


