#include "stdafx.h"
#include "BTIPerfClientPage.h"
#include "Util.h"



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



	return TRUE; // set focus to default control
}

void CIPerfClientPage::OnSize(UINT state, CSize Size)
{

	return ;
}

