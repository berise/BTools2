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
	m_Page2.SetTitle(_T("iPerfClient"));
	AddPage(m_Page2);

	m_Page3.SetTitle(_T("iPerfServer"));
	AddPage(m_Page3);

	SetActivePage(1);
}



//LRESULT CBTools2View::OnInitDialog(HWND hwndFocus, LPARAM lParam)
int  CBTools2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	::AtlMessageBox(NULL, L"init");
				/*
                if(SHCreateMenuBar(&mbi))
                {
                    g_hwndMb          = mbi.hwndMB;
                    TBBUTTONINFO tbbi = {0};
                    tbbi.cbSize       = sizeof(tbbi);
                    tbbi.dwMask       = TBIF_LPARAM | TBIF_BYINDEX;

                    SendMessage(g_hwndMb, TB_GETBUTTONINFO,0, (LPARAM)&tbbi);
                    g_hMenu           = (HMENU)tbbi.lParam;
                }
				*/


	return TRUE; // set focus to default control
}


LRESULT CBTools2View::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	::AtlMessageBox(NULL, L"init");				/*
                if(SHCreateMenuBar(&mbi))
                {
                    g_hwndMb          = mbi.hwndMB;
                    TBBUTTONINFO tbbi = {0};
                    tbbi.cbSize       = sizeof(tbbi);
                    tbbi.dwMask       = TBIF_LPARAM | TBIF_BYINDEX;

                    SendMessage(g_hwndMb, TB_GETBUTTONINFO,0, (LPARAM)&tbbi);
                    g_hMenu           = (HMENU)tbbi.lParam;
                }
				*/


	return TRUE; // set focus to default control
}


void CBTools2View::OnDestroy()
{
	//::AtlMessageBox(NULL, L"OnDestroy");
}

LRESULT CBTools2View::OnMenuAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	::AtlMessageBox(NULL, L"View::OnMenuAbout");
	

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
