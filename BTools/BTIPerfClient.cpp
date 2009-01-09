// BTIPerfClient.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTIPerfClient.h"

#include "Util.h" // unicode ansi


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
	, m_csHostName(_T(""))
{
	m_pIPerfClient=0;
	m_bClientStarted=FALSE;	
	
	m_fStatistics = NULL;
	//{{AFX_DATA_INIT(CIperfDlg)
	m_csHostName = _T("");
//	m_uiInterval = 0.0;
//	m_uiTotalTime = 0;
	// Add Preference items : logdir
	// m_csReportFile.Format("%s\\iperf_stats.txt", logdir);
	//m_csReportFile = _T("\\My Documents\\iperf_stats.txt");
	//m_csErrorFile = _T("\\My Documents\\iperf_err.txt");
	m_csReportFile = _T("\\My Documents\\btools_log\\btools_stats.txt");
	m_csErrorFile = _T("\\My Documents\\btools_log\\btools_error.txt");
}

BTIPerfClient::~BTIPerfClient()
{
	
}

void BTIPerfClient::DoDataExchange(CDataExchange* pDX)
{
	CPropertyViewPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOST, m_csHostName);
	DDX_Control(pDX, IDC_COMMAND_LIST, m_lbCommand);
	DDX_Control(pDX, IDC_RESULT_LIST, m_lbResult);
}


BEGIN_MESSAGE_MAP(BTIPerfClient, CPropertyViewPage)
	ON_BN_CLICKED(IDC_RUN_CLIENT, &BTIPerfClient::OnBnClickedRunClient)
	ON_BN_CLICKED(IDC_ADD_COMMAND, &BTIPerfClient::OnBnClickedAddCommand)
	ON_LBN_SELCHANGE(IDC_COMMAND_LIST, &BTIPerfClient::OnLbnSelchangeCommandList)
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
	m_lbCommand.AddString(_T("-c 192.168.0.13 -t 5 -i 1"));
	m_lbCommand.AddString(_T("-c 192.168.0.13 -t 3"));
	m_lbCommand.AddString(_T("-c 192.168.0.13 -t 5 -u"));
	m_lbCommand.AddString(_T("-c 192.168.0.13 -t 5 -u -b 2m"));
	m_lbCommand.AddString(_T("-c 192.168.0.13 -t 5 -u -b 4m"));
	m_lbCommand.AddString(_T("-c 192.168.0.13 -t 5 -u -b 8m"));	

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
	}
	else
	{
		m_pIPerfClient->Sig_Interupt(0);
		m_bClientStarted=FALSE;
		GetDlgItem(IDC_RUN_CLIENT)->SetWindowText(_T("Start Client"));
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
