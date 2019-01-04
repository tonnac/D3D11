// EffectPant.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "EffectPant.h"


// EffectPant

IMPLEMENT_DYNAMIC(EffectPant, CDockablePane)

EffectPant::EffectPant()
{

}

EffectPant::~EffectPant()
{
}


BEGIN_MESSAGE_MAP(EffectPant, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// EffectPant 메시지 처리기




int EffectPant::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Effect = EffectForm::CreatedOne(this);

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void EffectPant::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_Effect)
	{
		m_Effect->SetWindowPos(nullptr, 0, 0, cx, cy, SWP_NOZORDER);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


int EffectPant::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
