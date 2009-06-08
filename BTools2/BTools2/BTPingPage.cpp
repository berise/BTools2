#include "stdafx.h"
#include "BTPing.h"
#include "BTPingPage.h"
#include "Util.h"




// ping thread
DWORD WINAPI PingThread(LPVOID lpParameter )
{
	CBTPingPage *c=(CBTPingPage *)lpParameter;

	TCHAR szTarget[256];
	_tcscpy(szTarget, L"127.0.0.1");
	//c->m_target_host.GetWindowText(szTarget, 256);

	// BTPing�� Thread�ȿ��� �����Ǿ�� �Ѵ�. 
	// ICMP ��Ŷ�� id �ʵ�� ���� ������(���μ���) ID�� �ʿ��ϱ� �����̴�.
	BTPing btPing(c);

	btPing.SetDoLookup(true);
	btPing.SetMaxTimeOuts(6);

	//while(c->m_bStartThread )
	{
		//Ping( LPCWSTR dest, int timeOut, int dataSize, int interval, unsigned short nSendCount)
		btPing.Ping(szTarget, 5000, 32, 1000, 4);
	}

	return 0;
}



BOOL CBTPingPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	/*
	//m_sHost.Create(m_hWnd, NULL, NULL);
	m_sHost.Attach(GetDlgItem(IDC_HOST));
	//m_sHost = GetDlgItem(IDC_HOST);
			//m_box.Attach(::GetDlgItem(m_hWnd, IDC_STATIC_TODO));
	m_sHost.PostSubclassWindow();
	m_sHost.SetWindowText(L"Target Host", FALSE)
				.SetTextColor(RGB(0,0,255), FALSE)
				.SetBorderColor(RGB(255,0,0), FALSE)
				//.SetBold(TRUE, FALSE)
				// .SetFont(_T("Comic Sans MS"), 10, FALSE)
				//.SetAlignment(CXGroupBox::left, FALSE)
				.SetControlStyle(CXGroupBox::header, TRUE);
*/


	GetDlgItem(IDC_STATIC_PLACEHOLDER).ShowWindow(SW_HIDE);
	CRect rect;
	::GetWindowRect(GetDlgItem(IDC_STATIC_PLACEHOLDER), rect);
	//ScreenToClient(rect);
	
	// create the control
	HWND ret = m_OScopeCtrl.Create(m_hWnd, rect, NULL) ; 
	
	if(ret != NULL)
	{
		//m_OScopeCtrl.InvalidateCtrl();
	// customize the control
	m_OScopeCtrl.SetRange(0.0, 100.0, 1) ;
	m_OScopeCtrl.SetYUnits(L"Latency(ms)") ; // 
	m_OScopeCtrl.SetYUnits(L"ms") ; //  �ʹ� ���� �ʰ� ���� �ʿ�.
	m_OScopeCtrl.SetXUnits(L"Period(ms)") ; // �ʹ� ���� �ʵ��� ���� �ʿ�. �ʿ�� InvalidateCtrl���� ���� �ʿ�
	m_OScopeCtrl.SetBackgroundColor(RGB(0, 0, 64)) ;
	m_OScopeCtrl.SetGridColor(RGB(192, 192, 255)) ;
	m_OScopeCtrl.SetPlotColor(RGB(255, 0, 255)) ;

	}
	//m_bStop = FALSE ;
	//srand( GetTickCount() );

	m_bStartThread = FALSE;
	m_hThread = NULL;

	return TRUE; // set focus to default control
}




