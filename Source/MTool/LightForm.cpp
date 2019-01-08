// LightForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "LightForm.h"
#include "afxdialogex.h"

using namespace DirectX;

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
	DDX_Control(pDX, IDC_COMBO1, LightType);
	DDX_Control(pDX, IDC_DIRCLOCKWISE, DIRCLOCKWISE);
	DDX_Control(pDX, IDC_DIRROT, DIRROT);
	DDX_Control(pDX, IDC_POSCLOCKWISE, POSCLOCKWISE);
	DDX_Control(pDX, IDC_DIRRCLOCKWISE, DirCounterClock);
	DDX_Control(pDX, IDC_POSRCLOCKWISE, PosCounterClock);
	DDX_Control(pDX, IDC_CHECK2, PosRot);
	DDX_Control(pDX, IDC_CHECK3, isPrimary);
	DDX_Control(pDX, IDC_XPOS, XPOS);
	DDX_Control(pDX, IDC_DIRROTSPEED, DirRotSpeed);
	DDX_Control(pDX, IDC_FALL_END, FallEnd);
	DDX_Control(pDX, IDC_ZPOS, ZPOS);
	DDX_Control(pDX, IDC_YPOS, YPOS);
	DDX_Control(pDX, IDC_XPOS2, XDIR);
	DDX_Control(pDX, IDC_YPOS2, YDIR);
	DDX_Control(pDX, IDC_ZPOS2, ZDIR);
	DDX_Control(pDX, IDC_FALL_START, FALLSTART);
	DDX_Control(pDX, IDC_SPOTFACTOR, SPOTFACTOR);
	DDX_Control(pDX, IDC_XPOS3, DirAxisX);
	DDX_Control(pDX, IDC_YPOS3, DirAxisY);
	DDX_Control(pDX, IDC_ZPOS3, DirAxisZ);
	DDX_Control(pDX, IDC_XPOS4, PosAxisX);
	DDX_Control(pDX, IDC_YPOS4, PosAxisY);
	DDX_Control(pDX, IDC_ZPOS4, PosAxisZ);
	DDX_Control(pDX, IDC_POSROTSPEED, PosRotSpeed);
	DDX_Control(pDX, IDC_RSTRENGTH, RSTRENGTH);
	DDX_Control(pDX, IDC_GSTRENGTH, GSTRENGTH);
	DDX_Control(pDX, IDC_BSTRENGTH, BSTRENGTH);
}


BEGIN_MESSAGE_MAP(LightForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON4, &LightForm::OnBnClickedButton4)
	ON_CBN_DROPDOWN(IDC_LightList, &LightForm::OnCbnDropdownLightlist)
	ON_CBN_CLOSEUP(IDC_LightList, &LightForm::OnCbnCloseupLightlist)
	ON_CBN_SELCHANGE(IDC_COMBO1, &LightForm::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_DIRCLOCKWISE, &LightForm::OnBnClickedDirclockwise)
	ON_BN_CLICKED(IDC_DIRRCLOCKWISE, &LightForm::OnBnClickedDirrclockwise)
	ON_BN_CLICKED(IDC_DIRROT, &LightForm::OnBnClickedDirrot)
	ON_BN_CLICKED(IDC_CHECK2, &LightForm::OnBnClickedCheck2)
	ON_EN_UPDATE(IDC_XPOS, &LightForm::OnEnUpdateXpos)
	ON_EN_UPDATE(IDC_YPOS, &LightForm::OnEnUpdateYpos)
	ON_EN_UPDATE(IDC_ZPOS, &LightForm::OnEnUpdateZpos)
	ON_EN_UPDATE(IDC_XPOS2, &LightForm::OnEnUpdateXpos2)
	ON_EN_UPDATE(IDC_YPOS2, &LightForm::OnEnUpdateYpos2)
	ON_EN_UPDATE(IDC_ZPOS2, &LightForm::OnEnUpdateZpos2)
	ON_EN_UPDATE(IDC_FALL_START, &LightForm::OnEnUpdateFallStart)
	ON_EN_UPDATE(IDC_FALL_END, &LightForm::OnEnUpdateFallEnd)
	ON_EN_UPDATE(IDC_SPOTFACTOR, &LightForm::OnEnUpdateSpotfactor)
	ON_EN_UPDATE(IDC_XPOS3, &LightForm::OnEnUpdateXpos3)
	ON_EN_UPDATE(IDC_YPOS3, &LightForm::OnEnUpdateYpos3)
	ON_EN_UPDATE(IDC_ZPOS3, &LightForm::OnEnUpdateZpos3)
	ON_EN_UPDATE(IDC_DIRROTSPEED, &LightForm::OnEnUpdateDirrotspeed)
	ON_EN_UPDATE(IDC_XPOS4, &LightForm::OnEnUpdateXpos4)
	ON_EN_UPDATE(IDC_YPOS4, &LightForm::OnEnUpdateYpos4)
	ON_EN_UPDATE(IDC_ZPOS4, &LightForm::OnEnUpdateZpos4)
	ON_EN_UPDATE(IDC_POSROTSPEED, &LightForm::OnEnUpdatePosrotspeed)
	ON_EN_UPDATE(IDC_RSTRENGTH, &LightForm::OnEnUpdateRstrength)
	ON_EN_UPDATE(IDC_GSTRENGTH, &LightForm::OnEnUpdateGstrength)
	ON_EN_UPDATE(IDC_BSTRENGTH, &LightForm::OnEnUpdateBstrength)
