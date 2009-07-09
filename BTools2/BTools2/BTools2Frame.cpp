// BTools2Frame.cpp : implementation of the CBTools2Frame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resourceppc.h"

#include "aboutdlg.h"
#include "BTools2View.h"
#include "BTools2Frame.h"

BOOL CBTools2Frame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CBTools2Frame>::PreTranslateMessage(pMsg))
		return TRUE; 

	return m_view.IsWindow() ? m_view.PreTranslateMessage(pMsg) : FALSE;
}

bool CBTools2Frame::AppHibernate( bool bHibernate)
{
	// Insert your code here or delete member if not relevant
	return bHibernate;
}

bool CBTools2Frame::AppNewInstance( LPCTSTR lpstrCmdLine)
{
	// Insert your code here or delete member if not relevant
	return false;
}

void CBTools2Frame::AppSave()
{
	CAppInfo info;
	// Insert your code here
}

BOOL CBTools2Frame::OnIdle()
{
	if (!m_view.IsWindow() || m_view.SendMessage(PSM_GETCURRENTPAGEHWND) == NULL)
		PostMessage(WM_CLOSE);

	UIUpdateToolBar();
	return FALSE;
}

LRESULT CBTools2Frame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CAppInfo info;	
	//HICON hIcon = (HICON)::LoadImage(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	//SetIcon(hIcon);

	CreateSimpleCEMenuBar();
	UIAddToolBar(m_hWndCECommandBar);

	m_hWndClient = m_view.Create(m_hWnd);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

LRESULT CBTools2Frame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CBTools2Frame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CBTools2Frame::OnAction(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code

	return 0;
}

LRESULT CBTools2Frame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	return 0;
}


LRESULT CBTools2Frame::OnMenuClienttab(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	m_view.SetActivePage(1);

	return 0;
}

LRESULT CBTools2Frame::OnDestroy(void)
{
	//You should call SetMsgHandled(FALSE) or set bHandled = FALSE for the main window of your application
	SetMsgHandled(FALSE);
	//::AtlMessageBox(NULL, L"on destroy");

	return TRUE;
}
