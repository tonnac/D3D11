#pragma once

// LightForm 대화 상자

class LightForm : public CPaneDlg
{
	DECLARE_DYNAMIC(LightForm)

public:
	LightForm(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LightForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LightForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void UpdateLightList();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox LightList;
	afx_msg void OnBnClickedButton4();

	CComboBox LightType;
	afx_msg void OnCbnSelchangeCombo1();
	int mDirRadio = -1;
	afx_msg void OnBnClickedDirclockwise();
	afx_msg void OnBnClickedDirrclockwise();
	CButton DIRCLOCKWISE;
	CButton DIRROT;
	CButton POSCLOCKWISE;
	CButton DirCounterClock;
	CButton PosCounterClock;
	CButton PosRot;
	CButton isPrimary;
	afx_msg void OnBnClickedDirrot();

	afx_msg void OnBnClickedCheck2();

	afx_msg void OnEnUpdateXpos();
	CEdit XPOS;
	CEdit DirRotSpeed;
	CEdit FallEnd;
	CEdit ZPOS;
	CEdit YPOS;
	CEdit XDIR;
	CEdit YDIR;
	CEdit ZDIR;
	CEdit FALLSTART;
	CEdit SPOTFACTOR;
	CEdit DirAxisX;
	CEdit DirAxisY;
	CEdit DirAxisZ;
	CEdit PosAxisX;
	CEdit PosAxisY;
	CEdit PosAxisZ;
	CEdit PosRotSpeed;
	afx_msg void OnEnUpdateYpos();
	afx_msg void OnEnUpdateZpos();
	afx_msg void OnEnUpdateXpos2();
	afx_msg void OnEnUpdateYpos2();
	afx_msg void OnEnUpdateZpos2();
	afx_msg void OnEnUpdateFallStart();
	afx_msg void OnEnUpdateFallEnd();
	afx_msg void OnEnUpdateSpotfactor();
	afx_msg void OnEnUpdateXpos3();
	afx_msg void OnEnUpdateYpos3();
	afx_msg void OnEnUpdateZpos3();
	afx_msg void OnEnUpdateDirrotspeed();
	afx_msg void OnEnUpdateXpos4();
	afx_msg void OnEnUpdateYpos4();
	afx_msg void OnEnUpdateZpos4();
	afx_msg void OnEnUpdatePosrotspeed();
	afx_msg void OnEnUpdateRstrength();
	afx_msg void OnEnUpdateGstrength();
	afx_msg void OnEnUpdateBstrength();
	CEdit RSTRENGTH;
	CEdit GSTRENGTH;
	CEdit BSTRENGTH;

	void Initialize();
	void GetLightProperties(int nIndex);

public:
	void ValueInit();
	afx_msg void OnCbnSelchangeLightlist();

private:
	void ReadDirectional(const LightProperty* lProperty);
	void ReadPoint(const LightProperty* lProperty);
	void ReadSpot(const LightProperty* lProperty);

	void SetFloat(TCHAR* buf, size_t bufLength, int nID, float value);

	void UpdateButton();
private:
	int mLightListSize = 1;
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	CButton mModifyButton;
	CButton mDeleteButton;
};
