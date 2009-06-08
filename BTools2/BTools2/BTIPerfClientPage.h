#ifndef __BIPERF_CLIENT_INCLUDED__
#define __BIPERF_CLIENT_INCLUDED__

//#include "XGroupBox.h"
#include "stdafx.h"
#include "Client.hpp"
#include "Reporter.h"
class Listener;



class CIPerfClientPage: public CPropertyPage<IDD_IPERF_CLIENT_PAGE>
{
public:
	WTL_DLG_NOTITLE

	BEGIN_MSG_MAP(CPingPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

public: // windows messages
	void OnSize(UINT state, CSize Size);
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);


	// operations
public:
	BOOL ParseCommandLine(Settings *pSetting, CString &szCmd);


public:

private:
	// variables
		// Member variables
	FILE *m_fStatistics;

	BOOL m_bClientStarted;
	Client *m_pIPerfClient;

	CString	m_csReportFile;
	CString	m_csErrorFile;

	ext_Settings* m_pSettings;
	Settings *m_piperf_setting;

};



#endif