LRESULT CBTPingPage::OnPing(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{

	//::AtlMessageBox(NULL, L"OnPing");
	if(!m_bStartThread)
	{		
		m_hThread = CreateThread(NULL,NULL, PingThread,this,0,NULL);
		m_bStartThread = TRUE;
	}
	else
	{
		m_bStartThread = FALSE;
		if(m_hThread != NULL)
		{
			DWORD ret = WaitForSingleObject(m_hThread, INFINITE);
			if(ret == WAIT_TIMEOUT)
				::AtlMessageBox(NULL, L"wait timed out // error");
			else if (ret == WAIT_FAILED)
				::AtlMessageBox(NULL, L"wait failed");
			else 
			{
				m_hThread = NULL;
			}
		}
	}

	return 0;
}

LRESULT CBTPingPage::OnDestroy(void)
{
	//m_sHost.Detach();

	//You should call SetMsgHandled(FALSE) or set bHandled = FALSE for the main window of your application
	//::AtlMessageBox(NULL, L"CBTPingPage::OnDestroy");
	m_bStartThread = FALSE;
	if(m_hThread != NULL)
	{
		DWORD ret = WaitForSingleObject(m_hThread, INFINITE);
		if(ret == WAIT_TIMEOUT)
			::AtlMessageBox(NULL, L"wait timed out // error");
		else if (ret == WAIT_FAILED)
			::AtlMessageBox(NULL, L"wait failed");
		else 
			{
				m_hThread = NULL;
			}
	}

	return 0;
}






void CBTPingPage::OnSize(UINT state, CSize Size)
{
	CScreenLib::DockControl(m_hWnd, IDC_HOST, CScreenLib::dtTop);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.  
	CScreenLib::OptimizeWidth(m_hWnd, 5,
		IDC_HOST,
		IDC_HOST_COMBO, 
		IDC_RESULT_LIST, 
		IDC_STATIC_VISUAL,
		IDC_STATIC_PLACEHOLDER
		);

	// In order to show ping button on the top of IDC_HOST, IDC_HOST must be shrinked to width of IDC_PING
	CRect r1, r2;
	GetDlgItem(IDC_PING).GetClientRect(r1);
	GetDlgItem(IDC_HOST).GetClientRect(r2);
	GetDlgItem(IDC_HOST).MoveWindow(r2.left, r2.top, r2.Width() - r1.Width(), r2.Height());


	// �ֻ�� ����
	VerticalSpace(m_hWnd, IDC_HOST, IDC_HOST, -5);
	VerticalSpace(m_hWnd, IDC_HOST, IDC_HOST_COMBO, 6);
	VerticalSpace(m_hWnd, IDC_HOST_COMBO, IDC_RESULT_LIST, 3);

	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_STATIC_VISUAL, 6);
	VerticalSpace(m_hWnd, IDC_STATIC_VISUAL, IDC_STATIC_PLACEHOLDER, 3);


	// IDC_HOST�� Ping ��ư �� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_HOST, IDC_PING);

	// ������ ������ IDC_HOST_COMBO(�ּ�â)�� �������� �� ��ư�� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 1, IDC_HOST_COMBO, IDC_PING);

	// ���� ������ IDC_HOST_COMBO(�ּ�â)�� �������� ȣ��Ʈ �ؽ�Ʈ�� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 1, IDC_HOST_COMBO, IDC_HOST);



	// ���� ����� ������ ȣ��Ʈ �ؽ�Ʈ�� ���� LISt�� OSCOPECTRL�� ����(����)
	//CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 2, IDC_HOST, IDC_RESULT_LIST, IDC_CUSTOM1);

	// OSCOPECTRL�� ������ ������ ä���
	//CScreenLib::OptimizeHeight(m_hWnd, IDC_STATIC_VISUAL);
	//CScreenLib::DockControl(m_hWnd, IDC_STATIC_VISUAL, CScreenLib::dtBottom);
	
	
	// IDC_STATIC_VISUAL�Ʒ����� ���� ������ ũ�⸦ ����Ͽ� OScopeCtrl�� ���� ä���.
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
		

	

	// IDC_OSCOPECTRL�� Control�� �ƴϴ�. ��� CScreenLib���� ������ �߻�. 
	// ������ PlaceHolder�� ���� ó����.
	if(m_OScopeCtrl.m_hWnd != NULL)
	{
		CRect rect;

		if(DRA::GetDisplayMode() == DRA::Landscape)
		{
			//  ���� ��尡 �� ���, RESULT_LIST�� ���̰� �޶�����.. 
			// �ٽ� Portrait�� �� ��� ��� ó���ϳ�?? ����, �״�� Ŀ��Ʈ ó��
			CScreenLib::OptimizeHeight(m_hWnd, IDC_RESULT_LIST);

			GetDlgItem(IDC_RESULT_LIST).GetWindowRect(rect);
			//GetDlgItem(IDC_RESULT_LIST)->ShowWindow(SW_HIDE);
			
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);
		}
		else
		{
			//GetDlgItem(IDC_RESULT_LIST).ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(rect);//IDC_STATIC_PLACEHOLDER
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);//
			
			//m_OScopeCtrl.SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE);//
		}
	}

	return ;
}





//
void CBTPingPage::Log(TCHAR *wszLog)
{
	CListBox lb;
	lb.Attach(GetDlgItem(IDC_RESULT_LIST));

	if(wszLog != NULL)
		lb.AddString(wszLog);

}