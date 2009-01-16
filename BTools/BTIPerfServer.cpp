// BTIPerfServer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTIPerfServer.h"

#include "Util.h"
#include "LIstener.hpp"

#include "3rd/ScreenLib.h"


/* -------------------------------------------------------------------
 * Blocks the thread until a quit thread signal is sent
 * ------------------------------------------------------------------- */
void waitUntilQuit( void ) {
#ifdef HAVE_THREAD
    // signal handlers send quit signal on ^C and kill
    gQuit_cond.Lock();
    my_signal( SIGTERM, sig_quit );
    my_signal( SIGINT,  sig_quit );

#ifdef HAVE_USLEEP

    // this sleep is a hack to get around an apparent bug? in IRIX
    // where pthread_cancel doesn't work unless the thread
    // starts up before the gQuit_cand.Wait() call below.
    // A better solution is to just use sigwait here, but
    // then I have to emulate that for Windows...
    usleep( 10 );
#endif

    // wait for quit signal
    gQuit_cond.Wait();
    gQuit_cond.Unlock();
#endif
} // end waitUntilQuit



DWORD WINAPI StartServerThread(LPVOID lpParameter )

{
	Listener *l=(Listener*)lpParameter;

	l->DeleteSelfAfterRun();
	l->Start();

	// 현재 쓰레드 구현으로 되어 있지 않으므로 사용 불가.
	//PPC의 쓰레드 제어를 통해 서버 쓰레드를 종료시켜야 함.
	/*
	if ( l->m_pExtSettings->mThreads == 0 ) {
		l->SetDaemon();
		
		// the listener keeps going; we terminate on user input only
		waitUntilQuit();
#ifdef HAVE_THREAD
		if ( Thread::NumUserThreads() > 0 ) {
			printf( wait_server_threads );
			fflush( 0 );
		}
#endif
	}
	*/
	
	

	l->m_pReporter->ClientFinished();

	return 0;
}

// BTIPerfServer 대화 상자입니다.

IMPLEMENT_DYNAMIC(BTIPerfServer, CPropertyViewPage)

BTIPerfServer::BTIPerfServer(CWnd* pParent /*=NULL*/)
	: CPropertyViewPage(BTIPerfServer::IDD, 0, pParent)
{

	m_pIPerfServer=0;
	m_bServerStarted=FALSE;
	theListener = NULL;	
	
	m_fStatistics = NULL;
	//{{AFX_DATA_INIT(CIperfDlg)
	//m_csHostName = _T("");
//	m_uiInterval = 0.0;
//	m_uiTotalTime = 0;
	// Add Preference items : logdir
	// m_csReportFile.Format("%s\\iperf_stats.txt", logdir);
	//m_csReportFile = _T("\\My Documents\\iperf_stats.txt");
	//m_csErrorFile = _T("\\My Documents\\iperf_err.txt");
	m_csReportFile = _T("\\My Documents\\btools_stats.txt");
	m_csErrorFile = _T("\\My Documents\\btools_error.txt");
}

BTIPerfServer::~BTIPerfServer()
{
}

void BTIPerfServer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyViewPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMAND_LIST, m_lbCommand);
	DDX_Control(pDX, IDC_RESULT_LIST, m_lbResult);
}


BEGIN_MESSAGE_MAP(BTIPerfServer, CPropertyViewPage)
	ON_BN_CLICKED(IDC_RUN_SERVER, &BTIPerfServer::OnBnClickedRunServer)
	ON_LBN_SELCHANGE(IDC_COMMAND_LIST, &BTIPerfServer::OnLbnSelchangeCommandList)
	ON_BN_CLICKED(IDC_ADD_COMMAND, &BTIPerfServer::OnBnClickedAddCommand)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// BTIPerfServer 메시지 처리기입니다.

