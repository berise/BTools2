#include "stdafx.h"
#include "IniFile.h"
#include "BTPing.h"
#include "BTPingPage.h"

#include "Util.h"
#include "BTools2View.h"

#include <vector>



// ping thread
DWORD WINAPI PingThread(LPVOID lpParameter )
{
	CBTPingPage *pThis=(CBTPingPage *)lpParameter;

	// Ping을 호출하기 전에 m_szTarget을 설정해야 함


	// BTPing은 Thread안에서 생성되어야 한다.
	// ICMP 패킷의 id 필드는 현재 쓰레드(프로세스) ID를 필요하기 때문이다.	
	pThis->m_pPing = new BTPing(pThis);
	pThis->m_pPing->SetDoLookup(true);
	pThis->m_pPing->SetMaxTimeOuts(6);

	//while(pThis->m_bStartThread )
	{
		
		pThis->m_pPing->Ping(pThis->m_szTarget,		// Your target host
							4000,					// Timeout
							pThis->m_nDataSize,		// ICMP size
							1000,					// Interval
							pThis->m_nSendCount == -1 ? 99999 : pThis->m_nSendCount);  // how many icmp will you send?
	}
	

	// Thread 종료시
	EnterCriticalSection(&pThis->m_cs);
	pThis->GetDlgItem(IDC_DO_PING).SetWindowText(_T("&Ping"));
	int nIns = pThis->m_lbPingResult.AddString(_T("---- Done ----"));
	pThis->m_lbPingResult.SetCurSel(nIns);
	LeaveCriticalSection(&pThis->m_cs);
	pThis->m_bStartThread = FALSE;

	// zap away
	DELETE_PTR(pThis->m_pPing);

	return 0;
}


CBTPingPage::CBTPingPage()
{
	// ICON
	m_psp.dwFlags |= PSP_USEICONID;
	//m_psp.dwFlags |= PSP_USEHICON;
	//m_psp.pszIcon = MAKEINTRESOURCE(IDI_PING);
	m_psp.pszIcon = MAKEINTRESOURCE(IDI_PING);//m_pView->m_ImageList16x16.GetIcon(0);
	//HICON a = m_pView->m_ImageList16x16.G.GetIcon(0);	
	//m_psp.pszIcon = m_pView->m_ImageList16x16.G.GetIcon(0);
	m_psp.hInstance = _Module.GetResourceInstance();
	//m_psp.hInstance = ModuleHelper::GetResourceInstance();//_Module.GetResourceInstance();
}


BOOL CBTPingPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	// MENUBAR
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = GetParent();
	mbi.nToolBarId = IDR_MENU1;
	mbi.hInstRes   = _Module.GetResourceInstance();
	mbi.dwFlags    = SHCMBF_HMENU;
	SHCreateMenuBar(&mbi);


	GetDlgItem(IDC_STATIC_PLACEHOLDER).ShowWindow(SW_HIDE);
	CRect rect;
	::GetWindowRect(GetDlgItem(IDC_STATIC_PLACEHOLDER), rect);
	//ScreenToClient(rect);
	
	// create the control
	HWND ret = m_OScopeCtrl.Create(m_hWnd, rect, NULL) ; 
	
	if(ret != NULL)
	{
		//m_OScopeCtrl.InvalidateCtrl();
	// customize the control
	m_OScopeCtrl.SetRange(0.0, 100.0, 1) ;
	m_OScopeCtrl.SetYUnits(L"Latency(ms)") ; // 
	m_OScopeCtrl.SetYUnits(L"ms") ; //  너무 길지 않게 조절 필요.
	m_OScopeCtrl.SetXUnits(L"Period(ms)") ; // 너무 길지 않도록 설정 필요. 필요시 InvalidateCtrl내부 수정 필요
	m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
	m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
	m_OScopeCtrl.SetPlotColor(RGB(255, 0, 255)) ;

	}
	
	m_bStartThread = FALSE;
	m_hThread = NULL;

	m_pPing = NULL;

	 // 컨트롤 설정
	DoDataExchange(FALSE);
	CComboBox cbInt = GetDlgItem(IDC_CB_DATA_SIZE);
	cbInt.AddString(L"4");
	cbInt.AddString(L"8");
	cbInt.AddString(L"16");
	cbInt.AddString(L"32");
	cbInt.AddString(L"64");
	cbInt.AddString(L"128");
	cbInt.AddString(L"256");
	cbInt.SetCurSel(3);

	cbInt = GetDlgItem(IDC_CB_SEND_COUNT);
	cbInt.AddString(L"-1");
	cbInt.AddString(L"4");
	cbInt.AddString(L"999"); // boy loves this number.
	cbInt.SetCurSel(1);

	m_nDataSize = 32; // 4, 8, 16, 32, 64, 128, 256 (kB)
	m_nSendCount = 4; // -1, 4, 9, 999
	

	
	//m_cbHosts.AddString(L"kldp.org");
	//m_cbHosts.AddString(L"localhost");	
	//m_cbHosts.SetCurSel(1);

	// ini 파일에서 읽은 데이터를 설정
	char szFile[256];		
	unicode_to_ansi(m_pView->gszIniFile, wcslen(m_pView->gszIniFile), szFile, 256);

	vector<CIniFile::Record> s  = CIniFile::GetSection(INI_SECTION_PING, szFile);
	vector<CIniFile::Record>::iterator i;
	for( i = s.begin(); i != s.end(); i++)
	{
		TCHAR wszBuf[256];
		string value = CIniFile::GetValue( (*i).Key, INI_SECTION_PING, szFile);

		ansi_to_unicode(value.c_str(), value.length(), wszBuf, 256);
		m_cbHosts.AddString(wszBuf);
	}

	m_cbHosts.SetCurSel(0);


	// prepare to log
	TCHAR *log_dir = L"\\My Documents\\btools_log";
	TCHAR *log_file_postfix = L"ping.txt";
	// log
	TCHAR *pLogFile = SetupLog(log_dir, log_file_postfix);
	_tcscpy(m_szLogFile, pLogFile);

	// Initialize the critical section
    InitializeCriticalSection(&m_cs);

	return TRUE; // set focus to default control
}




