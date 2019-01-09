
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#pragma warning(disable : 4005)

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


class MFCUtil
{
public:
	static void AllowOnlyRealNum(CEdit *cedit, bool minus = true)
	{
		//// [ 1. initialize value ]
		CString cstrText;
		cedit->GetWindowText(cstrText);
		TCHAR chText[100] = { 0, };
		_tcscpy_s(chText, 100, cstrText);
		int count = (int)_tcslen(chText);

		//// [ 2. remove second point ]
		bool pointChecker = false;
		for (int i = 0; i < count; i++) {
			if (!pointChecker && chText[i] == '.') pointChecker = true;
			else if (pointChecker && chText[i] == '.') chText[i] = NULL;
		}

		//// [ 3. remove middle of minuse ]
		int startIdx = (minus) ? 1 : 0;
		for (int i = startIdx; i < count; i++) {
			if (chText[i] == '-') chText[i] = NULL;
		}

		//// [ 4. remove character ]
		for (int i = 0; i < count; i++) {
			if (!(chText[i] >= 48 && chText[i] <= 57)) {
				if (chText[i] != '.' && chText[i] != '-') chText[i] = NULL;
			}
		}

		//// [ 5. Set text ]
		cedit->SetWindowText(chText);

		//// [ 6. Move cursor to end ]
		cedit->SetSel(0, -1);
		cedit->SetSel(-1, -1);
	}

	static float getFloat(HWND hWnd, int nID)
	{
		float retV;
		TCHAR buf[256] = { 0, };
		GetDlgItemText(hWnd, nID, buf, 256);
		retV = (float)_ttof(buf);
		return retV;
	}
};

class CPaneDlg : public CPaneDialog
{
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) {
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}
};