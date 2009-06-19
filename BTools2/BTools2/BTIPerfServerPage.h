#ifndef __BIPERF_SERVER_INCLUDED__
#define __BIPERF_SERVER_INCLUDED__

//#include "XGroupBox.h"

#include "Server.hpp"
#include "Reporter.h"
class Listener;


class CIPerfServerPage: public CPropertyPage<IDD_IPERF_SERVER_PAGE>,
		public CWinDataExchange<CIPerfServerPage>,
		CReporter
{
public:
	WTL_DLG_NOTITLE


		// WTL message maps
	BEGIN_MSG_MAP(CPingPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)		
	END_MSG_MAP()

	BEGIN_DDX_MAP(CIPerfServerPage)
		DDX_CONTROL_HANDLE(IDC_COMMAND_LIST, m_lbCommand)
		DDX_CONTROL_HANDLE(IDC_RESULT_LIST, m_lbResult)
	END_DDX_MAP()


	void OnDataExchangeError(UINT nCtrlID, BOOL bSave){

        CString strMsg;

        strMsg.Format(_T("컨트롤(ID:%u) 과의 데이터 교환에 실패. "), nCtrlID);

        MessageBox(strMsg, _T("DDX에러"), MB_ICONWARNING);

 

        ::SetFocus(GetDlgItem(nCtrlID));

    }
	// windows messages
public:
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