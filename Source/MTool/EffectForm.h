#pragma once


// EffectForm 대화 상자

class EffectForm : public CFormView
{
	DECLARE_DYNAMIC(EffectForm)

public:
	EffectForm(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EffectForm();

public:
	static EffectForm* CreatedOne(CWnd* pParent);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EffectForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
