// BTPingDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTPingDialog.h"
#include "PingThread.h"
#include "util.h"	//	log

#include "3rd/ScreenLib.h"
#include "3rd/IniFile.h"


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
	DDX_Control(pDX, IDC_RESULT_LIST, m_lbPingResult);
	DDX_Control(pDX, IDC_HOST_COMBO, m_cbHosts);
	DDX_Control(pDX, IDC_HOST, m_sHost);
	DDX_Control(pDX, IDC_STATIC_VISUAL, m_sVisual);
}


BEGIN_MESSAGE_MAP(BTPingDialog, CPropertyViewPage)
	ON_BN_CLICKED(IDC_DO_PING, &BTPingDialog::OnBnClickedDoPing)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// BTPingDialog 메시지 처리기입니다.
void initIniFile(TCHAR *pszFileName)
{
	//Open Log file
	HANDLE hFile = CreateFile(pszFileName, 
			GENERIC_READ | GENERIC_WRITE, 
			0, 
			NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL );

	//A file was opened?
	if( INVALID_HANDLE_VALUE == hFile )
	{
		// init ini file
		char szAnsi[256];
		unicode_to_ansi(pszFileName, szAnsi);
		CIniFile::Create(szAnsi);
		CIniFile::SetValue("cmd1", "localhost", "Ping", szAnsi);
		CIniFile::SetValue("cmd2", "192.168.1.1", "Ping", szAnsi);
		CIniFile::SetValue("cmd3", "192.168.1.100", "Ping", szAnsi);

		CIniFile::SetValue("cmd3", "-c localhost -i 1", "iPref Client", szAnsi);
		CIniFile::SetValue("cmd2", "-c 192.168.0.1 -t 20 -i 1", "iPref Client", szAnsi);
		CIniFile::SetValue("cmd1", "-c 192.168.0.1 -u -i 1", "iPref Client", szAnsi);

		CIniFile::SetValue("cmd1", "-s", "iPref Server", szAnsi);
		CIniFile::SetValue("cmd2", "-s -u", "iPref Server", szAnsi);

		//CIniFile::SetValue("R", 0, "Ping Graph", szAnsi);
		

		CIniFile::Sort(szAnsi, false);
		return;
	}

	//If already exists
	if( ERROR_ALREADY_EXISTS == GetLastError() )
	{
		CloseHandle( hFile );
		return;
	}

}


