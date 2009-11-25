#include "stdafx.h"
#include "IniFile.h"
#include "BTIPerfClientPage.h"
#include "ScreenLib.h"
#include "Util.h"
#include "BTools2View.h"




DWORD WINAPI ClientThread(LPVOID lpParameter )
{
	Client *c=(Client *)lpParameter;

	c->Start();
	c->m_pReporter->ClientFinished();

	return 0;
}



//
//

//
// CReporter
// virtual function in Reporter
void CIPerfClientPage::PrintBuffer(char *buffer,char *speed)
{
	//	m_csReport.AddString((const unsigned short *)buffer);
	TCHAR wszBuf[256];
	ansi_to_unicode(buffer, strlen(buffer), wszBuf, 256);

	int nInserted = m_lbResult.AddString(wszBuf);
	m_lbResult.SetCurSel(nInserted );

	if (m_fStatistics !=NULL)
	{
		fprintf(m_fStatistics,"%s",buffer);
		fflush(m_fStatistics);
	}
	if (speed!=0)
	{
		CString ss(speed);
		//GetDlgItem(IDC_CLIENT_VALUES)->SetWindowText(ss);
	}
//	GetGlobalLog()->LogEvent(EVENT_DEBUG,buffer);


}

void CIPerfClientPage::ClientFinished()
{
	int nInserted = m_lbResult.AddString(_T("---- Done ----"));
	m_lbResult.SetCurSel(nInserted );
	//GetDlgItem(IDC_CLIENT_STATUS)->SetWindowText(_T("Client Finished"));
	m_bClientStarted=FALSE;
	
	fclose(m_fStatistics);
	m_fStatistics=NULL;
	fclose(winCEStderr);
	winCEStderr=NULL;


	DELETE_PTR(m_pSettings->mFileName);					// -F
    DELETE_PTR(m_pSettings->mHost);                   // -c
    DELETE_PTR(m_pSettings->mLocalhost);              // -B
    DELETE_PTR(m_pSettings->mOutputFileName);         // -o
	
	DELETE_PTR(m_pSettings);

	DELETE_PTR(m_piperf_setting);

	if (m_pIPerfClient!=NULL)
	{
		//nInserted = m_lbResult.AddString(_T("Delete client instance"));
		//m_lbResult.SetCurSel(nInserted );
		DELETE_PTR(m_pIPerfClient);
	}


	GetDlgItem(IDC_RUN_CLIENT).SetWindowText(_T("Start"));
	GetDlgItem(IDC_RUN_CLIENT).EnableWindow(TRUE);
	
}


void CIPerfClientPage::CallbackBW(double fBW)
{
	CString temp;
	temp.Format(L"Bandwidth : %lf", fBW/1024);
	//m_lbResult.AddString(temp);

	// visualization
	double nRandom;
	// generate a random number between 
	nRandom = -5.0 + 1000.0*rand()/(double)RAND_MAX;
	//m_OScopeCtrl.AppendPoint(nRandom);

		// append the new value to the plot
	m_OScopeCtrl.AppendPoint(fBW/1024);

}


/// To put icon on CPropertyPage
CIPerfClientPage::CIPerfClientPage()
{
	m_psp.dwFlags |= PSP_USEICONID;
	m_psp.pszIcon = MAKEINTRESOURCE(IDI_IPERF_CLIENT);
	m_psp.hInstance = ModuleHelper::GetResourceInstance();
}

BOOL CIPerfClientPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = GetParent();
	mbi.nToolBarId = IDR_MENU1;
	mbi.hInstRes   = _Module.GetResourceInstance();
	mbi.dwFlags    = SHCMBF_HMENU;
	SHCreateMenuBar(&mbi);


	m_pIPerfClient=0;
	m_bClientStarted=FALSE;	
	
	m_fStatistics = NULL;
	//{{AFX_DATA_INIT(CIperfDlg)
