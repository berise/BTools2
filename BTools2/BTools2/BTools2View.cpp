// BTools2View.cpp : implementation of the CBTools2View class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"

#include "resourceppc.h"
#include "aboutdlg.h"
#include "BTPing.h"
#include "BTools2View.h"

#include "Util.h"




BOOL CBTools2View::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

CBTools2View::CBTools2View()
{
	// image list
	// Init CPropertyView 
	//CTabCtrl *tab = GetTabControl();
	
	HINSTANCE hInstance = ModuleHelper::GetModuleInstance();

	
	m_ImageList16x16.Create(16, 16, ILC_COLOR32|ILC_MASK, 6, 3);
	m_ImageList16x16.AddIcon( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PING_16x)) ); // 
	m_ImageList16x16.AddIcon( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPERF_CLIENT_16x)) );
	m_ImageList16x16.AddIcon( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPERF_SERVER_16x)) );
	//m_ImageList16x16.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ABOUT1)) );
	//m_ImageList16x16.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ABOUT2)) );


	//SetTitle(_T("Properties"));
	//SetLinkText(_T("Tap <file:\\Windows\\default.htm{here}>."));

	m_Page1.SetView(this);
	m_Page1.SetTitle(_T("Ping"));
	AddPage(m_Page1);


	m_Page2.SetView(this);
	m_Page2.SetTitle(_T("iPerf Client"));
	AddPage(m_Page2);

	m_Page3.SetView(this);
	m_Page3.SetTitle(_T("iPerf Server"));
	AddPage(m_Page3);

	
	// 현재 프로그램 설정 파일 위치
	//TCHAR szTemp[MAX_PATH] = L"";
	SHGetSpecialFolderPath(m_hWnd, gszIniFile, CSIDL_PROGRAM_FILES, 0); 

	//_tcscpy(gszIniFile, szTemp);
	_tcscat(gszIniFile, L"\\BTools2\\BTools2.ini");

	OnSetDefaultIni(gszIniFile);


	// test code
	

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
		CAboutDlg dlg;
	dlg.DoModal();

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
	ShowWindow(SW_MINIMIZE);
	return 0;
}


//
void CBTools2View::OnSetDefaultIni(TCHAR *pszFileName)
{
	//Open Log file
	HANDLE hFile = CreateFile(pszFileName, 
			GENERIC_READ | GENERIC_WRITE, 
			0, 
			NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL );

	//A file was opened?
	if( INVALID_HANDLE_VALUE == hFile )
	{
		// init ini file
		char szFile[256];
		unicode_to_ansi(pszFileName, wcslen(pszFileName), szFile, 256);
		CIniFile::Create(szFile);

		// Name, Value, [Section], file
		CIniFile::SetValue("cmd1", "localhost", INI_SECTION_PING, szFile);
		CIniFile::SetValue("cmd2", "192.168.1.1", INI_SECTION_PING, szFile);
		CIniFile::SetValue("cmd3", "192.168.1.100", INI_SECTION_PING, szFile);

		CIniFile::SetValue("cmd3", "-c localhost -i 1", INI_SECTION_CLIENT, szFile);
		CIniFile::SetValue("cmd2", "-c 192.168.0.1 -t 20 -i 1", INI_SECTION_CLIENT, szFile);
		CIniFile::SetValue("cmd1", "-c 192.168.0.1 -u -i 1", INI_SECTION_CLIENT, szFile);

		CIniFile::SetValue("cmd1", "-s", INI_SECTION_SERVER, szFile);
		CIniFile::SetValue("cmd2", "-s -u",INI_SECTION_SERVER, szFile);

		CIniFile::Sort(szFile, false);
		return;
	}

	//If already exists
	if( ERROR_ALREADY_EXISTS == GetLastError() )
	{
		CloseHandle( hFile );
		return;
	}
}