BOOL BTPingDialog::OnInitDialog()
{
	CPropertyViewPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ping_thread = new CPingThread;

	// Read ping section from ini file
	initIniFile(gszIniFile);


		char szAnsi[256];
		unicode_to_ansi(gszIniFile, szAnsi);

		vector<CIniFile::Record> s  = CIniFile::GetSection("Ping", szAnsi);
		vector<CIniFile::Record>::iterator i;
		for( i = s.begin(); i != s.end(); i++)
		{
			string value = CIniFile::GetValue( (*i).Key, "Ping", szAnsi);

			m_cbHosts.AddString(ansi_to_unicode(value.c_str()));
		}
	

	// this might be enough
	//m_cbHosts.ModifyStyle(0, CBS_AUTOHSCROLL);	
	//m_cbHosts.AddString(_T("skt.com"));	
	//m_cbHosts.AddString(_T("localhost"));	
	//m_cbHosts.AddString(_T("192.168.1.1"));
	//m_cbHosts.AddString(_T("192.168.1.2"));
	//m_cbHosts.AddString(_T("knu.ac.kr"));
	
	m_cbHosts.SetCurSel(0);

	

	//  최초 rect가 너무 작은 경우 오류 발생(OScope의 ratio 계산, points의 new double에서 발생 ※※)
	// 임시로 크기를 가져옴 (올ㅁ?)
	CRect rect;
	GetDlgItem(IDC_CUSTOM1)->GetClientRect(rect);
	//ScreenToClient(rect);
	
	m_OScopeCtrl.Create(WS_CHILD | WS_VISIBLE, rect, this);


	// customize the control
	m_OScopeCtrl.SetRange(0.0, 10.0, 1) ;
	m_OScopeCtrl.SetYUnits(L"ms") ;
	m_OScopeCtrl.SetXUnits(L"Time(sec)") ;
	m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
	m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
	m_OScopeCtrl.SetPlotColor(RGB(0, 255, 0)) ;

	// old fashioned
	//m_OScopeCtrl.SetBackgroundColor(RGB(0, 64, 0)) ;
	//m_OScopeCtrl.SetGridColor(RGB(192, 255, 192)) ;
	//m_OScopeCtrl.SetPlotColor(RGB(0, 255, 0)) ;	


	m_sHost.EnableWindow(TRUE, TRUE);
	//SetIcon(IDI_UAC_SHIELD, 32, FALSE)
	m_sHost.SetWindowText(L"Host", FALSE)
				.SetTextColor(RGB(0,0,255), FALSE)
				.SetBorderColor(RGB(255,0,0), FALSE)
				.SetBold(TRUE, FALSE)
				// .SetFont(_T("Comic Sans MS"), 10, FALSE)
				.SetAlignment(CXGroupBox::left, FALSE)
				.SetControlStyle(CXGroupBox::header, FALSE);


  	m_sVisual.EnableWindow(TRUE, TRUE);
	//SetIcon(IDI_UAC_SHIELD, 32, FALSE)
	m_sVisual.SetWindowText(L"Ping Graph", FALSE)
				.SetTextColor(RGB(0,0,255), FALSE)
						.SetBorderColor(RGB(255,0,0), FALSE)
						.SetBold(TRUE, FALSE)
						// .SetFont(_T("Comic Sans MS"), 10, FALSE)
						.SetAlignment(CXGroupBox::left, FALSE)
						.SetControlStyle(CXGroupBox::header, FALSE);


	// LB style 변경.
	DWORD dwStyle = m_lbPingResult.GetStyle();
	//m_lbPingResult.ModifyStyle(0, dwStyle | LB_SETHORIZONTALEXTENT);
	//::AfxGetMainWnd()->m_wndCommandBar;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void BTPingDialog::OnDestroy()
{
	CPropertyViewPage::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(ping_thread)
	{
		ping_thread->stop();
		WaitForSingleObject(ping_thread->thread_handle, INFINITE /* 2000*/);
	}

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
		CString szTarget;
		//TCHAR msg[256];

		// Combobox manipulation, CBS_AUTOHSCROLL must be in style
		int nSel = m_cbHosts.GetCurSel();

		if(nSel == -1) // nothing selected
		{
			// get user input in edit control
			m_cbHosts.GetWindowText(szTarget.GetBuffer(128), 128);//LOWORD(nCount)+1);//

			// szTarget의 길이에 따라 처리 필요
			if(szTarget.GetLength() < 0) // 호스트 주소가 없는 경우
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
			m_cbHosts.GetLBText(nSel, szTarget);
		}

		// update ini 
		UpdateIni(szTarget.GetBuffer());

		szTarget.ReleaseBuffer();
		// szTarget를 다시 ComboBox에 넣고 Ping 시작
		m_cbHosts.AddString(szTarget);

		

		opt.m_sHostToResolve = szTarget;
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

	CScreenLib::DockControl(m_hWnd, IDC_HOST, CScreenLib::dtTop);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.  
	CScreenLib::OptimizeWidth(m_hWnd, 4,
		IDC_HOST_COMBO, 
		IDC_RESULT_LIST, 
		IDC_STATIC_VISUAL,
		IDC_CUSTOM1
		);

	// IDC_HOST에 Ping 버튼 상 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_HOST, IDC_DO_PING);

	// 오른쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 핑 버튼을 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_HOST_COMBO, IDC_DO_PING);

	// 왼쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 호스트 텍스트를 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_HOST_COMBO, IDC_HOST);


	VerticalSpace(m_hWnd, IDC_HOST, IDC_HOST_COMBO, 6);
	VerticalSpace(m_hWnd, IDC_HOST_COMBO, IDC_RESULT_LIST, 3);

	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_STATIC_VISUAL, 6);
	VerticalSpace(m_hWnd, IDC_STATIC_VISUAL, IDC_CUSTOM1, 3);


	// 왼쪽 상단의 기준인 호스트 텍스트에 따라 LISt와 OSCOPECTRL을 정렬(왼쪽)
	//CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// OSCOPECTRL은 나머지 영역을 채운다
	//CScreenLib::OptimizeHeight(m_hWnd, IDC_CUSTOM1);
	//CScreenLib::DockControl(m_hWnd, IDC_CUSTOM1, CScreenLib::dtBottom);
	CRect r, cr, vr;
	//::GetClientRect(::GetDlgItem(m_hWnd, IDC_STATIC_VISUAL), vr);
	GetDlgItem(IDC_STATIC_VISUAL)->GetWindowRect(vr);
	ScreenToClient(vr);
	GetWindowRect(cr);
	ScreenToClient(cr);

	GetDlgItem(IDC_CUSTOM1)->GetWindowRect(r);
	ScreenToClient(r);

	GetDlgItem(IDC_CUSTOM1)->MoveWindow(r.left, 
		r.top, 
		r.Width(), 
		cr.Height() - vr.bottom);

	

	// IDC_OSCOPECTRL은 Control이 아니다. 고로 CScreenLib에서 오류가 발생. 
	// 별도의 PlaceHolder를 통해 처리함.
	if(m_OScopeCtrl.GetSafeHwnd() != NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Landscape)
		{
			//  가로 모드가 될 경우, RESULT_LIST의 높이가 달라지면.. 
			// 다시 Portrait가 될 경우 어떻게 처리하나?? 따라서, 그대로 커멘트 처리
			CScreenLib::OptimizeHeight(m_hWnd, IDC_RESULT_LIST);

			GetDlgItem(IDC_RESULT_LIST)->GetWindowRect(rect);
			//GetDlgItem(IDC_RESULT_LIST)->ShowWindow(SW_HIDE);
			
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);
		}
		else
		{
			GetDlgItem(IDC_RESULT_LIST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CUSTOM1)->GetWindowRect(rect);
			
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);//SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);//
		}
	}
}



void BTPingDialog::UpdateIni(TCHAR *pValue)
{
	char szFile[256];
	char szTempKey[32], szTempValue[128];
	unicode_to_ansi(gszIniFile, szFile);
	unicode_to_ansi(pValue, szTempValue);

	vector<CIniFile::Record> v = CIniFile::GetSection("Ping", szFile);
	
	vector<CIniFile::Record>::iterator i;
	for( i = v.begin(); i != v.end(); i++)
	{
		CIniFile::Record r = (*i);

		//  이미 존재할 경우
		if(r.Value.compare(szTempValue) == 0)
		{
			return;
		}
	}

	CString szTemp;
	szTemp.Format(L"cmd%d", v.size());
	unicode_to_ansi(szTemp.GetBuffer(), szTempKey);
	
	CIniFile::SetValue(szTempKey, szTempValue, "Ping", szFile);
	szTemp.ReleaseBuffer();

}