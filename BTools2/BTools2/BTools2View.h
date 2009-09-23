// BTools2View.h : interface of the CBTools2View class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resourceppc.h"
#include "OScopeCtrl.h"
#include "ScreenLib.h"
#include "BTPingPage.h"
#include "BTIPerfClientPage.h"
#include "BTIPerfServerPage.h"

class CBTools2View : 
	public CPropertySheetImpl<CBTools2View>
{
public:
	DECLARE_WND_CLASS(NULL)
	BOOL PreTranslateMessage(MSG* pMsg);

public:
	CBTPingPage m_Page1;
	CIPerfClientPage m_Page2;
	CIPerfServerPage m_Page3;

	TCHAR gszIniFile[MAX_PATH];
	//
	
	CBTools2View();

	BEGIN_MSG_MAP(CBTools2View)		
		MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOKCancel)
        COMMAND_ID_HANDLER(IDCANCEL, OnOKCancel)
		COMMAND_ID_HANDLER_EX(ID_MENU_ABOUT, OnMenuAbout)
		COMMAND_ID_HANDLER_EX(ID_MENU_PINGTAB, OnMenuPingtab)
		COMMAND_ID_HANDLER_EX(ID_MENU_CLIENTTAB, OnMenuClienttab)
		COMMAND_ID_HANDLER_EX(ID_MENU_SERVERTAB, OnMenuServertab)
		COMMAND_ID_HANDLER_EX(ID_MENU_EXIT, OnMenuExit)
		CHAIN_MSG_MAP(CPropertySheetImpl<CBTools2View>)
	END_MSG_MAP()


	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//


	//int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	//LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnMenuAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnMenuPingtab(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnMenuClienttab(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnMenuServertab(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	
	LRESULT OnMenuExit(WORD wNotifyCode, WORD wID, HWND hWndCtl);


	//
	void OnSetDefaultIni(TCHAR *);
};

