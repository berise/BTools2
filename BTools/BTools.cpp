// BTools.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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



// CBToolsApp 생성
CBToolsApp::CBToolsApp()
	: CWinApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CBToolsApp 개체입니다.
CBToolsApp theApp;

// CBToolsApp 초기화

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
		 msg = L"이 프로그램은 기한 제한 버전입니다.\n";
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
		CString msg("이 프로그램은 기한 제한 버전입니다.\n"
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



    // CAPEDIT 및 SIPPREF와 같은 모든 Windows Mobile 관련 컨트롤을 초기화하기 위해
    // 응용 프로그램을 초기화하는 동안 SHInitExtraControls를 한 번 호출해야 합니다.
    SHInitExtraControls();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBToolsDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CBToolsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}





// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CBToolsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
