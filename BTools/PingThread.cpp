// PingThread.cpp: implementation of the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ping.h"
#include "PingThread.h"
//#include <process.h>    /* _beginthread, _endthread */
#include "BTPingDialog.h"
#include "Util.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL IsValidHandle( HANDLE hHandle) {
	return ((hHandle != NULL) && (hHandle != INVALID_HANDLE_VALUE));
}


CPingThread::CPingThread()
:m_bContinue(FALSE)
{
	m_hKillEvent =   CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hSignalEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
}

CPingThread::~CPingThread()
{
	//SetEvent(m_hKillEvent);
	WaitForSingleObject(m_hThread,INFINITE);
	
	if(IsValidHandle(m_hKillEvent) == TRUE)
		CloseHandle(m_hKillEvent);
	if(IsValidHandle(m_hSignalEvent) == TRUE)
		CloseHandle(m_hSignalEvent);
	
	m_bContinue = FALSE;
}



// vpn_connected
UINT CPingThread::ThreadProc(void* lpParam)
{ 
	BTPingDialog *parent = (BTPingDialog*)lpParam;
	//CPingThread *pThis = (CPingThread*)parent->ping_thread;

	//BOOL bSuccess = FALSE;
	CPing p;
	CPingReply pr;
	int nRequestsSent = 0;

	CString szMsg;

	TCHAR *log_dir = L"\\My Documents\\btools_log";
	TCHAR *log_file_postfix = L"ping.txt";
	// log
	TCHAR *pLogFile = SetupLog(log_dir, log_file_postfix);

	int n_inserted = parent->m_lbPingResult.AddString(_T("---- Ping started ----"));
	WriteLog(pLogFile, _T("---- Ping started ----"));

	parent->m_lbPingResult.SetCurSel(n_inserted);

	// set host to resolve

	//while (parent->is_thread_continue == TRUE)
	while (parent->ping_thread->m_bContinue == TRUE)
	{
		/*
		HANDLE hObjects[2];
		hObjects[0] = pThis->m_hKillEvent;
		hObjects[1] = pThis->m_hSignalEvent;

		DWORD dwWait = WaitForMultipleObjects(2,hObjects,FALSE,INFINITE);
		if (dwWait == WAIT_OBJECT_0)		
			break;

		if (dwWait == WAIT_OBJECT_0 + 1)
		{*/
		//		parent->opt.m_sHostToResolve = szServer;


		if (parent->opt.m_bUseRawSockets)
		{
				#ifndef CPING_NO_WINSOCK2
				if (parent->opt.m_sLocalBoundAddress.IsEmpty())
					parent->is_host_alive = p.PingUsingWinsock(parent->opt.m_sHostToResolve, pr, parent->opt.m_nTTL, parent->opt.m_dwTimeout, 
											  parent->opt.m_wDataRequestSize, parent->opt.m_nTOS, parent->opt.m_bDontFragment);
				else
				parent->is_host_alive  = p.PingUsingWinsock(parent->opt.m_sHostToResolve, pr, parent->opt.m_nTTL, parent->opt.m_dwTimeout, 
											  parent->opt.m_wDataRequestSize, parent->opt.m_nTOS, parent->opt.m_bDontFragment, parent->opt.m_sLocalBoundAddress);
				#endif
		}
		else
		{
				#ifndef CPING_NO_ICMP
				parent->is_host_alive  = p.PingUsingICMP(parent->opt.m_sHostToResolve, pr, parent->opt.m_nTTL, parent->opt.m_dwTimeout, 
										 parent->opt.m_wDataRequestSize, parent->opt.m_nTOS, parent->opt.m_bDontFragment);
				#endif
		}

		++nRequestsSent;
		if (parent->is_host_alive )
		{	
			hostent* phostent = NULL;
			if (parent->opt.m_bResolveAddressesToHostnames)
				phostent = gethostbyaddr(reinterpret_cast<char *>(&pr.Address.S_un.S_addr), 4, PF_INET);
			if (phostent)
			{
				CString sHost(phostent->h_name);
				szMsg.Format(_T("[%d] %d.%d.%d.%d [%s] replied in %d ms"), nRequestsSent,
					pr.Address.S_un.S_un_b.s_b1, pr.Address.S_un.S_un_b.s_b2, pr.Address.S_un.S_un_b.s_b3, 
					pr.Address.S_un.S_un_b.s_b4, sHost.operator LPCTSTR(), pr.RTT);
			}
			else
				szMsg.Format(_T("[%d] %d.%d.%d.%d replied in %d ms"), nRequestsSent,
				pr.Address.S_un.S_un_b.s_b1, pr.Address.S_un.S_un_b.s_b2, pr.Address.S_un.S_un_b.s_b3, 
				pr.Address.S_un.S_un_b.s_b4, pr.RTT);

			// append the new value to the plot
			parent->m_OScopeCtrl.AppendPoint(pr.RTT);
		}
		else
		{
			int icmp_error = GetLastError();
			int i;
			for( i = 0; i < sizeof(ICMPStatus2String)/sizeof(IDSTRING); i++)
			{
				if(ICMPStatus2String[i].id == icmp_error)
					break;
			}
			
			szMsg.Format(_T("Ping failed:%s"), ansi_to_unicode(ICMPStatus2String[i].status));
			//_tprintf(_T("Failed in ping specified host, GetLastError returns: %d"), GetLastError());
		}

		// 종료 조건 검사
		//Prepare for the next loop around?
		if (!parent->opt.m_bPingTillStopped)
		{
			if (nRequestsSent == parent->opt.m_nRequestsToSend)
			break;
		}		

		// Post WM_USER_PING_MESSAGE.
		//		if(parent != NULL){	parent->PostMessage(WM_USER_PING_MESSAGE, 0, (LPARAM)parent->is_host_alive);		}

		WriteLog(pLogFile, szMsg.GetBuffer());
		n_inserted = parent->m_lbPingResult.AddString(szMsg);
		parent->m_lbPingResult.SetCurSel(n_inserted);		

		Sleep(1000);
		//}  // WaitForMultipleObjects
	} // while


	// End of ping thread
	n_inserted = parent->m_lbPingResult.AddString(_T("---- Ping Finished ----"));
	WriteLog(pLogFile, _T("---- Ping Finished ----"));
	parent->m_lbPingResult.SetCurSel(n_inserted);

	return 0;
}

void CPingThread::run(void *lpParam)
{
	m_bContinue = TRUE;
	thread_handle = AfxBeginThread((AFX_THREADPROC)ThreadProc, (void*) lpParam );
	//SetEvent(m_hSignalEvent);
}


void CPingThread::ping()
{
	SetEvent(m_hSignalEvent);
}


void CPingThread::stop()
{
	m_bContinue = FALSE;
}

BOOL CPingThread::is_running()
{
	return m_bContinue;
}