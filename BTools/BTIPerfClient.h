#pragma once



#include "Client.hpp"
#include "Reporter.h"

#include "3rd/PropertyView.h"
#include "3rd/XGroupBox.h"
#include "3rd/OScopeCtrl.h"


// BTIPerfClient 대화 상자입니다.

/*! \brief Delegation of IPerf Client. It is basically a CView!
*/
class BTIPerfClient : public CPropertyViewPage, CReporter
{
	DECLARE_DYNAMIC(BTIPerfClient)

public:
	BTIPerfClient(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~BTIPerfClient();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IPERF_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual void PrintBuffer(char *buffer,char *speed);
	virtual void ClientFinished();
	virtual void CallbackBW(double fBW);



	BOOL ParseCommandLine(Settings *pSetting, CString &szCmd);

	afx_msg void OnBnClickedRunClient();
	CListBox m_lbCommand;
	virtual BOOL OnInitDialog();
	CListBox m_lbResult;
	afx_msg void OnBnClickedAddCommand();
	afx_msg void OnLbnSelchangeCommandList();
	afx_msg void OnSize(UINT nType, int cx, int cy);


private:
	// Member variables
	FILE *m_fStatistics;

	BOOL m_bClientStarted;
	Client *m_pIPerfClient;

	ext_Settings* m_pSettings;
	Settings *m_piperf_setting;


	//CString m_csHostName;
	CString	m_csReportFile;
	CString	m_csErrorFile;


	CXGroupBox m_sCommands;
	CXGroupBox m_sOutput;


	// visualization
	COScopeCtrl m_OScopeCtrl;
public:
	CComboBox m_cbCommands;
};
