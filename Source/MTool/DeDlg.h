#pragma once


// DeDlg 대화 상자

class DeDlg : public CFormView
{
	DECLARE_DYNAMIC(DeDlg)

public:
	DeDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DeDlg();

public:
	static DeDlg* CreatedOne(CWnd* pParent);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
