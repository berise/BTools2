#pragma once

/*!
	\file BTIPerfServer.h
	\author berise
	\date 2008.12

	IPerf 서버 모듈을 이식한 것이다. IPerf 클라이언트를 기본으로 개발하였다.


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

// BTIPerfServer 대화 상자입니다.

/*!
	\class BTIPerfServer
	\brief IPerf 서버 UI

	IPerf의 서버는 클라이언트의 요청을 받아 일(!)을 하는 Server와 Server를 래핑한
	Listener, Audience가 있으나, 여기서는 Listener만을 사용하여 구현한다.
*/
class BTIPerfServer : public CPropertyViewPage, CReporter
{
	DECLARE_DYNAMIC(BTIPerfServer)

public:
	BTIPerfServer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~BTIPerfServer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IPERF_SERVER };

public:

	/*!
		\brief CReporter의 가상 함수
	*/
	void virtual PrintBuffer(char *buffer,char *speed);
	void virtual ClientFinished();
	virtual void CallbackBW(double fBW);


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
	
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	/// XGroupBox for displaying header groups
	CXGroupBox m_sCommands;
	CXGroupBox m_sOutput;
	CXGroupBox m_sInfo;
	afx_msg void OnStnClickedStaticInfo();
};
