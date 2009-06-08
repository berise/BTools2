#ifndef __BIPERF_SERVER_INCLUDED__
#define __BIPERF_SERVER_INCLUDED__

//#include "XGroupBox.h"

#include "Server.hpp"
#include "Reporter.h"
class Listener;


class CIPerfServerPage: public CPropertyPage<IDD_IPERF_SERVER_PAGE>

{
public:
	WTL_DLG_NOTITLE


	BEGIN_MSG_MAP(CPingPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
		
	END_MSG_MAP()

	void OnSize(UINT state, CSize Size);

	// windows messages
public:
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);

	// operations
public:
	BOOL ParseCommandLine(Settings *pSetting, CString &szCmd);

public:
	// variables
	ext_Settings* m_pextSettings;
	Settings *m_piperf_setting;


	HANDLE m_ThreadHandle;
	Listener *theListener;
	Server *m_pIPerfServer;

	FILE *m_fStatistics;
	CString	m_csReportFile;
	CString	m_csErrorFile;

	BOOL m_bServerStarted;


	
};

#endif