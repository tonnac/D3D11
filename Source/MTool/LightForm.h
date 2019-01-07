#pragma once

#include "LightAdd.h"

// LightForm 대화 상자

class LightForm : public CFormView
{
	DECLARE_DYNAMIC(LightForm)

public:
	LightForm(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LightForm();

	static LightForm* CreateOne(CWnd* pParent);

//	std::unique_ptr<LightAdd> mAddLight = nullptr;
	LightAdd mAddLight;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LightForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox LightList;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnDropdownLightlist();
	afx_msg void OnCbnCloseupLightlist();
};
