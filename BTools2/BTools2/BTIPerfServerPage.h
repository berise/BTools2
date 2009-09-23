#ifndef __BIPERF_SERVER_INCLUDED__
#define __BIPERF_SERVER_INCLUDED__

#include "Server.hpp"
#include "Reporter.h"

class Listener;
class CBTools2View;


class CIPerfServerPage: public CPropertyPage<IDD_IPERF_SERVER_PAGE>,
		public CWinDataExchange<CIPerfServerPage>,
		CReporter
{
public:
	WTL_DLG_NOTITLE

		// WTL message maps
	BEGIN_MSG_MAP(CIPerfServerPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)		
		COMMAND_HANDLER_EX(IDC_RUN_SERVER, BN_CLICKED, OnRunServerBnClicked)
		COMMAND_CODE_HANDLER_EX(LBN_SELCHANGE, OnLbnSelChange)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CIPerfServerPage)
		DDX_CONTROL_HANDLE(IDC_COMMAND_LIST, m_lbCommand)
		DDX_CONTROL_HANDLE(IDC_RESULT_LIST, m_lbResult)
	END_DDX_MAP()


	void OnDataExchangeError(UINT nCtrlID, BOOL bSave);
	// windows messages
public:
	void OnSize(UINT state, CSize Size);


	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);

	LRESULT OnRunServerBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnLbnSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl);

	// operations
public:
	BOOL ParseCommandLine(Settings *pSetting, CString &szCmd);

	/*!
		\brief CReporter의 가상 함수
	*/
	virtual void  PrintBuffer(char *buffer,char *speed);
	virtual void  ClientFinished();
	virtual void CallbackBW(double fBW);

	void OnUpdateIni();
	void SetView(CBTools2View *pView);

public:
	//
	CBTools2View *m_pView;

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

	CListBox m_lbCommand;
	CListBox m_lbResult;

	
	
};

#endif