// OnPing
LRESULT CBTPingPage::OnPing(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	int nIns = 0 ;

	if(m_bStartThread == FALSE)
	{
		DoDataExchange(TRUE);


			// Combobox manipulation, CBS_AUTOHSCROLL must be in style
		int nSel = m_cbHosts.GetCurSel();
		if(nSel == -1) // nothing selected
		{
			// get user input in edit control
			m_cbHosts.GetWindowText(m_szTarget.GetBuffer(128), 128);//LOWORD(nCount)+1);//

			// szTarget의 길이에 따라 처리 필요
			if(m_szTarget.GetLength() < 0) // 호스트 주소가 없는 경우
			{
				return 0;
			}
			/*	DWORD nCount = m_cbHosts.GetEditSel();
			if(LOWORD(nCount) > 0)  현재 커서의 위치 (0 ... ^    \0)			*/
		}
		else // get selected ListBox string
		{
			m_cbHosts.GetLBText(nSel, m_szTarget);
		}

		m_cbHosts.InsertString(0, m_szTarget);

		int nItem = m_cbHosts.GetCount();
		if(nItem > MAX_ITEM) m_cbHosts.DeleteString(MAX_ITEM);
		

		// update ini
		OnUpdateIni();


		CString szMsg;
		szMsg.Format(L"%d, %d", m_nDataSize, m_nSendCount);
		//AtlMessageBox(NULL, szMsg.GetBuffer(), L"debug");

		m_hThread = CreateThread(NULL,NULL, PingThread,this,0,NULL);
		

		EnterCriticalSection(&m_cs);

		m_bStartThread = TRUE;
		GetDlgItem(IDC_DO_PING).SetWindowText(_T("Stop"));
		nIns = m_lbPingResult.AddString(_T("---- Start ----"));
		m_lbPingResult.SetCurSel(nIns);
		LeaveCriticalSection(&m_cs);
	}
	else
	{
		EnterCriticalSection(&m_cs);
		m_bStartThread = FALSE;
		//if(m_pPing != NULL)m_pPing->IsAborted();

		GetDlgItem(IDC_DO_PING).SetWindowText(_T("Ping"));
		nIns = m_lbPingResult.AddString(_T("---- Stop ----"));
		m_lbPingResult.SetCurSel(nIns);

		LeaveCriticalSection(&m_cs);

	}



	return 0;
}

LRESULT CBTPingPage::OnDestroy(void)
{
	DeleteCriticalSection(&m_cs);

	//You should call SetMsgHandled(FALSE) or set bHandled = FALSE for the main window of your application
	m_bStartThread = FALSE;
	//IsAborted();
	//if(m_pPing != NULL) m_pPing->IsAborted();

	return 0;
}




