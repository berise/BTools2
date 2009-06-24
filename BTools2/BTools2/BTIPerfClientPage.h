#ifndef __BIPERF_CLIENT_INCLUDED__
#define __BIPERF_CLIENT_INCLUDED__

//#include "XGroupBox.h"
#include "stdafx.h"
#include "Client.hpp"
#include "Reporter.h"
#include "OScopeCtrl.h"
class Listener;



class CIPerfClientPage: public CPropertyPage<IDD_IPERF_CLIENT_PAGE>,
	CReporter
{
public:
	WTL_DLG_NOTITLE

	BEGIN_MSG_MAP(CIPerfClientPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

public: // windows messages
	void OnSize(UINT state, CSize Size);
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);


	// operations
public:
	BOOL ParseCommandLine(Settings *pSetting, CString &szCmd);

	/*!
		\brief CReporter의 가상 함수
	*/
	virtual void  PrintBuffer(char *buffer,char *speed);
	virtual void  ClientFinished();
	virtual void CallbackBW(double fBW);


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

	COScopeCtrl m_OScopeCtrl;


	CListBox m_lbResult, m_lbCommands;

};



#endif