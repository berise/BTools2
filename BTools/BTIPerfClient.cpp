// BTIPerfClient.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTIPerfClient.h"

#include "Util.h" // unicode ansi

#include "3rd/ScreenLib.h"


DWORD WINAPI StartThread(LPVOID lpParameter )

{
	Client *c=(Client *)lpParameter;

	c->Start();
	c->m_pReporter->ClientFinished();

	return 0;
}



// BTIPerfClient 대화 상자입니다.

IMPLEMENT_DYNAMIC(BTIPerfClient, CPropertyViewPage)

BTIPerfClient::BTIPerfClient(CWnd* pParent /*=NULL*/)
	: CPropertyViewPage(BTIPerfClient::IDD, 0, pParent)
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
}

BTIPerfClient::~BTIPerfClient()
{
	
}

void BTIPerfClient::DoDataExchange(CDataExchange* pDX)
{
	CPropertyViewPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMAND_LIST, m_lbCommand);
	DDX_Control(pDX, IDC_RESULT_LIST, m_lbResult);
	DDX_Control(pDX, IDC_STATIC_COMMANDS, m_sCommands);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, m_sOutput);
}


BEGIN_MESSAGE_MAP(BTIPerfClient, CPropertyViewPage)
	ON_BN_CLICKED(IDC_RUN_CLIENT, &BTIPerfClient::OnBnClickedRunClient)
	ON_BN_CLICKED(IDC_ADD_COMMAND, &BTIPerfClient::OnBnClickedAddCommand)
	ON_LBN_SELCHANGE(IDC_COMMAND_LIST, &BTIPerfClient::OnLbnSelchangeCommandList)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// BTIPerfClient 메시지 처리기입니다.

BOOL BTIPerfClient::OnInitDialog()
{
	CPropertyViewPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//TCHAR CommandLines[256];
	/*
		[Client] tcp://host
		[Client] tcp://host,w16k, w32k
		[Client] udp://host
		[Client] udp://host,w16k, w32k
		[Server] tcp://host
		[Server] udp://host

	*/

	m_lbCommand.AddString(_T("-c localhost -t 5 -i 1"));
	m_lbCommand.AddString(_T("-c 192.168.0.1 -i 1"));
	m_lbCommand.AddString(_T("-c 125.152.1.1 -i 1"));
	m_lbCommand.AddString(_T("-c 192.168.0.1 -i 1 -u"));

	m_lbCommand.SetCurSel(0);	// default selection

	// init Groupboxes
	m_sCommands.EnableWindow(TRUE, TRUE);
	
	m_sCommands.SetWindowText(L"IPerf Client Commands", FALSE)
				//.SetIcon(IDI_ABOUT1, 16, FALSE)
				.SetTextColor(RGB(0,0,255), FALSE)
				.SetBorderColor(RGB(255,0,0), FALSE)
				.SetBold(TRUE, FALSE)
				// .SetFont(_T("Comic Sans MS"), 10, FALSE)
				.SetAlignment(CXGroupBox::left, FALSE)
				.SetControlStyle(CXGroupBox::header, FALSE);

	m_sOutput.EnableWindow(TRUE, TRUE);
	
	m_sOutput.SetWindowText(L"IPerf Client Output", FALSE)	//SetIcon(IDI_UAC_SHIELD, 32, FALSE)
				.SetTextColor(RGB(0,0,255), FALSE)
				.SetBorderColor(RGB(255,0,0), FALSE)
				.SetBold(TRUE, FALSE)
				// .SetFont(_T("Comic Sans MS"), 10, FALSE)
				.SetAlignment(CXGroupBox::left, FALSE)
				.SetControlStyle(CXGroupBox::header, FALSE);


	// OScopeCtrl
	// PlaceHodler for OScopeCtrl must be set to invisible.
	CRect rect;
	GetDlgItem(IDC_STATIC_GRAPH)->GetClientRect(rect);
	m_OScopeCtrl.Create(WS_CHILD | WS_VISIBLE, rect, this);


	// customize the OScope Control
	m_OScopeCtrl.SetRange(0.0, 100.0, 1) ;
	m_OScopeCtrl.SetYUnits(L"Bandwidth(kB/s)") ;
	m_OScopeCtrl.SetXUnits(L"Time(sec)") ;
	m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
	m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
	m_OScopeCtrl.SetPlotColor(RGB(255, 0, 255)) ;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void BTIPerfClient::PrintBuffer(char *buffer,char *speed)
{
//	m_csReport.AddString((const unsigned short *)buffer);
	int nInserted = m_lbResult.AddString(ansi_to_unicode(buffer));

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

void BTIPerfClient::ClientFinished()
{
	m_lbResult.AddString(_T("iperf client finished"));
	//GetDlgItem(IDC_CLIENT_STATUS)->SetWindowText(_T("Client Finished"));
	m_bClientStarted=FALSE;
	GetDlgItem(IDC_RUN_CLIENT)->SetWindowText(_T("Start"));
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

}

// char *transfer, char *bandwidth
// 위 두가지를 숫자로 전환하면 종료 (David의 string-to-number를 사용)
// 포멧도 필요 m_Setting->mFormat  그래프 레이블 변경에 필요함.
void BTIPerfClient::CallbackBW(double fBW)
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


/* @note dynamic allocation confusion
char** array;
int clients;

// Read 'clients' from console:
cin >> clients;

// Allocate array of slots:
array = new char* [clients];

// Read all names from console:
char strName [128];

for (int i=0; i<clients; i++) {
  cin.getline (strName, sizeof (strName), '\n'); // Read it
  array [i] = _strdup (strName); // Get a duplicate and add it
}

// Now dump them back:
for (int n=0; n<clients; n++) {
  cout << array [i] << endl;
  free (array [i]); // De-allocate every ptr from '_strdup()'
}

// Release array itself:
delete [] array;

*/
BOOL BTIPerfClient::ParseCommandLine(Settings *pSetting, CString &szCmd)
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

// TODO:현재 CommandList에 선택된 명령을 옵션으로 변경하여 실행한다.
void BTIPerfClient::OnBnClickedRunClient()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// ListBox에서 선택된 명령행을 가져옮
	CString szCmd;

	int nSelected = m_lbCommand.GetCurSel();
	m_lbCommand.GetText(nSelected, szCmd);
	
	

	m_pSettings = new ext_Settings();

	// ListBox의 인자를 
	m_piperf_setting = new Settings(m_pSettings);
	ParseCommandLine(m_piperf_setting, szCmd);
	


	if (!m_bClientStarted)
	{
		m_lbResult.AddString(_T("iperf client started..."));


		m_bClientStarted=TRUE;
		GetDlgItem(IDC_RUN_CLIENT)->SetWindowText(_T("Stop"));
		UpdateData(TRUE);


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

		CreateThread(NULL,NULL,StartThread,m_pIPerfClient,0,NULL);

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
		GetDlgItem(IDC_RUN_CLIENT)->SetWindowText(_T("Start"));
	}
	
}

void BTIPerfClient::OnBnClickedAddCommand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szCmd;

	GetDlgItem(IDC_COMMAND_EDIT)->GetWindowText(szCmd);

	m_lbCommand.AddString(szCmd);


}

