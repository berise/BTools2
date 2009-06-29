#ifndef __BTPAGES_INCLUDED__
#define __BTPAGES_INCLUDED__

//#include "XGroupBox.h"
#include "Client.hpp"
#include "Server.hpp"
#include "Reporter.h"
class Listener;




class CBTPingPage: public CPropertyPage<IDD_PING_PAGE>,
			public CWinDataExchange<CBTPingPage>
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
	END_DDX_MAP()

public:
	void OnSize(UINT state, CSize Size);
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnDestroy(void);


	LRESULT OnPing(WORD wNotifyCode, WORD wID, HWND hWndCtl);


public:
	void Log(TCHAR *wszLog);

public:
	//
	//
	BOOL m_bStartThread;
	COScopeCtrl m_OScopeCtrl;
	HANDLE m_hThread;

	CComboBox m_cbHosts;
	//
	//CXGroupBox m_sHost;


	LRESULT OnMenuAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl);
};

#endif