//	m_uiInterval = 0.0;
//	m_uiTotalTime = 0;
	// Add Preference items : logdir
	// m_csReportFile.Format("%s\\iperf_stats.txt", logdir);
	//m_csReportFile = _T("\\My Documents\\iperf_stats.txt");
	//m_csErrorFile = _T("\\My Documents\\iperf_err.txt");

	TCHAR *log_dir = L"\\My Documents\\btools_log";
	TCHAR *log_file_postfix = L"iClient.txt";
	// log
	TCHAR *pLogFile = SetupLog(log_dir, log_file_postfix);

	m_csReportFile = pLogFile;


	TCHAR *error_file_postfix = L"iClient_error.txt";
	pLogFile = SetupLog(log_dir, error_file_postfix);
	m_csErrorFile = pLogFile;


	// initialize GUI parts
		// OScopeCtrl
	// PlaceHodler for OScopeCtrl must be set to invisible.
	CRect rect;
	GetDlgItem(IDC_STATIC_PLACEHOLDER).GetClientRect(rect);
	HWND ret = m_OScopeCtrl.Create(m_hWnd, rect, NULL) ; 
	
	if(ret != NULL)
	{
		//m_OScopeCtrl.InvalidateCtrl();
		// customize the control
		m_OScopeCtrl.SetRange(0.0, 100.0, 1) ;
		//m_OScopeCtrl.SetYUnits(L"Latency(ms)") ; // 
		m_OScopeCtrl.SetYUnits(L"KB/s") ; //  너무 길지 않게 조절 필요.
		m_OScopeCtrl.SetXUnits(L"Time(sec)") ; // 너무 길지 않도록 설정 필요. 필요시 InvalidateCtrl내부 수정 필요
		m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
		m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
		m_OScopeCtrl.SetPlotColor(RGB(255, 0, 255)) ;

	}

	// DDX/DDV
	DoDataExchange(FALSE);
	CComboBox cbInt = GetDlgItem(IDC_COMBO_INTERVAL);
	cbInt.AddString(L"1");
	cbInt.AddString(L"3");
	cbInt.AddString(L"5");
	cbInt.AddString(L"10");
	cbInt.AddString(L"30");
	cbInt.SetCurSel(0);

	CComboBox cbDur = GetDlgItem(IDC_COMBO_DURATION);
	cbDur.AddString(L"5");
	cbDur.AddString(L"10");
	cbDur.AddString(L"20");
	cbDur.AddString(L"30");
	cbDur.AddString(L"50");
	cbDur.AddString(L"999");
	cbDur.SetCurSel(0);

	CComboBox cbSocketType = GetDlgItem(IDC_COMBO_SOCKET_TYPE);
	cbSocketType.AddString(L"TCP");
	cbSocketType.AddString(L"UDP");
	cbSocketType.SetCurSel(0);

	//m_nInterval = 1;	m_nDuration = 5;	m_szSocketType = L"TCP";

	//m_cbCommand.AddString(L"-c 127.0.0.1");
	//m_cbCommand.AddString(L"-c 192.168.0.1");
	//m_cbCommand.SetCurSel(0);

	// ini 파일에서 읽은 데이터를 설정
	char szFile[256];		
	unicode_to_ansi(m_pView->gszIniFile, wcslen(m_pView->gszIniFile), szFile, 256);

	vector<CIniFile::Record> s  = CIniFile::GetSection(INI_SECTION_CLIENT, szFile);
	vector<CIniFile::Record>::iterator i;
	for( i = s.begin(); i != s.end(); i++)
	{
		TCHAR wszBuf[256];
		string value = CIniFile::GetValue( (*i).Key, INI_SECTION_CLIENT, szFile);

		ansi_to_unicode(value.c_str(), value.length(), wszBuf, 256);
		m_cbCommand.AddString(wszBuf);
	}

	m_cbCommand.SetCurSel(0);


	return TRUE; // set focus to default control
}

