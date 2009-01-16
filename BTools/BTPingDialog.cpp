// BTPingDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTPingDialog.h"
#include "PingThread.h"
#include "util.h"	//	log

#include "3rd/ScreenLib.h"

#define IDC_OSCOPECTRL WM_USER+999


// BTPingDialog ��ȭ �����Դϴ�.

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


// BTPingDialog �޽��� ó�����Դϴ�.

BOOL BTPingDialog::OnInitDialog()
{
	CPropertyViewPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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

	

	//  ���� rect�� �ʹ� ���� ��� ���� �߻�(OScope�� ratio ���, points�� new double���� �߻� �ء�)
	// �ӽ÷� ũ�⸦ ������ (�ä�?)
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


	// LB style ����.
	DWORD dwStyle = m_ping_result.GetStyle();
	//m_ping_result.ModifyStyle(0, dwStyle | LB_SETHORIZONTALEXTENT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void BTPingDialog::OnDestroy()
{
	CPropertyViewPage::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
  item.pszText = (LPTSTR)(LPCTSTR) str; // �߸��� ���!
  // �ùٸ� ���.
  // item.pszText = str.GetBuffer();
  // �Ǵ�
  // _tcscpy_s(item.pszText, MAX_PATH, str.GetBuffer(str.GetLength()));
  ListView_SetItem ( &item );
  str.ReleaseBuffer();

���� :
http://eroom.korea.com/post/board.aspx?bid=kmh_19505&mode=read&view=board&pid=189103&page=1
  */

void BTPingDialog::OnBnClickedDoPing()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

		// szServer�� ���̿� ���� ó�� �ʿ�
		if(szServer.GetLength() < 0) // ȣ��Ʈ �ּҰ� ���� ���
		{
			return;
		}
		/*
		DWORD nCount = m_cbHosts.GetEditSel();

		if(LOWORD(nCount) > 0)  ���� Ŀ���� ��ġ (0 ... ^    \0)
		*/
	}
	else // get selected ListBox string
	{
		m_cbHosts.GetLBText(nSel, szServer);
	}

	// szServer�� �ٽ� ComboBox�� �ְ� Ping ����
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.  
	CScreenLib::OptimizeWidth(m_hWnd, 3, IDC_HOST_COMBO, IDC_RESULT_LIST, IDC_CUSTOM1);

	// ������ ������ IDC_HOST_COMBO(�ּ�â)�� �������� �� ��ư�� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_HOST_COMBO, IDC_DO_PING);

	// ���� ������ IDC_HOST_COMBO(�ּ�â)�� �������� ȣ��Ʈ �ؽ�Ʈ�� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 3, IDC_HOST_COMBO, 
		IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// ���� ����� ������ ȣ��Ʈ �ؽ�Ʈ�� ���� LISt�� OSCOPECTRL�� ����(����)
	//CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// OSCOPECTRL�� ������ ������ ä���
	CScreenLib::OptimizeHeight(m_hWnd, IDC_CUSTOM1);

	// IDC_OSCOPECTRL�� Control�� �ƴϴ�. ��� CScreenLib���� ������ �߻�. ������ ó����.
	if(m_OScopeCtrl.GetSafeHwnd() != NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Landscape)
		{
			//  ���� ��尡 �� ���, RESULT_LIST�� ���̰� �޶�����.. 
			// �ٽ� Portrait�� �� ��� ��� ó���ϳ�?? ����, �״�� Ŀ��Ʈ ó��
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