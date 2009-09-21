#ifndef __BTPAGES_INCLUDED__
#define __BTPAGES_INCLUDED__

//#include "XGroupBox.h"
#include "ut_icmp.h"
#include "Client.hpp"
#include "Server.hpp"
#include "Reporter.h"
class Listener;
class BTPing;
class CBTools2View;


class CBTPingPage: public CPropertyPage<IDD_PING_PAGE>,
			public CWinDataExchange<CBTPingPage>, 
			public CUT_ICMP
{
public:
	//WTL_DLG_NOTITLE

	BEGIN_MSG_MAP(CBTPingPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER_EX(IDC_PING, OnPing)
		CHAIN_MSG_MAP(CPropertyPage<IDD_PING_PAGE>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CBTPingPage)
		DDX_CONTROL_HANDLE(IDC_HOST_COMBO, m_cbHosts)
		DDX_CONTROL_HANDLE(IDC_RESULT_LIST, m_lbPingResult)
		DDX_INT(IDC_CB_DATA_SIZE, m_nDataSize)
		DDX_INT(IDC_CB_SEND_COUNT, m_nSendCount)
	END_DDX_MAP()

public:
	void OnSize(UINT state, CSize Size);
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnDestroy(void);


	LRESULT OnPing(WORD wNotifyCode, WORD wID, HWND hWndCtl);

	// UT_ICMP virtual functions
	void OnReceiveICMP();    
    // OnError is a member of the CUT_WSClient class, from which CUT_ICMP inherits...
    int OnError(int error);
	BOOL IsAborted();

	void OnUpdateMRU();


public:
	void Log(TCHAR *wszLog);

public:
	//
	CBTools2View *m_pView;
	//
	BTPing *m_pPing;
	BOOL m_bStartThread;
	COScopeCtrl m_OScopeCtrl;
	HANDLE m_hThread;

	CString m_szTarget;
	TCHAR m_szLogFile[256];

	CRITICAL_SECTION m_cs;

	CComboBox m_cbHosts;
	CListBox	m_lbPingResult;
	
	int m_nDataSize, m_nSendCount;


	LRESULT OnMenuAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl);
};

#endif