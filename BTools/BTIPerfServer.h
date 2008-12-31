#pragma once
#include "afxwin.h"
#include "3rd/PropertyView.h"
#include "Server.hpp"
#include "Reporter.h"

class Listener;

// BTIPerfServer 대화 상자입니다.

class BTIPerfServer : public CPropertyViewPage, CReporter
{
	DECLARE_DYNAMIC(BTIPerfServer)

public:
	BTIPerfServer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~BTIPerfServer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IPERF_SERVER };

public:
	void virtual PrintBuffer(char *buffer,char *speed);
	void virtual ClientFinished();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_lbCommand;
	CListBox m_lbResult;

	BOOL ParseCommandLine(Settings *pSetting, CString &szCmd);


	ext_Settings* m_pextSettings;
	Settings *m_piperf_setting;


	HANDLE m_ThreadHandle;
	Listener *theListener;
	Server *m_pIPerfServer;

	FILE *m_fStatistics;

	CString	m_csReportFile;
	CString	m_csErrorFile;

	BOOL m_bServerStarted;

	afx_msg void OnBnClickedRunServer();
	afx_msg void OnLbnSelchangeCommandList();
	afx_msg void OnBnClickedAddCommand();

	//
	
void GetLocalIP(TCHAR *hostname, TCHAR *ip)
{
	//  PURPOSE:  
	//		- Obtain the current IP assign to the local PC
	//  PARAMETERS:
	//		- hWnd		:: Handle of the parent window.
	//  OPERATION:
	//		- Call the gethostname API
	//  RETURN VALUE:
	//      - NIL

	HOSTENT *LocalAddress;
	char	*Buff;
	TCHAR	*wBuff;


	// Create new string buffer
	Buff = new char[256];
	wBuff = new TCHAR[256];
	// Reset the string buffer
	memset(Buff, '\0', 256);
	memset(wBuff, TEXT('\0'), 256*sizeof(TCHAR));
	// Get computer name
	if (gethostname(Buff, 256) == 0)
	{
		// Convert computer name from MultiByte char to UNICODE
		mbstowcs(wBuff, Buff, 256);
		// Copy the machine name into the GUI control
		//SetWindowText(GetDlgItem(hWnd, IDC_MACHINE), wBuff);
		wcscpy(hostname, wBuff);


		// Get the local PC IP address
		LocalAddress = gethostbyname(Buff);
		// Reset the string buffer
		memset(Buff, '\0', 256);
		// Compose the obtain ip address
		sprintf(Buff, "%d.%d.%d.%d\0", LocalAddress->h_addr_list[0][0] & 0xFF, LocalAddress->h_addr_list[0][1] & 0x00FF, LocalAddress->h_addr_list[0][2] & 0x0000FF, LocalAddress->h_addr_list[0][3] & 0x000000FF);
		// Reset the wBuff
		memset(wBuff, TEXT('\0'), 256*sizeof(TCHAR));
		// Convert computer name from MultiByte char to UNICODE
		mbstowcs(wBuff, Buff, 256);
		// Set the ip address to edit control 1
		//SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), wBuff);
		wcscpy(ip, wBuff);
		// Set the default port number
		//SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), TEXT("5000"));
	}
	else
	{
		// Notify user about the error
		//MessageBox(hWnd, TEXT("Fail to get host name."), TEXT("MySocket"), MB_OK | MB_ICONEXCLAMATION);
		
	}


	//
	delete Buff;
}

};