BOOL BTIPerfServer::OnInitDialog()
{
	CPropertyViewPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_lbCommand.AddString(_T("-s"));
	m_lbCommand.AddString(_T("-s -i 1"));
	m_lbCommand.AddString(_T("-s -u"));
	m_lbCommand.AddString(_T("-s -u -i 1"));

	m_lbCommand.SetCurSel(0);	// default set


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// virtual function in Reporter
void BTIPerfServer::PrintBuffer(char *buffer,char *speed)
{
//	m_csReport.AddString((const unsigned short *)buffer);
	int nInserted = m_lbResult.AddString(ansi_to_unicode(buffer));

	m_lbResult.SetCurSel(nInserted );

	/*if (m_fStatistics !=NULL)
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
*/
}

void BTIPerfServer::ClientFinished()
{	
	m_lbResult.AddString(_T("Server finished"));

	m_bServerStarted=FALSE;
	GetDlgItem(IDC_RUN_SERVER)->SetWindowText(_T("Start"));

	fclose(m_fStatistics);
	m_fStatistics=NULL;

	fclose(winCEStderr);
	winCEStderr=NULL;


	DELETE_PTR(m_pextSettings->mFileName);					// -F
    DELETE_PTR(m_pextSettings->mHost);                   // -c
    DELETE_PTR(m_pextSettings->mLocalhost);              // -B
    DELETE_PTR(m_pextSettings->mOutputFileName);         // -o
	
	DELETE_PTR(m_pextSettings);

	DELETE_PTR(m_piperf_setting);
}


BOOL BTIPerfServer::ParseCommandLine(Settings *pSetting, CString &szCmd)
{
	char **pargv = new char*[25]; // allocate big size
	int curPos = 0, count = 0;
	CString resToken;

	pargv[count] = new char[10];
	strcpy(pargv[count], "btiperf"); // program name
	count++;

	do
	{
		resToken= szCmd.Tokenize(L" ",curPos);
		
		pargv[count] = new char [resToken.GetLength()+1];
		unicode_to_ansi(resToken.GetBuffer(), pargv[count]);
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



void BTIPerfServer::OnBnClickedRunServer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// start up client or server (listener)
	TCHAR hostname[32], ip[48];

	GetLocalIP(hostname, ip);

	GetDlgItem(IDC_SERVER_IP)->SetWindowText(ip);


	if (!m_bServerStarted)
	{		
		// ListBox에서 선택된 명령행을 가져옮
		CString szCmd;

		int nSelected = m_lbCommand.GetCurSel();
		if(nSelected < 0) // nothing selected
			return;	// do nothing


		m_lbCommand.GetText(nSelected, szCmd);		

		m_pextSettings = new ext_Settings();

		// ListBox의 인자를 
		m_piperf_setting = new Settings(m_pextSettings);
		ParseCommandLine(m_piperf_setting, szCmd);


		theListener = new Listener( m_pextSettings, this );
		m_ThreadHandle = CreateThread(NULL,NULL,StartServerThread, theListener,0,NULL);

		if(m_ThreadHandle != NULL)
			m_bServerStarted = TRUE;
		else	
		{	// error
			theListener->m_pReporter->ClientFinished();
			return;
		}

		GetDlgItem(IDC_RUN_SERVER)->SetWindowText(_T("Stop"));
	}
	else
	{
		theListener->Sig_Interupt(0);
		theListener->Stop();

		//DWORD dwExitCode;
		//TerminateThread(m_ThreadHandle, dwExitCode);

		m_bServerStarted=FALSE;
		GetDlgItem(IDC_RUN_SERVER)->SetWindowText(_T("Start &Server"));
	}
}

void BTIPerfServer::OnLbnSelchangeCommandList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szCmd;

	int nSelected = m_lbCommand.GetCurSel();
	m_lbCommand.GetText(nSelected, szCmd);

	GetDlgItem(IDC_COMMAND_EDIT)->SetWindowText(szCmd);
}

void BTIPerfServer::OnBnClickedAddCommand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szCmd;

	GetDlgItem(IDC_COMMAND_EDIT)->GetWindowText(szCmd);

	m_lbCommand.AddString(szCmd);
}

void BTIPerfServer::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CScreenLib::DockControl(m_hWnd, IDC_COMMAND_LIST, CScreenLib::dtTop);

	CScreenLib::OptimizeWidth(m_hWnd, 2, IDC_COMMAND_LIST, IDC_RESULT_LIST);

	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_COMMAND_LIST, IDC_ADD_COMMAND);

	// IDC_COMMAND_LIST을 기준으로 COMMAND_EDIT을 왼쪽에 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_COMMAND_LIST, IDC_COMMAND_EDIT);

	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_COMMAND_LIST, IDC_RUN_SERVER);

}
