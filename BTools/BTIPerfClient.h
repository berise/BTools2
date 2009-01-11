#pragma once
#include "3rd/PropertyView.h"

#include "Client.hpp"
#include "Reporter.h"
#include "afxwin.h"


// BTIPerfClient ��ȭ �����Դϴ�.

class BTIPerfClient : public CPropertyViewPage, CReporter
{
	DECLARE_DYNAMIC(BTIPerfClient)

public:
	BTIPerfClient(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~BTIPerfClient();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IPERF_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	void virtual PrintBuffer(char *buffer,char *speed);
	void virtual ClientFinished();


	FILE *m_fStatistics;

	BOOL m_bClientStarted;
	Client *m_pIPerfClient;

	ext_Settings* m_pSettings;
	Settings *m_piperf_setting;


	CString m_csHostName;
	CString	m_csReportFile;
	CString	m_csErrorFile;

	BOOL ParseCommandLine(Settings *pSetting, CString &szCmd);

	afx_msg void OnBnClickedRunClient();
	CListBox m_lbCommand;
	virtual BOOL OnInitDialog();
	CListBox m_lbResult;
	afx_msg void OnBnClickedAddCommand();
	afx_msg void OnLbnSelchangeCommandList();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
