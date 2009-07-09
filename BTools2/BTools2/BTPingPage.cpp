#include "stdafx.h"
#include "BTPing.h"
#include "BTPingPage.h"
#include "Util.h"


void CBTPingPage::OnReceiveICMP()
{
	double nResponseDuration = GetResponseDuration();
	static double  minY = 0.0, maxY = 100.0;
	if(nResponseDuration < minY)
	{
		minY = nResponseDuration-10;
		m_OScopeCtrl.SetRange(minY, maxY,  1) ;
	}

	if(nResponseDuration > maxY)
	{
		maxY = nResponseDuration+10;
		m_OScopeCtrl.SetRange(minY, maxY,  1) ;
	}

	m_OScopeCtrl.AppendPoint(nResponseDuration);

	TCHAR wszLog[256], wszHost[256];
	size_t hostlen;
	

	GetResponseAddress(wszHost, 256, &hostlen);
	wsprintf(wszLog, L"%d, %s, %d ms", 		
		GetResponseSequence(),
		wszHost, //GetResponseAddress(),
		GetResponseDuration());

	Log(wszLog);
	
	/*
    cout << endl << GetResponseSequence() << ". " 
        << GetResponseDuration() << "ms" << " \t " 
        << GetResponseAddress() << "\t" 
        << GetResponseName() << endl 
        << "Type: " << GetResponseType() 
        << "  Code: " << GetResponseCode() << " "
        << GetResponseMessage() << endl;
    if(GetResponseFinished())
        cout << endl << "Data finshed." << endl;
		*/
}

// OnError is a member of the CUT_WSClient class, from which
// CUT_ICMP inherits...
int CBTPingPage::OnError(int error)
{	
    //if(UTE_SUCCESS != error && UTE_ERROR != error)
	{
		TCHAR wszLog[256];
		wsprintf(wszLog, L"%s", 		
		CUT_ERR::GetErrorString(error));
		Log(wszLog);
	}
        
		
    return error;
}


/// user pressed stop button.
BOOL CBTPingPage::IsAborted()
{

	return m_bStartThread == 1 ? FALSE : TRUE;
}



// ping thread
DWORD WINAPI PingThread(LPVOID lpParameter )
{
	CBTPingPage *pThis=(CBTPingPage *)lpParameter;

	CString szTarget;

	// Combobox manipulation, CBS_AUTOHSCROLL must be in style
	int nSel = pThis->m_cbHosts.GetCurSel();
	if(nSel == -1) // nothing selected
		{
			// get user input in edit control
			pThis->m_cbHosts.GetWindowText(szTarget.GetBuffer(128), 128);//LOWORD(nCount)+1);//

			// szTarget�� ���̿� ���� ó�� �ʿ�
			if(szTarget.GetLength() < 0) // ȣ��Ʈ �ּҰ� ���� ���
			{
				return 0;
			}
			/*	DWORD nCount = m_cbHosts.GetEditSel();
			if(LOWORD(nCount) > 0)  ���� Ŀ���� ��ġ (0 ... ^    \0)			*/
		}
		else // get selected ListBox string
		{
			pThis->m_cbHosts.GetLBText(nSel, szTarget);
		}


	// BTPing�� Thread�ȿ��� �����Ǿ�� �Ѵ�.
	// ICMP ��Ŷ�� id �ʵ�� ���� ������(���μ���) ID�� �ʿ��ϱ� �����̴�.	
	pThis->m_pPing = new BTPing(pThis);
	pThis->m_pPing->SetDoLookup(true);
	pThis->m_pPing->SetMaxTimeOuts(6);

	//while(pThis->m_bStartThread )
	{
		
		pThis->m_pPing->Ping(szTarget, 5000, pThis->m_nDataSize, 1000, pThis->m_nSendCount == -1 ? 99999 :pThis->m_nSendCount);
	}
	

	// Thread �����
	EnterCriticalSection(&pThis->m_cs);
	pThis->GetDlgItem(IDC_DO_PING).SetWindowText(_T("&Ping"));
	int nIns = pThis->m_lbPingResult.AddString(_T("---- Finished ----"));
	pThis->m_lbPingResult.SetCurSel(nIns);
	LeaveCriticalSection(&pThis->m_cs);
	pThis->m_bStartThread = FALSE;

	// zap away
	DELETE_PTR(pThis->m_pPing);

	return 0;
}



