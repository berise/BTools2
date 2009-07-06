// BTools2View.cpp : implementation of the CBTools2View class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resourceppc.h"

#include "BTPing.h"
#include "BTools2View.h"




BOOL CBTools2View::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

CBTools2View::CBTools2View()
{
	//SetTitle(_T("Properties"));
	//SetLinkText(_T("Tap <file:\\Windows\\default.htm{here}>."));

	m_Page1.SetTitle(_T("Ping"));
	AddPage(m_Page1);
	m_Page2.SetTitle(_T("iPerf Client"));
	AddPage(m_Page2);

	m_Page3.SetTitle(_T("iPerf Server"));
	AddPage(m_Page3);

	//SetActivePage(1);
}



//LRESULT CBTools2View::OnInitDialog(HWND hwndFocus, LPARAM lParam)
/*
int  CBTools2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	::AtlMessageBox(NULL, L"init");
	return TRUE; // set focus to default control
}


LRESULT CBTools2View::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	::AtlMessageBox(NULL, L"init");	
	return TRUE; // set focus to default control
}
*/

void CBTools2View::OnDestroy()
{
	//::AtlMessageBox(NULL, L"CBTools2View::OnDestroy");
}

LRESULT CBTools2View::OnMenuAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	//::AtlMessageBox(NULL, L"View::OnMenuAbout");
	

	return 0;
}

LRESULT CBTools2View::OnMenuPingtab(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	SetActivePage(0);

	return 0;
}

LRESULT CBTools2View::OnMenuClienttab(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	SetActivePage(1);

	return 0;
}

LRESULT CBTools2View::OnMenuServertab(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	SetActivePage(2	);

	return 0;
}

LRESULT CBTools2View::OnMenuExit(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	//SetMsgHandled(TRUE);
	
	::PostMessage(GetParent(), WM_CLOSE, NULL, NULL);
	
	return 0;
}


LRESULT CBTools2View::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//::AtlMessageBox(NULL, L"View::OnClose");
	EndDialog(m_hWnd, IDCANCEL);
	return 0;
}

LRESULT CBTools2View::OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//::AtlMessageBox(NULL, L"View::OnOKCancel");
	// EndDialog(m_hWnd, wID);
	return 0;
}