// BTPingDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTPingDialog.h"
#include "PingThread.h"
#include "util.h"	//	log


// BTPingDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(BTPingDialog, CPropertyViewPage)

BTPingDialog::BTPingDialog(CWnd* pParent /*=NULL*/)
	: CPropertyViewPage(BTPingDialog::IDD, 0, pParent)
{
	// ping options
	opt.m_bResolveAddressesToHostnames = FALSE;
	opt.m_bPingTillStopped = FALSE;
	opt.m_nRequestsToSend = -1; // infinite loop
	opt.m_nTTL = 30;
	opt.m_wDataRequestSize = 32;
	opt.m_dwTimeout = 5000;
	opt.m_bUseRawSockets = FALSE;
	opt.m_nTOS = 0;
	opt.m_bDontFragment  = FALSE;

	is_host_alive = false;



}

BTPingDialog::~BTPingDialog()
{
}

void BTPingDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyViewPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ping_result);
	DDX_Control(pDX, IDC_COMBO1, m_ping_host);
}


BEGIN_MESSAGE_MAP(BTPingDialog, CPropertyViewPage)
	ON_BN_CLICKED(IDC_DO_PING, &BTPingDialog::OnBnClickedDoPing)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// BTPingDialog 메시지 처리기입니다.

BOOL BTPingDialog::OnInitDialog()
{
	CPropertyViewPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ping_thread = new CPingThread;

	// this might be enough
	//m_ping_host.ModifyStyle(0, CBS_AUTOHSCROLL);	
	m_ping_host.AddString(_T("203.242.63.135"));
	m_ping_host.AddString(_T("192.168.1.1"));

	m_ping_host.AddString(_T("localhost"));


	m_ping_host.SetCurSel(2);

	// LB style 변경.
	DWORD dwStyle = m_ping_result.GetStyle();
	//m_ping_result.ModifyStyle(0, dwStyle | LB_SETHORIZONTALEXTENT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BTPingDialog::OnDestroy()
{
	CPropertyViewPage::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(ping_thread)
		WaitForSingleObject(ping_thread->thread_handle, 1000);

	if(ping_thread != NULL)
	{
		delete ping_thread;
		ping_thread = NULL;
	}
}



/* CString to LPTSTR
CString to LPTSTR

CString str = _T("new text");

  LVITEM item = {0};
 
  item.mask = LVIF_TEXT;
  item.iItem = 1;
  item.pszText = (LPTSTR)(LPCTSTR) str; // 잘못된 사용!
  // 올바른 사용.
  // item.pszText = str.GetBuffer();
  // 또는
  // _tcscpy_s(item.pszText, MAX_PATH, str.GetBuffer(str.GetLength()));
  ListView_SetItem ( &item );
  str.ReleaseBuffer();

참조 :
http://eroom.korea.com/post/board.aspx?bid=kmh_19505&mode=read&view=board&pid=189103&page=1
  */

void BTPingDialog::OnBnClickedDoPing()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(ping_thread->is_running() == FALSE)
	{

	CString szServer;
	//TCHAR msg[256];

	// Combobox manipulation, CBS_AUTOHSCROLL must be in style
	int nSel = m_ping_host.GetCurSel();

	if(nSel == -1) // nothing selected
	{
		// get user input in edit control
		m_ping_host.GetWindowText(szServer.GetBuffer(128), 128);//LOWORD(nCount)+1);//

		// szServer의 길이에 따라 처리 필요
		if(szServer.GetLength() < 0) // 호스트 주소가 없는 경우
		{
			return;
		}
		/*
		DWORD nCount = m_ping_host.GetEditSel();

		if(LOWORD(nCount) > 0)  현재 커서의 위치 (0 ... ^    \0)
		*/
	}
	else // get selected ListBox string
	{
		m_ping_host.GetLBText(nSel, szServer);
	}

	// szServer를 다시 ComboBox에 넣고 Ping 시작
	m_ping_host.AddString(szServer);

	opt.m_sHostToResolve = szServer;
	ping_thread->run(this);	

	GetDlgItem(IDC_DO_PING)->SetWindowText(_T("Stop"));

	}
	else
	{
		ping_thread->stop();

		GetDlgItem(IDC_DO_PING)->SetWindowText(_T("&Ping"));
		
	}
}



void BTPingDialog::OnSize(UINT nType, int cx, int cy)
{
	CPropertyViewPage::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect r;
	GetClientRect(r);
	m_ping_result.MoveWindow(0, r.Height()/4, r.Width(), (r.Height()/4)*3);
}
