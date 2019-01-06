// CreateDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "CreateDialog.h"
#include "afxdialogex.h"


// CreateDialog 대화 상자

IMPLEMENT_DYNAMIC(CreateDialog_, CDialogEx)

CreateDialog_::CreateDialog_(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_dtd(0)
	, m_dqd(0)
{

}

CreateDialog_::~CreateDialog_()
{
}

void CreateDialog_::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dtd);
	DDV_MinMaxUInt(pDX, m_dtd, 0, 1024);
	DDX_Text(pDX, IDC_EDIT2, m_dqd);
}


BEGIN_MESSAGE_MAP(CreateDialog_, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateDialog_::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CreateDialog_::OnBnClickedButton1)
END_MESSAGE_MAP()


// CreateDialog 메시지 처리기


void CreateDialog_::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


BOOL CreateDialog_::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_dtd = -1;
	UpdateData(FALSE);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CreateDialog_::OnBnClickedButton1()
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		CMToolApp* app = (CMToolApp*)AfxGetApp();

		COLORREF color = dlg.GetColor();

		float r = (float)GetRValue(color) / 255;
		float g = (float)GetGValue(color) / 255;
		float b = (float)GetBValue(color) / 255;

		DirectX::XMFLOAT4 Color = DirectX::XMFLOAT4(r, g, b, 1.0f);

		app->mTool->setBackColor(&Color.x);
	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
