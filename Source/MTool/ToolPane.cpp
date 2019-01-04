// ToolPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "ToolPane.h"


// ToolPane

IMPLEMENT_DYNAMIC(ToolPane, CDockablePane)

ToolPane::ToolPane()
{

}

ToolPane::~ToolPane()
{
}


BEGIN_MESSAGE_MAP(ToolPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// ToolPane 메시지 처리기




int ToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	mDeDlg = DeDlg::CreatedOne(this);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void ToolPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (mDeDlg)
	{
		mDeDlg->SetWindowPos(nullptr, 0, 0, cx, cy, SWP_NOZORDER);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int ToolPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