void CBTPingPage::OnSize(UINT state, CSize Size)
{
	CScreenLib::DockControl(m_hWnd, IDC_HOST, CScreenLib::dtTop);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.  
	// 2009/9/29 IDC_STATIC_VISUAL 제거
	CScreenLib::OptimizeWidth(m_hWnd, 4,
		IDC_HOST,
		IDC_HOST_COMBO, 
		IDC_RESULT_LIST, 
		IDC_STATIC_PLACEHOLDER
		);

	// 2009/9/23 relocate IDC_PING
	// Reason : Pushing ping button will close dialog by pressing OK
	// 
	// In order to show ping button on the top of IDC_HOST, IDC_HOST must be shrinked to width of IDC_PING
	CRect r1, r2;
	GetDlgItem(IDC_PING).GetClientRect(r1);
	GetDlgItem(IDC_HOST).GetClientRect(r2);
	GetDlgItem(IDC_HOST).MoveWindow(r2.left, r2.top, r2.Width() - r1.Width(), r2.Height());


	// 최상단 정렬
	VerticalSpace(m_hWnd, IDC_HOST, IDC_HOST_COMBO, 3);
	VerticalSpace(m_hWnd, IDC_HOST_COMBO, IDC_STATIC_DATA_SIZE, 3);
	VerticalSpace(m_hWnd, IDC_HOST_COMBO, IDC_STATIC_SEND_COUNT, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_DATA_SIZE, IDC_CB_DATA_SIZE, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_SEND_COUNT, IDC_CB_SEND_COUNT, 3);

	VerticalSpace(m_hWnd, IDC_CB_SEND_COUNT, IDC_RESULT_LIST, 3);



	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_STATIC_PLACEHOLDER, 4);
	//VerticalSpace(m_hWnd, IDC_STATIC_VISUAL, IDC_STATIC_PLACEHOLDER, 3);


	// IDC_STATIC_DATA_SIZE에 Ping 버튼 상 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_CB_DATA_SIZE, IDC_PING);

	// 오른쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 핑 버튼을 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_HOST_COMBO, 
					IDC_PING);

	// 왼쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 호스트 텍스트를 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 3, IDC_HOST_COMBO, 
					IDC_HOST, IDC_STATIC_DATA_SIZE, IDC_CB_DATA_SIZE);



	// 왼쪽 상단의 기준인 호스트 텍스트에 따라 LISt와 OSCOPECTRL을 정렬(왼쪽)
	//CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// OSCOPECTRL은 나머지 영역을 채운다
	//CScreenLib::OptimizeHeight(m_hWnd, IDC_STATIC_VISUAL);
	//CScreenLib::DockControl(m_hWnd, IDC_STATIC_VISUAL, CScreenLib::dtBottom);
	
	
	// IDC_STATIC_VISUAL아래쪽의 남은 공간의 크기를 계산하여 OScopeCtrl을 가득 채운다.
	CRect r, cr, vr;
	//::GetClientRect(::GetDlgItem(m_hWnd, IDC_STATIC_VISUAL), vr);
	GetDlgItem(IDC_RESULT_LIST).GetWindowRect(vr);
	ScreenToClient(vr);
	GetWindowRect(cr);
	ScreenToClient(cr);

	GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(r);
	ScreenToClient(r);

	GetDlgItem(IDC_STATIC_PLACEHOLDER).MoveWindow(r.left, r.top, r.Width(), cr.Height() - vr.bottom - 6); // padding(-6)
		

	

	// IDC_OSCOPECTRL은 Control이 아니다. 고로 CScreenLib에서 오류가 발생. 
	// 별도의 PlaceHolder를 통해 처리함.
	if(m_OScopeCtrl.m_hWnd != NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Landscape)
		{
			//  가로 모드가 될 경우, RESULT_LIST의 높이가 달라지면.. 
			// 다시 Portrait가 될 경우 어떻게 처리하나?? 따라서, 그대로 커멘트 처리
			CScreenLib::OptimizeHeight(m_hWnd, IDC_RESULT_LIST);

			GetDlgItem(IDC_RESULT_LIST).GetWindowRect(rect);
			//GetDlgItem(IDC_RESULT_LIST)->ShowWindow(SW_HIDE);
			
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);
		}
		else
		{
			GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(rect);//IDC_STATIC_PLACEHOLDER
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);//
		}
	}

	return ;
}



//
void CBTPingPage::Log(TCHAR *wszLog)
{
	EnterCriticalSection(&m_cs);
	if(wszLog != NULL) 
	{
		
		int nIns = m_lbPingResult.AddString(wszLog);
		WriteLog(m_szLogFile, wszLog);
		m_lbPingResult.SetCurSel(nIns);		
	}
	LeaveCriticalSection(&m_cs);
}

void CBTPingPage::SetView(CBTools2View *pView)
{
	m_pView = pView;
}

// Listbox 내용을(m_cbHost) 을 파일로 덤프
void CBTPingPage::OnUpdateIni()
{
	char szFile[256];
	char szKey[100];
	char szCmd[256];
	

	unicode_to_ansi(m_pView->gszIniFile, wcslen(m_pView->gszIniFile), szFile, 256);

	CIniFile::DeleteSection(INI_SECTION_PING, szFile);
	CIniFile::AddSection(INI_SECTION_PING, szFile);

	int nCount = m_cbHosts.GetCount();

	TCHAR wszCmd[256];
	for(int i = 0; i < nCount; i++)
	{
		m_cbHosts.GetLBText(i, wszCmd);
		unicode_to_ansi(wszCmd, wcslen(wszCmd), szCmd, 256);
		sprintf(szKey, "cmd%d", i+1);

		//
		CIniFile::SetValue(szKey, szCmd, INI_SECTION_PING, szFile);
	}
	CIniFile::Sort(szFile, FALSE);

}