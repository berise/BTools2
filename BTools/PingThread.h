// PingThread.h: interface for the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
#define AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ping.h"


// PING MESSAGE in WM_USER space
#define WM_USER_PING_MESSAGE WM_USER+999



class CPingThread  
{
public:
	CPingThread();
	virtual ~CPingThread();

	BOOL	m_bContinue;
	UINT	m_dwID;
	HANDLE	m_hThread;

	HANDLE m_hKillEvent;
	HANDLE m_hSignalEvent;


	CWinThread *thread_handle;
	static UINT __cdecl ThreadProc(void* param);

	void run(void *lpParam);
	void ping();
	void stop();
	BOOL is_running();

//	CPingOption opt;
};

#endif // !defined(AFX_PINGTHREAD_H__88E7C296_2B7A_4A67_AA7B_C89EA8D3BAAC__INCLUDED_)
