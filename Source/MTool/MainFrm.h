
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

#include "CreateDD.h"
#include "ToolPane.h"
#include "EffectPant.h"
#include "LightPane.h"
#include "LightForm.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	CreateDD mSaveMap;
	ToolPane mToolpane;
	EffectPant mEffectpane;
	LightForm mLightForm;
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnToolMtool();
	afx_msg void SavaMap();
	afx_msg void OnSkyBox();
	afx_msg void OnUpdateSkyBox(CCmdUI *pCmdUI);

private:
	bool isSkyBox = true;
public:
	afx_msg void OnBackColor();
	afx_msg void OnUpdateBackColor(CCmdUI *pCmdUI);
	afx_msg void OnWireframe();
	afx_msg void OnUpdateWireframe(CCmdUI *pCmdUI);
	afx_msg void OnLightdockingView();
};


