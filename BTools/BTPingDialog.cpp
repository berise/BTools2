// BTPingDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTPingDialog.h"
#include "PingThread.h"
#include "util.h"	//	log

#include "3rd/ScreenLib.h"

#define IDC_OSCOPECTRL WM_USER+999


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
	DDX_Control(pDX, IDC_RESULT_LIST, m_ping_result);
	DDX_Control(pDX, IDC_HOST_COMBO, m_cbHosts);
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
	//m_cbHosts.ModifyStyle(0, CBS_AUTOHSCROLL);	
	m_cbHosts.AddString(_T("localhost"));
	m_cbHosts.AddString(_T("203.242.63.135"));	
	m_cbHosts.AddString(_T("192.168.1.1"));
	m_cbHosts.AddString(_T("knu.ac.kr"));
	m_cbHosts.AddString(_T("snu.ac.kr"));
	m_cbHosts.AddString(_T("pnu.ac.kr"));

	m_cbHosts.SetCurSel(0);

	

	//  최초 rect가 너무 작은 경우 오류 발생(OScope의 ratio 계산, points의 new double에서 발생 ※※)
	// 임시로 크기를 가져옴 (올ㅁ?)
	CRect rect;
	GetDlgItem(IDC_CUSTOM1)->GetClientRect(rect);
	//ScreenToClient(rect);
	
	m_OScopeCtrl.Create(WS_CHILD | WS_VISIBLE |  WS_BORDER , rect, this);


	// customize the control
  m_OScopeCtrl.SetRange(0.0, 10.0, 1) ;
  m_OScopeCtrl.SetYUnits(L"ms") ;
  m_OScopeCtrl.SetXUnits(L"Period (1 sec)") ;
  m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
  m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
  m_OScopeCtrl.SetPlotColor(RGB(255, 255, 255)) ;


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
	int nSel = m_cbHosts.GetCurSel();

	if(nSel == -1) // nothing selected
	{
		// get user input in edit control
		m_cbHosts.GetWindowText(szServer.GetBuffer(128), 128);//LOWORD(nCount)+1);//

		// szServer의 길이에 따라 처리 필요
		if(szServer.GetLength() < 0) // 호스트 주소가 없는 경우
		{
			return;
		}
		/*
		DWORD nCount = m_cbHosts.GetEditSel();

		if(LOWORD(nCount) > 0)  현재 커서의 위치 (0 ... ^    \0)
		*/
	}
	else // get selected ListBox string
	{
		m_cbHosts.GetLBText(nSel, szServer);
	}

	// szServer를 다시 ComboBox에 넣고 Ping 시작
	m_cbHosts.AddString(szServer);

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
	__super::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.  
	CScreenLib::OptimizeWidth(m_hWnd, 3, IDC_HOST_COMBO, IDC_RESULT_LIST, IDC_CUSTOM1);

	// 오른쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 핑 버튼을 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_HOST_COMBO, IDC_DO_PING);

	// 왼쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 호스트 텍스트를 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 3, IDC_HOST_COMBO, 
		IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// 왼쪽 상단의 기준인 호스트 텍스트에 따라 LISt와 OSCOPECTRL을 정렬(왼쪽)
	//CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// OSCOPECTRL은 나머지 영역을 채운다
	CScreenLib::OptimizeHeight(m_hWnd, IDC_CUSTOM1);

	// IDC_OSCOPECTRL은 Control이 아니다. 고로 CScreenLib에서 오류가 발생. 별도로 처리함.
	if(m_OScopeCtrl.GetSafeHwnd() != NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Landscape)
		{
			//  가로 모드가 될 경우, RESULT_LIST의 높이가 달라지면.. 
			// 다시 Portrait가 될 경우 어떻게 처리하나?? 따라서, 그대로 커멘트 처리
			//CScreenLib::OptimizeHeight(m_hWnd, IDC_RESULT_LIST);

			GetDlgItem(IDC_RESULT_LIST)->GetWindowRect(rect);
			GetDlgItem(IDC_RESULT_LIST)->ShowWindow(SW_HIDE);
			
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);
		}
		else
		{
			GetDlgItem(IDC_RESULT_LIST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CUSTOM1)->GetWindowRect(rect);
			
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);
		}

	}
}