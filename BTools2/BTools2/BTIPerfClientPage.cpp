#include "stdafx.h"
#include "BTIPerfClientPage.h"
#include "ScreenLib.h"
#include "Util.h"




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
	int nInserted = m_lbResult.AddString(_T("iperf client finished"));
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


//


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
		m_OScopeCtrl.SetYUnits(L"kBytes/s") ; //  너무 길지 않게 조절 필요.
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

	m_cbCommand.AddString(L"-c 127.0.0.1");
	m_cbCommand.AddString(L"-c 192.168.0.1");
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
		cr.Height() - vr.bottom);


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



LRESULT CIPerfClientPage::OnRunClient(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	DoDataExchange(TRUE);
	CString szMsg;
	szMsg.Format(L" -i %d -t %d", m_nInterval, m_nDuration);

	CString szCommand;
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
		/*
		DWORD nCount = m_cbCommands.GetEditSel();

		if(LOWORD(nCount) > 0)  현재 커서의 위치 (0 ... ^    \0)
		*/
	}
	else // get selected ListBox string
	{
		m_cbCommand.GetLBText(nSel, szCmd);
	}
	_tcscat(szCmd, szMsg);

	//AtlMessageBox(NULL, szCmd);


		// szServer를 다시 ComboBox에 넣고 Ping 시작
	m_cbCommand.AddString(szCmd);

	m_pSettings = new ext_Settings();

	// ListBox의 인자를 
	m_piperf_setting = new Settings(m_pSettings);
	ParseCommandLine(m_piperf_setting, CString(szCmd));
	


	if (!m_bClientStarted)
	{
		m_lbResult.AddString(_T("iperf client started..."));


		m_bClientStarted=TRUE;
		GetDlgItem(IDC_RUN_CLIENT).SetWindowText(_T("Stop"));

		m_fStatistics = fopen(GetCharBuffer(m_csReportFile),"a+");
		winCEStderr = fopen(GetCharBuffer(m_csErrorFile),"a+");

		if (m_pIPerfClient!=0)
			delete m_pIPerfClient;

		//Settings *pSettings = new Settings(m_pSettings);

		//m_pSettings->mHost=GetCharBuffer(m_csHostName);
		//m_pSettings->mInterval=m_uiInterval;

		/*if (m_uiTotalTime>1)
			m_pSettings->mAmount=-m_uiTotalTime*100;
			*/

		m_pIPerfClient = new Client(m_pSettings, true, NULL, this);

		m_pIPerfClient->InitiateServer();

		CreateThread(NULL,NULL, ClientThread,m_pIPerfClient,0,NULL);

		//GetDlgItem(IDC_CLIENT_STATUS)->SetWindowText(_T("실행 중(Running)..."));

		//		StartThread(m_pIPerfClient);
		// Run the test
		// TODO : Start 버튼 이후, 초기화 및 client 실행시 까지 Button을 disable
		// Stop시도 마찬가지 처리 (client/server 둘다)
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