END_MESSAGE_MAP()


// LightForm 메시지 처리기

void LightForm::OnBnClickedButton4()
{
	switch (LightType.GetCurSel())
	{
	case 0:
	{
		std::unique_ptr<LightProperty> light = std::make_unique<LightProperty>();
		light->Type = LightType::Directional;
		light->light.Direction = XMFLOAT3(-MFCUtil::getFloat(m_hWnd, IDC_XPOS2), -MFCUtil::getFloat(m_hWnd, IDC_YPOS2), -MFCUtil::getFloat(m_hWnd, IDC_ZPOS2));
		light->light.Strength = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_RSTRENGTH), MFCUtil::getFloat(m_hWnd, IDC_GSTRENGTH), MFCUtil::getFloat(m_hWnd, IDC_BSTRENGTH));
		light->isPrimary = isPrimary.GetCheck();
		light->DirRot.isRotate = DIRROT.GetCheck();
		if (light->DirRot.isRotate)
		{
			light->DirRot.Axis = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_XPOS3), MFCUtil::getFloat(m_hWnd, IDC_YPOS3), MFCUtil::getFloat(m_hWnd, IDC_ZPOS3));
			light->DirRot.isClockwise = (bool)DIRCLOCKWISE.GetCheck() == true ? true : false;
			light->DirRot.RotSpeed = MFCUtil::getFloat(m_hWnd, IDC_DIRROTSPEED);
		}
		LightStorage::getLight()->AddLight(light);
	}break;
	case 1:
	{
		std::unique_ptr<LightProperty> light = std::make_unique<LightProperty>();
		light->Type = LightType::Point;
		light->light.Position = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_XPOS), MFCUtil::getFloat(m_hWnd, IDC_YPOS), MFCUtil::getFloat(m_hWnd, IDC_ZPOS));
		light->light.Strength = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_RSTRENGTH), MFCUtil::getFloat(m_hWnd, IDC_GSTRENGTH), MFCUtil::getFloat(m_hWnd, IDC_BSTRENGTH));
		light->isPrimary = isPrimary.GetCheck();
		light->PosRot.isRotate = PosRot.GetCheck();
		light->light.FalloffStart = MFCUtil::getFloat(m_hWnd, IDC_FALL_START);
		light->light.FalloffEnd = MFCUtil::getFloat(m_hWnd, IDC_FALL_END);
		if (light->PosRot.isRotate)
		{
			light->PosRot.Axis = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_XPOS4), MFCUtil::getFloat(m_hWnd, IDC_YPOS4), MFCUtil::getFloat(m_hWnd, IDC_ZPOS4));
			light->PosRot.isClockwise = (bool)POSCLOCKWISE.GetCheck() == true ? true : false;
			light->PosRot.RotSpeed = MFCUtil::getFloat(m_hWnd, IDC_POSROTSPEED);
		}
		LightStorage::getLight()->AddLight(light);
	}break;
	case 2:
	{
		std::unique_ptr<LightProperty> light = std::make_unique<LightProperty>();
		light->Type = LightType::Spot;
		light->light.Direction = XMFLOAT3(-MFCUtil::getFloat(m_hWnd, IDC_XPOS2), -MFCUtil::getFloat(m_hWnd, IDC_YPOS2), -MFCUtil::getFloat(m_hWnd, IDC_ZPOS2));
		light->light.Strength = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_RSTRENGTH), MFCUtil::getFloat(m_hWnd, IDC_GSTRENGTH), MFCUtil::getFloat(m_hWnd, IDC_BSTRENGTH));
		light->isPrimary = isPrimary.GetCheck();
		light->DirRot.isRotate = DIRROT.GetCheck();
		if (light->DirRot.isRotate)
		{
			light->DirRot.Axis = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_XPOS3), MFCUtil::getFloat(m_hWnd, IDC_YPOS3), MFCUtil::getFloat(m_hWnd, IDC_ZPOS3));
			light->DirRot.isClockwise = (bool)DIRCLOCKWISE.GetCheck() == true ? true : false;
			light->DirRot.RotSpeed = MFCUtil::getFloat(m_hWnd, IDC_DIRROTSPEED);
		}
		light->light.Position = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_XPOS), MFCUtil::getFloat(m_hWnd, IDC_YPOS), MFCUtil::getFloat(m_hWnd, IDC_ZPOS));
		light->PosRot.isRotate = PosRot.GetCheck();
		light->light.FalloffStart = MFCUtil::getFloat(m_hWnd, IDC_FALL_START);
		light->light.FalloffEnd = MFCUtil::getFloat(m_hWnd, IDC_FALL_END);
		if (light->PosRot.isRotate)
		{
			light->PosRot.Axis = XMFLOAT3(MFCUtil::getFloat(m_hWnd, IDC_XPOS4), MFCUtil::getFloat(m_hWnd, IDC_YPOS4), MFCUtil::getFloat(m_hWnd, IDC_ZPOS4));
			light->PosRot.isClockwise = (bool)POSCLOCKWISE.GetCheck() == true ? true : false;
			light->PosRot.RotSpeed = MFCUtil::getFloat(m_hWnd, IDC_POSROTSPEED);
		}
		LightStorage::getLight()->AddLight(light);
	}break;
	default:
		break;
	}
	

	//mAddLight.DestroyWindow();
	////mAddLight.reset();

	////mAddLight = std::make_unique<LightForm>();

	//if (mAddLight.GetSafeHwnd() == nullptr)
	//{
	//	mAddLight.Create(IDD_ADD_LIGHT);
	//}
	//mAddLight.ShowWindow(SW_SHOW);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnCbnDropdownLightlist()
{
	LightVec *lights = LightStorage::getLight()->GetLightVec();

	CString str0 = _T("조명 추가");
	LightList.AddString(str0);

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


void LightForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	LightType.AddString(L"Directional");
	LightType.AddString(L"Point");
	LightType.AddString(L"Spot");
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	GetDlgItem(IDC_XPOS)->EnableWindow(false);
	GetDlgItem(IDC_YPOS)->EnableWindow(false);
	GetDlgItem(IDC_ZPOS)->EnableWindow(false);
	GetDlgItem(IDC_XPOS2)->EnableWindow(false);
	GetDlgItem(IDC_YPOS2)->EnableWindow(false);
	GetDlgItem(IDC_ZPOS2)->EnableWindow(false);
	GetDlgItem(IDC_DIRROT)->EnableWindow(false);
	GetDlgItem(IDC_DIRCLOCKWISE)->EnableWindow(false);
	GetDlgItem(IDC_DIRRCLOCKWISE)->EnableWindow(false);
	GetDlgItem(IDC_POSCLOCKWISE)->EnableWindow(false);
	GetDlgItem(IDC_POSRCLOCKWISE)->EnableWindow(false);
	GetDlgItem(IDC_XPOS3)->EnableWindow(false);
	GetDlgItem(IDC_YPOS3)->EnableWindow(false);
	GetDlgItem(IDC_ZPOS3)->EnableWindow(false);
	GetDlgItem(IDC_XPOS4)->EnableWindow(false);
	GetDlgItem(IDC_YPOS4)->EnableWindow(false);
	GetDlgItem(IDC_ZPOS4)->EnableWindow(false);
	GetDlgItem(IDC_CHECK2)->EnableWindow(false);
	GetDlgItem(IDC_CHECK3)->EnableWindow(false);
	GetDlgItem(IDC_DIRROTSPEED)->EnableWindow(false);
	GetDlgItem(IDC_POSROTSPEED)->EnableWindow(false);
	GetDlgItem(IDC_FALL_START)->EnableWindow(false);
	GetDlgItem(IDC_FALL_END)->EnableWindow(false);
	GetDlgItem(IDC_SPOTFACTOR)->EnableWindow(false);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void LightForm::OnCbnSelchangeCombo1()
{
	switch (LightType.GetCurSel())
	{
	case 0:
		GetDlgItem(IDC_XPOS)->EnableWindow(false);
		GetDlgItem(IDC_YPOS)->EnableWindow(false);
		GetDlgItem(IDC_ZPOS)->EnableWindow(false);
		GetDlgItem(IDC_XPOS2)->EnableWindow(true);
		GetDlgItem(IDC_YPOS2)->EnableWindow(true);
		GetDlgItem(IDC_ZPOS2)->EnableWindow(true);
		GetDlgItem(IDC_DIRROT)->EnableWindow(true);
		GetDlgItem(IDC_CHECK2)->EnableWindow(false);
		GetDlgItem(IDC_POSCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_POSRCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_XPOS4)->EnableWindow(false);
		GetDlgItem(IDC_YPOS4)->EnableWindow(false);
		GetDlgItem(IDC_ZPOS4)->EnableWindow(false);
		GetDlgItem(IDC_POSROTSPEED)->EnableWindow(false);
		GetDlgItem(IDC_FALL_START)->EnableWindow(false);
		GetDlgItem(IDC_FALL_END)->EnableWindow(false);
		GetDlgItem(IDC_SPOTFACTOR)->EnableWindow(false);
		GetDlgItem(IDC_CHECK3)->EnableWindow(true);
		break;
	case 1:
		GetDlgItem(IDC_XPOS)->EnableWindow(true);
		GetDlgItem(IDC_YPOS)->EnableWindow(true);
		GetDlgItem(IDC_ZPOS)->EnableWindow(true);
		GetDlgItem(IDC_XPOS2)->EnableWindow(false);
		GetDlgItem(IDC_YPOS2)->EnableWindow(false);
		GetDlgItem(IDC_ZPOS2)->EnableWindow(false);
		GetDlgItem(IDC_POSCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_POSRCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_XPOS3)->EnableWindow(false);
		GetDlgItem(IDC_YPOS3)->EnableWindow(false);
		GetDlgItem(IDC_ZPOS3)->EnableWindow(false);
		GetDlgItem(IDC_DIRROT)->EnableWindow(false);
		GetDlgItem(IDC_CHECK2)->EnableWindow(true);
		GetDlgItem(IDC_FALL_START)->EnableWindow(true);
		GetDlgItem(IDC_FALL_END)->EnableWindow(true);
		GetDlgItem(IDC_SPOTFACTOR)->EnableWindow(false);
		GetDlgItem(IDC_DIRROTSPEED)->EnableWindow(false);

		GetDlgItem(IDC_CHECK3)->EnableWindow(true);
		break;
	case 2:
		GetDlgItem(IDC_XPOS)->EnableWindow(true);
		GetDlgItem(IDC_YPOS)->EnableWindow(true);
		GetDlgItem(IDC_ZPOS)->EnableWindow(true);
		GetDlgItem(IDC_XPOS2)->EnableWindow(true);
		GetDlgItem(IDC_YPOS2)->EnableWindow(true);
		GetDlgItem(IDC_ZPOS2)->EnableWindow(true);
		GetDlgItem(IDC_DIRROT)->EnableWindow(true);
		GetDlgItem(IDC_CHECK2)->EnableWindow(true);
		GetDlgItem(IDC_FALL_START)->EnableWindow(true);
		GetDlgItem(IDC_FALL_END)->EnableWindow(true);
		GetDlgItem(IDC_SPOTFACTOR)->EnableWindow(true);
		GetDlgItem(IDC_CHECK3)->EnableWindow(true);
		break;
	default:
		break;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void LightForm::OnBnClickedDirclockwise()
{
	mDirRadio = 0;
	UpdateData(false);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnBnClickedDirrclockwise()
{
	mDirRadio = 1;
	UpdateData(false);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnBnClickedDirrot()
{
	isDirRot = !isDirRot;

	if (isDirRot)
	{
		GetDlgItem(IDC_DIRRCLOCKWISE)->EnableWindow(true);
		GetDlgItem(IDC_DIRCLOCKWISE)->EnableWindow(true);
		GetDlgItem(IDC_XPOS3)->EnableWindow(true);
		GetDlgItem(IDC_YPOS3)->EnableWindow(true);
		GetDlgItem(IDC_ZPOS3)->EnableWindow(true);
		GetDlgItem(IDC_DIRROTSPEED)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_DIRRCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_DIRCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_XPOS3)->EnableWindow(false);
		GetDlgItem(IDC_YPOS3)->EnableWindow(false);
		GetDlgItem(IDC_ZPOS3)->EnableWindow(false);
		GetDlgItem(IDC_DIRROTSPEED)->EnableWindow(false);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnBnClickedCheck2()
{
	isPosRot = !isPosRot;

	if (isPosRot)
	{
		GetDlgItem(IDC_POSCLOCKWISE)->EnableWindow(true);
		GetDlgItem(IDC_POSRCLOCKWISE)->EnableWindow(true);
		GetDlgItem(IDC_XPOS4)->EnableWindow(true);
		GetDlgItem(IDC_YPOS4)->EnableWindow(true);
		GetDlgItem(IDC_ZPOS4)->EnableWindow(true);
		GetDlgItem(IDC_POSROTSPEED)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_POSCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_POSRCLOCKWISE)->EnableWindow(false);
		GetDlgItem(IDC_XPOS4)->EnableWindow(false);
		GetDlgItem(IDC_YPOS4)->EnableWindow(false);
		GetDlgItem(IDC_ZPOS4)->EnableWindow(false);
		GetDlgItem(IDC_POSROTSPEED)->EnableWindow(false);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateXpos()
{
	MFCUtil::AllowOnlyRealNum(&XPOS);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void LightForm::OnEnUpdateYpos()
{
	MFCUtil::AllowOnlyRealNum(&YPOS);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateZpos()
{
	MFCUtil::AllowOnlyRealNum(&ZPOS);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateXpos2()
{
	MFCUtil::AllowOnlyRealNum(&XDIR);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateYpos2()
{
	MFCUtil::AllowOnlyRealNum(&YDIR);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateZpos2()
{
	MFCUtil::AllowOnlyRealNum(&ZDIR);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateFallStart()
{
	MFCUtil::AllowOnlyRealNum(&FALLSTART);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateFallEnd()
{
	MFCUtil::AllowOnlyRealNum(&FallEnd);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateSpotfactor()
{
	MFCUtil::AllowOnlyRealNum(&SPOTFACTOR);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateXpos3()
{
	MFCUtil::AllowOnlyRealNum(&DirAxisX);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateYpos3()
{
	MFCUtil::AllowOnlyRealNum(&DirAxisY);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateZpos3()
{
	MFCUtil::AllowOnlyRealNum(&DirAxisZ);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateDirrotspeed()
{
	MFCUtil::AllowOnlyRealNum(&DirRotSpeed);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateXpos4()
{
	MFCUtil::AllowOnlyRealNum(&PosAxisX);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateYpos4()
{
	MFCUtil::AllowOnlyRealNum(&PosAxisY);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateZpos4()
{
	MFCUtil::AllowOnlyRealNum(&PosAxisZ);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdatePosrotspeed()
{
	MFCUtil::AllowOnlyRealNum(&PosRotSpeed);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateRstrength()
{
	MFCUtil::AllowOnlyRealNum(&RSTRENGTH, false);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateGstrength()
{
	MFCUtil::AllowOnlyRealNum(&GSTRENGTH, false);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightForm::OnEnUpdateBstrength()
{
	MFCUtil::AllowOnlyRealNum(&BSTRENGTH, false);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하여, IParam 마스크에 OR 연산하여 설정된 ENM_SCROLL 플래그를 지정하여 컨트롤에 EM_SETEVENTMASK 메시지를 보내지 않으면
	// 편집 컨트롤이 바뀐 텍스트를 표시하려고 함을 나타냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