BOOL CBTPingPage::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = GetParent();
	mbi.nToolBarId = IDR_MENU1;
	mbi.hInstRes   = _Module.GetResourceInstance();
	mbi.dwFlags    = SHCMBF_HMENU;
	SHCreateMenuBar(&mbi);


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
	
	m_bStartThread = FALSE;
	m_hThread = NULL;

	m_pPing = NULL;

	 // ��Ʈ�� ����
	DoDataExchange(FALSE);
	CComboBox cbInt = GetDlgItem(IDC_CB_DATA_SIZE);
	cbInt.AddString(L"4");
	cbInt.AddString(L"8");
	cbInt.AddString(L"16");
	cbInt.AddString(L"32");
	cbInt.AddString(L"64");
	cbInt.AddString(L"128");
	cbInt.AddString(L"256");
	cbInt.SetCurSel(3);

	cbInt = GetDlgItem(IDC_CB_SEND_COUNT);
	cbInt.AddString(L"-1");
	cbInt.AddString(L"4");
	cbInt.AddString(L"999"); // boy loves this number.
	cbInt.SetCurSel(1);

	m_nDataSize = 32; // 4, 8, 16, 32, 64, 128, 256 (kB)
	m_nSendCount = 4; // -1, 4, 9, 999
	
	m_cbHosts.AddString(L"kldp.org");
	m_cbHosts.AddString(L"localhost");	
	m_cbHosts.SetCurSel(1);


	// prepare to log
	TCHAR *log_dir = L"\\My Documents\\btools_log";
	TCHAR *log_file_postfix = L"ping.txt";
	// log
	TCHAR *pLogFile = SetupLog(log_dir, log_file_postfix);
	_tcscpy(m_szLogFile, pLogFile);

	// Initialize the critical section
    InitializeCriticalSection(&m_cs);

	return TRUE; // set focus to default control
}




LRESULT CBTPingPage::OnPing(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	int nIns = 0 ;
	if(m_bStartThread == FALSE)
	{
		DoDataExchange(TRUE);
		CString szMsg;
		szMsg.Format(L"%d, %d", m_nDataSize, m_nSendCount);
		//AtlMessageBox(NULL, szMsg.GetBuffer(), L"debug");

		m_hThread = CreateThread(NULL,NULL, PingThread,this,0,NULL);
		

		EnterCriticalSection(&m_cs);

		m_bStartThread = TRUE;
		GetDlgItem(IDC_DO_PING).SetWindowText(_T("Stop"));
		nIns = m_lbPingResult.AddString(_T("---- Ping started ----"));
		m_lbPingResult.SetCurSel(nIns);
		LeaveCriticalSection(&m_cs);
	}
	else
	{
		EnterCriticalSection(&m_cs);		
		m_bStartThread = FALSE;
		m_pPing->IsAborted();
		LeaveCriticalSection(&m_cs);

	}

	return 0;
}

LRESULT CBTPingPage::OnDestroy(void)
{
	DeleteCriticalSection(&m_cs);

	//You should call SetMsgHandled(FALSE) or set bHandled = FALSE for the main window of your application
	m_bStartThread = FALSE;
	if(m_pPing != NULL)
		m_pPing->IsAborted();

	/*
	
	if(m_hThread != NULL)
	{
		DWORD ret = WaitForSingleObject(m_hThread, 3000);
		if(ret == WAIT_TIMEOUT)
			;//::AtlMessageBox(NULL, L"wait timed out // error");
		else if (ret == WAIT_FAILED)
			;//::AtlMessageBox(NULL, L"wait failed");
		else 
			{
				m_hThread = NULL;
			}
	}
	*/
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
	VerticalSpace(m_hWnd, IDC_HOST, IDC_HOST_COMBO, 6);
	VerticalSpace(m_hWnd, IDC_HOST_COMBO, IDC_STATIC_DATA_SIZE, 3);
	VerticalSpace(m_hWnd, IDC_HOST_COMBO, IDC_STATIC_SEND_COUNT, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_DATA_SIZE, IDC_CB_DATA_SIZE, 3);
	VerticalSpace(m_hWnd, IDC_STATIC_SEND_COUNT, IDC_CB_SEND_COUNT, 3);

	VerticalSpace(m_hWnd, IDC_CB_SEND_COUNT, IDC_RESULT_LIST, 3);



	VerticalSpace(m_hWnd, IDC_RESULT_LIST, IDC_STATIC_VISUAL, 6);
	VerticalSpace(m_hWnd, IDC_STATIC_VISUAL, IDC_STATIC_PLACEHOLDER, 3);


	// IDC_HOST�� Ping ��ư �� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atTop, 1, IDC_HOST, IDC_PING);

	// ������ ������ IDC_HOST_COMBO(�ּ�â)�� �������� �� ��ư�� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atRight, 3, IDC_HOST_COMBO, 
					IDC_PING, IDC_STATIC_SEND_COUNT, IDC_CB_SEND_COUNT);

	// ���� ������ IDC_HOST_COMBO(�ּ�â)�� �������� ȣ��Ʈ �ؽ�Ʈ�� ����
	CScreenLib::AlignControls(m_hWnd, CScreenLib::atLeft, 3, IDC_HOST_COMBO, 
					IDC_HOST, IDC_STATIC_DATA_SIZE, IDC_CB_DATA_SIZE);



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
			GetDlgItem(IDC_STATIC_PLACEHOLDER).GetWindowRect(rect);//IDC_STATIC_PLACEHOLDER
			ScreenToClient(rect);
			m_OScopeCtrl.MoveWindow(rect);//
		}
	}

	return ;
}





//
void CBTPingPage::Log(TCHAR *wszLog)
{
	EnterCriticalSection(&m_cs);
	if(wszLog != NULL) 
	{
		
		int nIns = m_lbPingResult.AddString(wszLog);
		WriteLog(m_szLogFile, wszLog);
		m_lbPingResult.SetCurSel(nIns);		
	}
	LeaveCriticalSection(&m_cs);
}
