#pragma once


// CreateDialog 대화 상자

class CreateDialog_ : public CDialogEx
{
	DECLARE_DYNAMIC(CreateDialog_)

public:
	CreateDialog_(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CreateDialog_();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// ㅇㅅㅇ
	UINT m_dtd;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	// ㅇㅂㅇ
	UINT m_dqd;
	afx_msg void OnBnClickedButton1();
};
