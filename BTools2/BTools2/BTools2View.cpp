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
	m_Page2.SetTitle(_T("Client"));
	AddPage(m_Page2);

	m_Page3.SetTitle(_T("Server"));
	AddPage(m_Page3);
}



//LRESULT CBTools2View::OnInitDialog(HWND hwndFocus, LPARAM lParam)
int  CBTools2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	SHMENUBARINFO mbi;
                ZeroMemory(&mbi, sizeof(SHMENUBARINFO));

                mbi.cbSize     = sizeof(SHMENUBARINFO);
                mbi.hwndParent = m_hWnd;
                mbi.nToolBarId = IDR_MENU1;
				mbi.hInstRes   = _Module.GetModuleInstance();
                mbi.dwFlags    = SHCMBF_HMENU;

				SHCreateMenuBar(&mbi);

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

	SHMENUBARINFO mbi;
                ZeroMemory(&mbi, sizeof(SHMENUBARINFO));

                mbi.cbSize     = sizeof(SHMENUBARINFO);
                mbi.hwndParent = m_hWnd;
                mbi.nToolBarId = IDR_MENU1;
				mbi.hInstRes   = _Module.GetResourceInstance();
                mbi.dwFlags    = SHCMBF_HMENU;

				SHCreateMenuBar(&mbi);

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


void CBTools2View::OnDestroy()
{
	//::AtlMessageBox(NULL, L"OnDestroy");
}
