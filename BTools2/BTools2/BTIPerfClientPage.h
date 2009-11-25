#ifndef __BIPERF_CLIENT_INCLUDED__
#define __BIPERF_CLIENT_INCLUDED__

//#include "XGroupBox.h"
#include "stdafx.h"
#include "Client.hpp"
#include "Reporter.h"
#include "OScopeCtrl.h"

// forward declaration
class Listener;
class CBTools2View;




class CIPerfClientPage: public CPropertyPage<IDD_IPERF_CLIENT_PAGE>,
		public CWinDataExchange<CIPerfClientPage>,
		CReporter
{
public:
	CIPerfClientPage();
	WTL_DLG_NOTITLE

	BEGIN_MSG_MAP(CIPerfClientPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_RUN_CLIENT, OnRunClient)
	END_MSG_MAP()


	BEGIN_DDX_MAP(CBTPingPage)
		DDX_CONTROL_HANDLE(IDC_CB_COMMAND, m_cbCommand)
		DDX_CONTROL_HANDLE(IDC_RESULT_LIST, m_lbResult)
		DDX_INT(IDC_COMBO_INTERVAL, m_nInterval)
		DDX_INT(IDC_COMBO_DURATION, m_nDuration)
		DDX_TEXT(IDC_COMBO_SOCKET_TYPE, m_szSocketType)
	END_DDX_MAP()


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
	
	LRESULT OnRunClient(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	void OnUpdateIni();
	void SetView(CBTools2View *pView);

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


	/// parent window
	CBTools2View *m_pView;


	// DDX variables
	CListBox m_lbResult;
	CComboBox m_cbCommand;
	int m_nInterval, m_nDuration;
	CString m_szSocketType;
	CString m_szCommand;

};


#endif