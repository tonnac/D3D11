// CreateDD.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "CreateDD.h"
#include "afxdialogex.h"


// CreateDD 대화 상자

IMPLEMENT_DYNAMIC(CreateDD, CDialogEx)

CreateDD::CreateDD(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CreateDD::~CreateDD()
{
}

void CreateDD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CreateDD, CDialogEx)
END_MESSAGE_MAP()


// CreateDD 메시지 처리기
