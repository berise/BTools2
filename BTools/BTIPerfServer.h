#pragma once

/*!
	\file BTIPerfServer.h
	\author berise
	\date 2008.12

	IPerf ���� ����� �̽��� ���̴�. IPerf Ŭ���̾�Ʈ�� �⺻���� �����Ͽ���.


	\todo
	\li Consistent log
	\li Add error text to log

	\bug
	Still didn't appeared
*/
#include "afxwin.h"

#include "Server.hpp"
#include "Reporter.h"

#include "3rd/XGroupBox.h"
#include "3rd/PropertyView.h"



class Listener;

// BTIPerfServer ��ȭ �����Դϴ�.

/*!
	\class BTIPerfServer
	\brief IPerf ���� UI

	IPerf�� ������ Ŭ���̾�Ʈ�� ��û�� �޾� ��(!)�� �ϴ� Server�� Server�� ������
	Listener, Audience�� ������, ���⼭�� Listener���� ����Ͽ� �����Ѵ�.
*/
class BTIPerfServer : public CPropertyViewPage, CReporter
{
	DECLARE_DYNAMIC(BTIPerfServer)

public:
	BTIPerfServer(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~BTIPerfServer();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IPERF_SERVER };

public:

	/*!
		\brief CReporter�� ���� �Լ�
	*/
	void virtual PrintBuffer(char *buffer,char *speed);
	void virtual ClientFinished();
	virtual void CallbackBW(double fBW);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	/// XGroupBox for displaying header groups
	CXGroupBox m_sCommands;
	CXGroupBox m_sOutput;
	CXGroupBox m_sInfo;
	afx_msg void OnStnClickedStaticInfo();
};
