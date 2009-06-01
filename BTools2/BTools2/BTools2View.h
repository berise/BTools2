// BTools2View.h : interface of the CBTools2View class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlcrack.h>
#include <atlmisc.h>
#include "resourceppc.h"
#include "OScopeCtrl.h"
#include "ScreenLib.h"


class CPingPage: public CPropertyPage<IDD_PING_PAGE>
{
public:
	//WTL_DLG_NOTITLE

	BEGIN_MSG_MAP(CPingPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDC_PING, OnPing)
	END_MSG_MAP()

public:
	void OnSize(UINT state, CSize Size);
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnPing(WORD wNotifyCode, WORD wID, HWND hWndCtl);


	//
	//
	BOOL m_bStartThread;
	COScopeCtrl m_OScopeCtrl;

};

class CIPerfClientPage: public CPropertyPage<IDD_IPERF_CLIENT_PAGE>
{
	WTL_DLG_NOTITLE



	BEGIN_MSG_MAP(CPingPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

	void OnSize(UINT state, CSize Size);

public:
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);
};


class CIPerfServerPage: public CPropertyPage<IDD_IPERF_SERVER_PAGE>

{
	WTL_DLG_NOTITLE



	BEGIN_MSG_MAP(CPingPage)
		MSG_WM_SIZE(OnSize)
		MSG_WM_INITDIALOG(OnInitDialog)
		
	END_MSG_MAP()

	void OnSize(UINT state, CSize Size);

public:
	BOOL  OnInitDialog(HWND hwndFocus, LPARAM lParam);
	
};

class CBTools2View : 
	public CPropertySheetImpl<CBTools2View>
{
public:
	DECLARE_WND_CLASS(NULL)
	

	BOOL PreTranslateMessage(MSG* pMsg);

public:

	CPingPage m_Page1;
	CIPerfClientPage m_Page2;
	CIPerfServerPage m_Page3;
	

	CBTools2View();

	BEGIN_MSG_MAP(CBTools2View)		
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
	END_MSG_MAP()

	//


	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnClose(void);
	LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
};

