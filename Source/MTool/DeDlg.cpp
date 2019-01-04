// DeDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "DeDlg.h"
#include "afxdialogex.h"


// DeDlg 대화 상자

IMPLEMENT_DYNAMIC(DeDlg, CFormView)

DeDlg::DeDlg(CWnd* pParent /*=nullptr*/)
	: CFormView(IDD_DeDlg)
{

}

DeDlg::~DeDlg()
{
}

DeDlg * DeDlg::CreatedOne(CWnd * pParent)
{
	DeDlg * pForm = new DeDlg;
	pForm->Create(
		NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent, 0, NULL);
	return pForm;
}

void DeDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DeDlg, CFormView)
END_MESSAGE_MAP()


// DeDlg 메시지 처리기
