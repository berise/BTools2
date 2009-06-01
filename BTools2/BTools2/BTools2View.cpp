// BTools2View.cpp : implementation of the CBTools2View class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resourceppc.h"

#include "BTPing.h"
#include "BTools2View.h"


// helper function
// move hwndAlignee to just below hwnd apart nx (dialog unit)
// align points are left & bottom.
// 왼쪽 정렬은 제거. 상하 만 조절
void VerticalSpace(HWND hwndDlg, UINT nIDAlignCtrl, UINT nIDAffectedCtl, UINT nx)
{
	ASSERT(hwndDlg != NULL);

	HWND hwndAlign = ::GetDlgItem(hwndDlg, nIDAlignCtrl);
	ASSERT(hwndAlign!= NULL);
	RECT rcAlign;
	::GetWindowRect(hwndAlign, &rcAlign);

	POINT ptMoveTo;
	ptMoveTo.x = rcAlign.left;
	ptMoveTo.y = rcAlign.bottom;

	::ScreenToClient(hwndDlg, &ptMoveTo);

	// Verify the affected control and obtain its rect
	HWND hwndCtl = ::GetDlgItem(hwndDlg, nIDAffectedCtl);
	ASSERT(hwndCtl != NULL);
	RECT rcCtl;
	::GetWindowRect(hwndCtl, &rcCtl);
	POINT ptCtl;
	ptCtl.x = rcCtl.left;
	ptCtl.y = rcCtl.bottom;
	::ScreenToClient(hwndDlg, &ptCtl);


	::SetWindowPos(hwndCtl, NULL, ptCtl.x, ptMoveTo.y + nx,
			0, 0, SWP_NOSIZE|SWP_NOZORDER);

	/*
	::MoveWindow(hwndCtl, 
		ptMoveTo.x, 
		ptMoveTo.y + nx,
		rcCtl.right - rcCtl.left,
		rcCtl.bottom - rcCtl.top,
		TRUE);
	*/	
}



// ping thread
DWORD WINAPI StartThread(LPVOID lpParameter )
{
	CPingPage *c=(CPingPage *)lpParameter;

	TCHAR szTarget[256];
	_tcscpy(szTarget, L"127.0.0.1");
	//c->m_target_host.GetWindowText(szTarget, 256);

	// BTPing은 Thread안에서 생성되어야 한다. 
	// ICMP 패핏의 id 필드는 현재 쓰레드(프로세스) ID를 필요하기 때문이다.
	BTPing btPing(c);

	btPing.SetDoLookup(true);
	btPing.SetMaxTimeOuts(6);

	//while(c->m_bThreadStarted )
	{
		btPing.Ping(szTarget, 5000, 32, 500, 11);
		//c->m_OScopeCtrl.AppendPoint(44.9);
		//Sleep(500);
	}

	
	/*

	TCHAR szTarget[256];
	c->m_target_host.GetWindowText(szTarget, 256);

	c->m_ping->SetDoLookup(true);
	c->m_ping->SetMaxTimeOuts(6);

	c->m_ping->Ping(szTarget, 5000, 32, 5);
	// to change the program to a trace route comment the above line 
        // and uncomment the line below        
        //icmp.TraceRoute  (buf, 5000, 32);
*/
	return 0;
}


BOOL CBTools2View::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

CBTools2View::CBTools2View()
{
	//SetTitle(_T("Properties"));
	//SetLinkText(_T("Tap <file:\\Windows\\default.htm{here}>."));

	m_Page1.SetTitle(_T("Ping"));
	AddPage(m_Page1);
	m_Page2.SetTitle(_T("Client"));
	AddPage(m_Page2);

	m_Page3.SetTitle(_T("Server"));
	AddPage(m_Page3);
}


void CPingPage::OnSize(UINT state, CSize Size)
{
	CScreenLib::DockControl(m_hWnd, IDC_HOST, CScreenLib::dtTop);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.  
	CScreenLib::OptimizeWidth(m_hWnd, 4,
		IDC_HOST_COMBO, 
		IDC_RESULT_LIST, 
		IDC_STATIC_VISUAL,
		IDC_STATIC_PLACEHOLDER
		);

	// IDC_HOST에 Ping 버튼 상 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_HOST, IDC_PING);

	// 오른쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 핑 버튼을 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_HOST_COMBO, IDC_PING);

	// 왼쪽 정렬은 IDC_HOST_COMBO(주소창)을 기준으로 호스트 텍스트를 정렬
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_HOST_COMBO, IDC_HOST);


	VerticalSpace(m_hWnd, IDC_HOST, IDC_HOST_COMBO, 6);
	VerticalSpace(m_hWnd, IDC_HOST_COMBO, IDC_RESULT_LIST, 3);

	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_STATIC_VISUAL, 6);
	VerticalSpace(m_hWnd, IDC_STATIC_VISUAL, IDC_STATIC_PLACEHOLDER, 3);


	// 왼쪽 상단의 기준인 호스트 텍스트에 따라 LISt와 OSCOPECTRL을 정렬(왼쪽)
	//CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// OSCOPECTRL은 나머지 영역을 채운다
	//CScreenLib::OptimizeHeight(m_hWnd, IDC_STATIC_VISUAL);
	//CScreenLib::DockControl(m_hWnd, IDC_STATIC_VISUAL, CScreenLib::dtBottom);
	
	
	// IDC_STATIC_VISUAL아래쪽의 남은 공간의 크기를 계산하여 OScopeCtrl을 가득 채운다.
	CRect r, cr, vr;
	//::GetClientRect(::GetDlgItem(m_hWnd, IDC_STATIC_VISUAL), vr);
	GetDlgItem(IDC_STATIC_VISUAL).GetWindowRect(vr);
	ScreenToClient(vr);
	GetWindowRect(cr);
	ScreenToClient(cr);

	GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(r);
	ScreenToClient(r);

	GetDlgItem(IDC_STATIC_PLACEHOLDER).MoveWindow(r.left, 
		r.top, 
		r.Width(), 
		cr.Height() - vr.bottom - 5); // padding(-5)
		

	

	// IDC_OSCOPECTRL은 Control이 아니다. 고로 CScreenLib에서 오류가 발생. 
	// 별도의 PlaceHolder를 통해 처리함.
	if(m_OScopeCtrl.m_hWnd != NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Landscape)
		{
			//  가로 모드가 될 경우, RESULT_LIST의 높이가 달라지면.. 
			// 다시 Portrait가 될 경우 어떻게 처리하나?? 따라서, 그대로 커멘트 처리
			CScreenLib::OptimizeHeight(m_hWnd, IDC_RESULT_LIST);

			GetDlgItem(IDC_RESULT_LIST).GetWindowRect(rect);
			//GetDlgItem(IDC_RESULT_LIST)->ShowWindow(SW_HIDE);
			
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);
		}
		else
		{
			GetDlgItem(IDC_RESULT_LIST).ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(rect);
			ScreenToClient(rect);
			//m_OScopeCtrl.MoveWindow(rect);//
			m_OScopeCtrl.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);//
		}
	}



	return ;
}

