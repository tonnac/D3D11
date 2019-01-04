
// MToolView.cpp: CMToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MTool.h"
#endif

#include "MToolDoc.h"
#include "MToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMToolView

IMPLEMENT_DYNCREATE(CMToolView, CView)

BEGIN_MESSAGE_MAP(CMToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMToolView 생성/소멸

CMToolView::CMToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMToolView::~CMToolView()
{
}

BOOL CMToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMToolView 그리기

void CMToolView::OnDraw(CDC* /*pDC*/)
{
	CMToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMToolView 인쇄


void CMToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMToolView 진단

#ifdef _DEBUG
void CMToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMToolDoc* CMToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMToolDoc)));
	return (CMToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMToolView 메시지 처리기


void CMToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CMToolApp* app = (CMToolApp*)AfxGetApp();
	g_ClientWidth = cx;
	g_ClientHeight = cy;
	if(app->mTool != nullptr)
		app->mTool->OnResize();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