void CIPerfClientPage::OnSize(UINT state, CSize Size)
{
		// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CScreenLib::DockControl(m_hWnd, IDC_STATIC_COMMANDS, CScreenLib::dtTop);
	
	CScreenLib::OptimizeWidth(m_hWnd, 4, 
		IDC_STATIC_COMMANDS,
		IDC_CB_COMMANDS,
		IDC_RESULT_LIST,
		IDC_STATIC_PLACEHOLDER);


	// 최상단 정렬
	//VerticalSpace(m_hWnd, IDC_STATIC_COMMANDS, IDC_STATIC_COMMANDS, -5);
	VerticalSpace(m_hWnd, IDC_STATIC_COMMANDS, IDC_CB_COMMANDS, 3);
	VerticalSpace(m_hWnd, IDC_CB_COMMANDS, IDC_STATIC_OPT_INT, 3);
	VerticalSpace(m_hWnd, IDC_CB_COMMANDS, IDC_STATIC_OPT_DUR, 3);
	VerticalSpace(m_hWnd, IDC_CB_COMMANDS, IDC_STATIC_SOCKET_TYPE, 3);
	//VerticalSpace(m_hWnd, IDC_STATIC_OPT_INT, IDC_STATIC_OPT_DUR, 3);
	//VerticalSpace(m_hWnd, IDC_STATIC_OPT_INT, IDC_STATIC_OPT_DUR, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_OPT_INT, IDC_COMBO_INTERVAL, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_OPT_INT, IDC_COMBO_DURATION, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_OPT_INT, IDC_COMBO_SOCKET_TYPE, 3);

	VerticalSpace(m_hWnd, IDC_COMBO_DURATION, IDC_RESULT_LIST, 3);
	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_CLIENT_IP, 2);
	VerticalSpace(m_hWnd, IDC_CLIENT_IP, IDC_STATIC_PLACEHOLDER, 2);


	// IDC_COMMAND_LIST 기준으로 오른쪽에 Add정렬
	CScreenLib::AlignControls(m_hWnd, 
		CScreenLib::atRight, 
		2, 
		IDC_CB_COMMANDS,		// right edge point
		IDC_RUN_CLIENT,
		IDC_COMBO_SOCKET_TYPE);

	
	// TIPS for ScreenLib
	// Affected Ctrl의 개수를 잘못 사용할 경우, 발견하기 어렵다 -_-;
	// IDC_COMMAND_LIST을 기준으로 COMMAND_EDIT을 왼쪽에 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, 
		IDC_CB_COMMANDS,
		IDC_STATIC_OPT_INT,
		IDC_CLIENT_IP);

	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_STATIC_OPT_INT, IDC_COMBO_INTERVAL);
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_STATIC_OPT_DUR, IDC_COMBO_DURATION);
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_STATIC_SOCKET_TYPE, IDC_COMBO_SOCKET_TYPE);


	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_CLIENT_IP, IDC_RUN_CLIENT);
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_CB_COMMANDS, IDC_RUN_CLIENT);

	// OSCOPECTRL은 나머지 영역을 채운다
	//CScreenLib::OptimizeWidth(m_hWnd, 1, IDC_STATIC_PLACEHOLDER);
	//CScreenLib::OptimizeHeight(m_hWnd, IDC_STATIC_PLACEHOLDER);

	CRect r, cr, vr;
	//::GetClientRect(::GetDlgItem(m_hWnd, IDC_STATIC_VISUAL), vr);
	GetDlgItem(IDC_CLIENT_IP).GetWindowRect(vr);
	ScreenToClient(vr);
	GetWindowRect(cr);
	ScreenToClient(cr);

	GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(r);
	ScreenToClient(r);

	GetDlgItem(IDC_STATIC_PLACEHOLDER).MoveWindow(r.left, 
		r.top, 
		r.Width(), 
		cr.Height() - vr.bottom-6);


	if(m_OScopeCtrl.m_hWnd!= NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Portrait)
		{
			//CScreenLib::OptimizeHeight(m_hWnd, IDC_STATIC_PLACEHOLDER);

			CRect rect;
			GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(rect);
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);//SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);//
		}
		else if (DRA::GetDisplayMode() == DRA::Landscape)
		{
			;	// not yet implemented
		}
	}	

	return ;
}


BOOL CIPerfClientPage::ParseCommandLine(Settings *pSetting, CString &szCmd)
{
	char **pargv = new char*[25]; // allocate big size
	int curPos = 0, count = 0;
	CString resToken;

	pargv[count] = new char[10];
	strcpy(pargv[count], "btiperf"); // dummy program name
	count++;

	do
	{
		resToken= szCmd.Tokenize(L" ",curPos);
		
		pargv[count] = new char [resToken.GetLength()+1];
		unicode_to_ansi(resToken.GetBuffer(), resToken.GetLength(), pargv[count], resToken.GetLength());
		//pargv[count] =  argv[count];
		//*pargv++ = argv[count];

		count++;
	} while (resToken != _T(""));

	pSetting->ParseCommandLine(count-1, pargv);


	for(int i = 0; i < count; i++)
		delete pargv[i];
	delete [] pargv;


	return 0;
}



