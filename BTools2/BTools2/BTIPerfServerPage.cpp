#include "stdafx.h"
#include "BTIPerfServerPage.h"
#include "Util.h"
#include "LIstener.hpp"
#include "ScreenLib.h"


// Server Thread
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

	l->m_pReporter->ClientFinished();

	return 0;
}



///////////
BOOL CIPerfServerPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	// initialize member variables
	m_pIPerfServer=0;
	m_bServerStarted=FALSE;
	theListener = NULL;	
	
	m_fStatistics = NULL;
	
	TCHAR *log_dir = L"\\My Documents\\btools_log";
	TCHAR *log_file_postfix = L"iServer.txt";
	// log
	TCHAR *pLogFile = SetupLog(log_dir, log_file_postfix);

	m_csReportFile = pLogFile;


	TCHAR *error_file_postfix = L"iServer_error.txt";
	pLogFile = SetupLog(log_dir, error_file_postfix);
	m_csErrorFile = pLogFile;


	return TRUE; // set focus to default control
}



void CIPerfServerPage::OnSize(UINT state, CSize Size)
{
	CScreenLib::DockControl(m_hWnd, IDC_STATIC_COMMANDS, CScreenLib::dtTop);

	CScreenLib::OptimizeWidth(m_hWnd, 5, 
		IDC_STATIC_COMMANDS, 
		IDC_COMMAND_LIST, 
		IDC_STATIC_OUTPUT,
		IDC_RESULT_LIST,
		IDC_STATIC_INFO
		);

	// WM6은 -5 설정
	VerticalSpace(m_hWnd, IDC_STATIC_COMMANDS, IDC_COMMAND_LIST, -5);
	VerticalSpace(m_hWnd, IDC_COMMAND_LIST, IDC_COMMAND_EDIT, 3);

	VerticalSpace(m_hWnd, IDC_COMMAND_EDIT, IDC_STATIC_OUTPUT, 6);
	VerticalSpace(m_hWnd, IDC_STATIC_OUTPUT, IDC_RESULT_LIST, 3);	

	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_STATIC_INFO,6);
	VerticalSpace(m_hWnd, IDC_STATIC_INFO, IDC_SERVER_IP, 3);
	


	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_COMMAND_LIST, IDC_ADD_COMMAND);

	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_COMMAND_EDIT, IDC_ADD_COMMAND);

	// IDC_COMMAND_LIST을 기준으로 IDC_SERVER_IP을 왼쪽에 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, IDC_COMMAND_LIST, 
		IDC_COMMAND_EDIT,
		IDC_SERVER_IP);

	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_SERVER_IP, IDC_RUN_SERVER);
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_COMMAND_LIST, IDC_RUN_SERVER);
}

//

BOOL CIPerfServerPage::ParseCommandLine(Settings *pSetting, CString &szCmd)
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



//
// CReporter
// virtual function in Reporter
void CIPerfServerPage::PrintBuffer(char *buffer,char *speed)
{
//	m_csReport.AddString((const unsigned short *)buffer);
	if(buffer != NULL)
	{
		TCHAR wszBuf[256];
		ansi_to_unicode(buffer, strlen(buffer), wszBuf, 256);

		int nInserted = m_lbResult.AddString(wszBuf);
		WriteLog(m_csReportFile.GetBuffer(), wszBuf );

		m_lbResult.SetCurSel(nInserted );
	}

	if(speed != NULL)
	{
		; // DO NOTHING 4 a while
	}

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

void CIPerfServerPage::ClientFinished()
{	
	int nInserted = m_lbResult.AddString(_T("Server finished"));
	m_lbResult.SetCurSel(nInserted );

	m_bServerStarted=FALSE;
	GetDlgItem(IDC_RUN_SERVER).SetWindowText(_T("Start"));
	GetDlgItem(IDC_RUN_SERVER).EnableWindow(TRUE);

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

	if(theListener != NULL)
	{
		nInserted = m_lbResult.AddString(_T("Delete listener instance"));
		m_lbResult.SetCurSel(nInserted );

		delete theListener;
		theListener = NULL;
	}
}

void CIPerfServerPage::CallbackBW(double fBW)
{
	CString temp;
	temp.Format(L"Bandwidth : %l", fBW);
	//m_lbResult.AddString(temp);
}
