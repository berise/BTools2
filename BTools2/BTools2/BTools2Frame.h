// BTools2Frame.h : interface of the CBTools2Frame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CBTools2Frame : 
	public CFrameWindowImpl<CBTools2Frame>, 
	public CUpdateUI<CBTools2Frame>,
	public CAppWindow<CBTools2Frame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_APP_FRAME_CLASS(NULL, IDR_MAINFRAME, L"Software\\WTL\\BTools2")

	CBTools2View m_view;

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// CAppWindow operations
	bool AppHibernate( bool bHibernate);

	bool AppNewInstance( LPCTSTR lpstrCmdLine);

	void AppSave();

	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CBTools2Frame)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CBTools2Frame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_ACTION, OnAction)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CAppWindow<CBTools2Frame>)
		CHAIN_MSG_MAP(CUpdateUI<CBTools2Frame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CBTools2Frame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnAction(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
