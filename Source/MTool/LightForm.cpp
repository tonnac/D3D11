// LightForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "LightForm.h"
#include "afxdialogex.h"


// LightForm 대화 상자

IMPLEMENT_DYNAMIC(LightForm, CFormView)

LightForm::LightForm(CWnd* pParent /*=nullptr*/)
	: CFormView(IDD_LightForm)
{

}

LightForm::~LightForm()
{
}

LightForm * LightForm::CreateOne(CWnd * pParent)
{
	static LightForm * pForm = new LightForm;
	pForm->Create(
		NULL, NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500),
		pParent, 0, NULL);
	return pForm;
}

void LightForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, LightList);
}


BEGIN_MESSAGE_MAP(LightForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON4, &LightForm::OnBnClickedButton4)
	ON_CBN_DROPDOWN(IDC_LightList, &LightForm::OnCbnDropdownLightlist)
	ON_CBN_CLOSEUP(IDC_LightList, &LightForm::OnCbnCloseupLightlist)
END_MESSAGE_MAP()


// LightForm 메시지 처리기

void LightForm::OnBnClickedButton4()
{
	mAddLight.DestroyWindow();
	//mAddLight.reset();

	//mAddLight = std::make_unique<LightAdd>();

	if (mAddLight.GetSafeHwnd() == nullptr)
	{
		mAddLight.Create(IDD_ADD_LIGHT);
	}
	mAddLight.ShowWindow(SW_SHOW);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnCbnDropdownLightlist()
{
	LightVec *lights = LightStorage::getLight()->GetLightVec();

	for (UINT i = 0; i < (UINT)lights->size(); ++i)
	{
		CString str;

		TCHAR LType[30] = { 0, };
		TCHAR Primary[10] = { 0, };

		if ((*lights)[i]->isPrimary)
		{
			_stprintf_s(Primary, std::size(Primary), _T("주 광원"));
		}

		switch ((*lights)[i]->Type)
		{
		case LightType::Directional:
			_stprintf_s(LType, std::size(LType), _T("Directional"));
			break;
		case LightType::Point:
			_stprintf_s(LType, std::size(LType), _T("Point"));
			break;
		case LightType::Spot:
			_stprintf_s(LType, std::size(LType), _T("Spot"));
			break;
		default:
			break;
		}

		str.Format(_T("%d번 조명: %s %s"), i, LType, Primary);

		LightList.AddString(str);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnCbnCloseupLightlist()
{
	LightList.ResetContent();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
