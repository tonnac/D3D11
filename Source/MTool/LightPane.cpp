// LightPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "LightPane.h"


// LightPane

IMPLEMENT_DYNAMIC(LightPane, CDockablePane)

LightPane::LightPane()
{

}

LightPane::~LightPane()
{
}


BEGIN_MESSAGE_MAP(LightPane, CDockablePane)
	ON_WM_MOUSEACTIVATE()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// LightPane 메시지 처리기


//
//
//int LightPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	CFrameWnd* pParentFrame = GetParentFrame();
//	if (pParentFrame == pDesktopWnd ||
//		pDesktopWnd->IsChild(pParentFrame))
//	{
//		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
//	}
//	return MA_NOACTIVATE;
//}
//
//
//int LightPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
//
//	return 0;
//}