void BTIPerfClient::OnLbnSelchangeCommandList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szCmd;

	int nSelected = m_lbCommand.GetCurSel();
	m_lbCommand.GetText(nSelected, szCmd);


	GetDlgItem(IDC_COMMAND_EDIT)->SetWindowText(szCmd);


}

void BTIPerfClient::OnSize(UINT nType, int cx, int cy)
{
	CPropertyViewPage::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CScreenLib::DockControl(m_hWnd, IDC_STATIC_COMMANDS, CScreenLib::dtTop);
	//CScreenLib::DockControl(m_hWnd, IDC_COMMAND_LIST, CScreenLib::dtTop);

	CScreenLib::OptimizeWidth(m_hWnd, 5, 
		IDC_STATIC_COMMANDS,
		IDC_COMMAND_LIST,
		IDC_STATIC_OUTPUT,
		IDC_RESULT_LIST,
		IDC_STATIC_GRAPH);

	VerticalSpace(m_hWnd, IDC_STATIC_COMMANDS, IDC_COMMAND_LIST, 8);
	VerticalSpace(m_hWnd, IDC_COMMAND_LIST, IDC_COMMAND_EDIT, 6);

	VerticalSpace(m_hWnd, IDC_COMMAND_EDIT, IDC_STATIC_OUTPUT, 12);
	VerticalSpace(m_hWnd, IDC_STATIC_OUTPUT, IDC_RESULT_LIST, 8);
	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_CLIENT_IP, 8);
	VerticalSpace(m_hWnd, IDC_CLIENT_IP, IDC_STATIC_GRAPH, 8);



	// IDC_COMMAND_LIST 기준으로 오른쪽에 Add정렬
	CScreenLib::AlignControls(m_hWnd, 
		CScreenLib::atRight, 
		2, 
		IDC_COMMAND_LIST,		// right edge point
		IDC_ADD_COMMAND,
		IDC_RUN_CLIENT);

	// TIPS for ScreenLib
	// Affected Ctrl의 개수를 잘못 사용할 경우, 발견하기 어렵다 -_-;
	// IDC_COMMAND_LIST을 기준으로 COMMAND_EDIT을 왼쪽에 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, 
		IDC_COMMAND_LIST, 
		IDC_COMMAND_EDIT, 
		IDC_CLIENT_IP);

	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_COMMAND_EDIT, IDC_ADD_COMMAND);

	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_CLIENT_IP, IDC_RUN_CLIENT);
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_COMMAND_LIST, IDC_RUN_CLIENT);


	if(m_OScopeCtrl.GetSafeHwnd() != NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Portrait)
		{
			CScreenLib::OptimizeHeight(m_hWnd, IDC_STATIC_GRAPH);

			CRect rect;
			GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(rect);
			ScreenToClient(rect);
			m_OScopeCtrl.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);//MoveWindow(rect);
		}
		else if (DRA::GetDisplayMode() == DRA::Landscape)
		{
			;	// not yet implemented
		}
	}	
}
