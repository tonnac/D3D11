// EffectForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "EffectForm.h"
#include "afxdialogex.h"


// EffectForm 대화 상자

IMPLEMENT_DYNAMIC(EffectForm, CFormView)

EffectForm::EffectForm(CWnd* pParent /*=nullptr*/)
	: CFormView(IDD_EffectForm)
{

}

EffectForm::~EffectForm()
{
}

EffectForm * EffectForm::CreatedOne(CWnd * pParent)
{
	EffectForm * pForm = new EffectForm;
	pForm->Create(
		NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent, 0, NULL);
	return pForm;
}

void EffectForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EffectForm, CFormView)
END_MESSAGE_MAP()


// EffectForm 메시지 처리기