// GUI 설정을 가져온 후
// Ini파일 업데이트 후
// iPerf Client를 실행한다.
LRESULT CIPerfClientPage::OnRunClient(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	DoDataExchange(TRUE);

	CString szCmdOption;
	TCHAR szCmd[256];

	// Combobox manipulation, CBS_AUTOHSCROLL must be in style
	int nSel = m_cbCommand.GetCurSel();

	if(nSel == -1) // nothing selected
	{
		// get user input in edit control
		m_cbCommand.GetWindowText(szCmd, 256);//LOWORD(nCount)+1);//

		// szServer의 길이에 따라 처리 필요
		if(_tcslen(szCmd) < 0) // 호스트 주소가 없는 경우
		{
			return 0;
		}		
	}
	else // get selected ListBox string
	{
		m_cbCommand.GetLBText(nSel, szCmd);
	}

	// ComboBox의 명령만 cbCommand에 넣어야 함. 그렇지 않으면 -i -t 옵션이 계속 추가됨
	// szServer를 다시 ComboBox에 넣고 Ping 시작
	m_cbCommand.InsertString(0, szCmd);

	szCmdOption.Format(L" -i %d -t %d", m_nInterval, m_nDuration);

	_tcscat(szCmd, szCmdOption);

	// DEBUG
	//AtlMessageBox(NULL, szCmd);

	int nItem = m_cbCommand.GetCount();
	if(nItem > MAX_ITEM) m_cbCommand.DeleteString(MAX_ITEM);

	OnUpdateIni();


	
	// prepare iPerf client
	m_pSettings = new ext_Settings();

	// ListBox의 인자를 
	m_piperf_setting = new Settings(m_pSettings);
	ParseCommandLine(m_piperf_setting, CString(szCmd));
	


	if (!m_bClientStarted)
	{
		m_lbResult.AddString(_T("---- Start ----"));


		m_bClientStarted=TRUE;
		GetDlgItem(IDC_RUN_CLIENT).SetWindowText(_T("Stop"));

		m_fStatistics = fopen(GetCharBuffer(m_csReportFile),"a+");
		winCEStderr = fopen(GetCharBuffer(m_csErrorFile),"a+");

		if (m_pIPerfClient!=0)
			delete m_pIPerfClient;

	
		m_pIPerfClient = new Client(m_pSettings, true, NULL, this);
		m_pIPerfClient->InitiateServer();

		CreateThread(NULL,NULL, ClientThread,m_pIPerfClient,0,NULL);		
	}
	else
	{
		m_pIPerfClient->Sig_Interupt(0);
		m_bClientStarted=FALSE;
		GetDlgItem(IDC_RUN_CLIENT).EnableWindow(FALSE);
		GetDlgItem(IDC_RUN_CLIENT).SetWindowText(_T("Start"));
	}

	return 0;
}

void CIPerfClientPage::SetView(CBTools2View *pView)
{
	m_pView = pView;
}

// Listbox 내용을(m_cbHost) 을 파일로 덤프
void CIPerfClientPage::OnUpdateIni()
{
	char szFile[256];
	char szKey[100];
	char szCmd[256];
	

	unicode_to_ansi(m_pView->gszIniFile, wcslen(m_pView->gszIniFile), szFile, 256);

	CIniFile::DeleteSection("iPref Client", szFile);
	CIniFile::AddSection("iPref Client", szFile);

	int nCount = m_cbCommand.GetCount();

	TCHAR wszCmd[256];
	for(int i = 0; i < nCount; i++)
	{
		m_cbCommand.GetLBText(i, wszCmd);
		unicode_to_ansi(wszCmd, wcslen(wszCmd), szCmd, 256);
		sprintf(szKey, "cmd%d", i+1);

		//??? 이상하다. char -> string으로 복사가 안 되네.. ptr만 전달되네...
		CIniFile::SetValue(szKey, szCmd, "iPref Client", szFile);
	}
	CIniFile::Sort(szFile, FALSE);

}