BOOL CPingPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	CRect rect;
	::GetClientRect(GetDlgItem(IDC_STATIC_PLACEHOLDER), rect) ;
	
	// create the control
	m_OScopeCtrl.Create(m_hWnd, rect, NULL, WS_VISIBLE | WS_CHILD, 0) ; 
	
	// customize the control
	m_OScopeCtrl.SetRange(0.0, 100.0, 1) ;
	m_OScopeCtrl.SetYUnits(L"Latency(ms)") ; // 
	m_OScopeCtrl.SetYUnits(L"ms") ; //  너무 길지 않게 조절 필요.
	m_OScopeCtrl.SetXUnits(L"Period(ms)") ; // 너무 길지 않도록 설정 필요. 필요시 InvalidateCtrl내부 수정 필요
	m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
	m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
	m_OScopeCtrl.SetPlotColor(RGB(255, 0, 255)) ;

	//m_bStop = FALSE ;
	//srand( GetTickCount() );

	m_bStartThread = FALSE;

	return TRUE; // set focus to default control
}




///////////


void CIPerfClientPage::OnSize(UINT state, CSize Size)
{

	return ;
}



BOOL CIPerfClientPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{


	return TRUE; // set focus to default control
}


void CIPerfServerPage::OnSize(UINT state, CSize Size)
{

	return ;
}

BOOL CIPerfServerPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{


	return TRUE; // set focus to default control
}


//LRESULT CBTools2View::OnInitDialog(HWND hwndFocus, LPARAM lParam)
int  CBTools2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	SHMENUBARINFO mbi;
                ZeroMemory(&mbi, sizeof(SHMENUBARINFO));

                mbi.cbSize     = sizeof(SHMENUBARINFO);
                mbi.hwndParent = m_hWnd;
                mbi.nToolBarId = IDR_MENU1;
				mbi.hInstRes   = _Module.GetModuleInstance();
                mbi.dwFlags    = SHCMBF_HMENU;

				SHCreateMenuBar(&mbi);

				::AtlMessageBox(NULL, L"init");
				/*
                if(SHCreateMenuBar(&mbi))
                {
                    g_hwndMb          = mbi.hwndMB;
                    TBBUTTONINFO tbbi = {0};
                    tbbi.cbSize       = sizeof(tbbi);
                    tbbi.dwMask       = TBIF_LPARAM | TBIF_BYINDEX;

                    SendMessage(g_hwndMb, TB_GETBUTTONINFO,0, (LPARAM)&tbbi);
                    g_hMenu           = (HMENU)tbbi.lParam;
                }
				*/


	return TRUE; // set focus to default control
}


LRESULT CBTools2View::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{

	SHMENUBARINFO mbi;
                ZeroMemory(&mbi, sizeof(SHMENUBARINFO));

                mbi.cbSize     = sizeof(SHMENUBARINFO);
                mbi.hwndParent = m_hWnd;
                mbi.nToolBarId = IDR_MENU1;
				mbi.hInstRes   = _Module.GetResourceInstance();
                mbi.dwFlags    = SHCMBF_HMENU;

				SHCreateMenuBar(&mbi);

				::AtlMessageBox(NULL, L"init");
				/*
                if(SHCreateMenuBar(&mbi))
                {
                    g_hwndMb          = mbi.hwndMB;
                    TBBUTTONINFO tbbi = {0};
                    tbbi.cbSize       = sizeof(tbbi);
                    tbbi.dwMask       = TBIF_LPARAM | TBIF_BYINDEX;

                    SendMessage(g_hwndMb, TB_GETBUTTONINFO,0, (LPARAM)&tbbi);
                    g_hMenu           = (HMENU)tbbi.lParam;
                }
				*/


	return TRUE; // set focus to default control
}

LRESULT CBTools2View::OnClose(void)
{

	::AtlMessageBox(NULL, L"OnClose");
	return 0;
}

LRESULT CPingPage::OnPing(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	//::AtlMessageBox(NULL, L"OnPing");
	if(!m_bStartThread)
	{		
		CreateThread(NULL,NULL,StartThread,this,0,NULL);
		m_bStartThread = TRUE;
	}
	else
	{
		m_bStartThread = FALSE;
	}

	return 0;
}
