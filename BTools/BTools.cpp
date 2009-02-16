// BTools.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "BTools.h"
#include "MainFrm.h"

#include "BToolsDoc.h"
#include "BToolsView.h"
#include "afxwin.h"

#include "About.h"	// about box as you know
#include "Util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CBToolsApp

BEGIN_MESSAGE_MAP(CBToolsApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CBToolsApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()



// CBToolsApp ����
CBToolsApp::CBToolsApp()
	: CWinApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CBToolsApp ��ü�Դϴ�.
CBToolsApp theApp;

// CBToolsApp �ʱ�ȭ

BOOL CBToolsApp::InitInstance()
{
	// Program Expiration routines
	CTime expirationTime(2009,	// year
		3,					// month
		1,					// date
		23,					// hour 24
		59,					// minute
		59);				// second
	CTime currentTime = CTime::GetCurrentTime();

    CTimeSpan leftTime = expirationTime - currentTime;   

     if(leftTime.GetTotalSeconds() > 0)
    {
		 CString msg;
		 msg = L"�� ���α׷��� ���� ���� �����Դϴ�.\n";
		 msg += L"This is a date limited version of BTools.\n";
		 msg += expirationTime.Format(L"Expiration date :\n %Y-%m-%d  %H:%M\n\n");
		 msg += leftTime.Format(L"%D day(s) and\n %H:%M:%S left" );
         //AfxMessageBox(msg);
		 if(leftTime.GetDays() < 7)
		 {
			 msg = leftTime.Format(L"This software will expire after %D day(s) %H Hour(s) %M Minute(s)");
			 AfxMessageBox(msg);
		 }
    }
    else
    {
		/*CString msg("This is a limited version of BTools.\n"
				"This is submitted for the Windows Mobile Contest only.\n"
				"Please contact the author for more use.\n\n"
				"Thank you for your interest");
				*/
		CString msg("�� ���α׷��� ���� ���� �����Դϴ�.\n"
				"This is a limited version of BTools.\n"
				"It is submitted for the Windows Mobile Contest,\n"
				"co-held by SK & Microsoft.\n"
				"Please contact the author for more use.\nberise@gmail.com\n"
				"Thank you for your interest\n");

		msg += expirationTime.Format(L"Expiration date :\n %Y-%m-%d  %H:%M\n\n");
		msg += leftTime.Format(L"%D day(s) and\n %H:%M:%S left" );
        //msg.Format(L"This software is submitted for the Windows Mobile Contest");
		AfxMessageBox(msg);
		return FALSE;
	 }

	//CAboutDlg aboutDlg;	aboutDlg.DoModal();
    //



    // CAPEDIT �� SIPPREF�� ���� ��� Windows Mobile ���� ��Ʈ���� �ʱ�ȭ�ϱ� ����
    // ���� ���α׷��� �ʱ�ȭ�ϴ� ���� SHInitExtraControls�� �� �� ȣ���ؾ� �մϴ�.
    SHInitExtraControls();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBToolsDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CBToolsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}





// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CBToolsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
