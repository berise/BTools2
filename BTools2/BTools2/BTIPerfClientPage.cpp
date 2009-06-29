#include "stdafx.h"
#include "BTIPerfClientPage.h"
#include "ScreenLib.h"
#include "Util.h"



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
		nInserted = m_lbResult.AddString(_T("Delete client instance"));
		m_lbResult.SetCurSel(nInserted );
		delete m_pIPerfClient;
		m_pIPerfClient = NULL;
	}


	GetDlgItem(IDC_RUN_CLIENT).SetWindowText(_T("Start"));
	GetDlgItem(IDC_RUN_CLIENT).EnableWindow(TRUE);
	
}


void CIPerfClientPage::CallbackBW(double fBW)
{
	CString temp;
	temp.Format(L"Bandwidth : %l", fBW);
	//m_lbResult.AddString(temp);
}


//


BOOL CIPerfClientPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
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




	return TRUE; // set focus to default control
}

void CIPerfClientPage::OnSize(UINT state, CSize Size)
{
		// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CScreenLib::DockControl(m_hWnd, IDC_STATIC_COMMANDS, CScreenLib::dtTop);
	//CScreenLib::DockControl(m_hWnd, IDC_COMMAND_LIST, CScreenLib::dtTop);

	CScreenLib::OptimizeWidth(m_hWnd, 6, 
		IDC_STATIC_COMMANDS,
		IDC_CB_COMMANDS,
		IDC_RESULT_LIST,
		IDC_STATIC_PLACEHOLDER);

	//VerticalSpace(m_hWnd, IDC_STATIC_COMMANDS, IDC_COMMAND_LIST, 3);
	//VerticalSpace(m_hWnd, IDC_COMMAND_LIST, IDC_COMMAND_EDIT, 3);

	// 최상단 정렬
	VerticalSpace(m_hWnd, IDC_STATIC_COMMANDS, IDC_STATIC_COMMANDS, -5);
	VerticalSpace(m_hWnd, IDC_STATIC_COMMANDS, IDC_CB_COMMANDS, 3);
	VerticalSpace(m_hWnd, IDC_CB_COMMANDS, IDC_STATIC_OPT_INT, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_OPT_INT, IDC_STATIC_OPT_DUR, 3);
	//VerticalSpace(m_hWnd, IDC_STATIC_OPT_INT, IDC_STATIC_OPT_DUR, 3);
	VerticalSpace(m_hWnd, IDC_CB_COMMANDS, IDC_COMBO_INTERVAL, 3);
	VerticalSpace(m_hWnd, IDC_COMBO_INTERVAL, IDC_COMBO_DURATION, 3);
	VerticalSpace(m_hWnd, IDC_COMBO_INTERVAL, IDC_COMBO_SOCKET_TYPE, 3);

	VerticalSpace(m_hWnd, IDC_COMBO_DURATION, IDC_RESULT_LIST, 3);
	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_CLIENT_IP, 3);
	VerticalSpace(m_hWnd, IDC_CLIENT_IP, IDC_STATIC_PLACEHOLDER, 3);


	// IDC_COMMAND_LIST 기준으로 오른쪽에 Add정렬
	CScreenLib::AlignControls(m_hWnd, 
		CScreenLib::atRight, 
		4, 
		IDC_CB_COMMANDS,		// right edge point
		IDC_RUN_CLIENT,
		IDC_COMBO_INTERVAL,
		IDC_COMBO_DURATION,
		IDC_COMBO_SOCKET_TYPE);

	//CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_COMBO_INTERVAL, IDC_COMBO_DURATION);

	// TIPS for ScreenLib
	// Affected Ctrl의 개수를 잘못 사용할 경우, 발견하기 어렵다 -_-;
	// IDC_COMMAND_LIST을 기준으로 COMMAND_EDIT을 왼쪽에 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, 
		IDC_CB_COMMANDS, 
		IDC_COMMAND_EDIT, 
		IDC_CLIENT_IP);


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

	