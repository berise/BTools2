#pragma once
#include "resourceppc.h"
#include "3rd/PropertyView.h"
#include "afxwin.h"

class CPingThread;



typedef struct CPingOption_
{ 
	CString m_sHostToResolve;
	CString m_sLocalBoundAddress; 
	BOOL    m_bResolveAddressesToHostnames; 
	BOOL    m_bPingTillStopped; 
	int     m_nRequestsToSend; 
	UCHAR   m_nTTL; 
	UCHAR   m_nTOS; 
	WORD    m_wDataRequestSize; 
	DWORD   m_dwTimeout; 
	BOOL    m_bUseRawSockets; 
	BOOL    m_bDontFragment;
} CPingOption;




// BTPingDialog 대화 상자입니다.

class BTPingDialog : public CPropertyViewPage
{
	DECLARE_DYNAMIC(BTPingDialog)

public:
	BTPingDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~BTPingDialog();

public:
	CPingThread *ping_thread;
	CPingOption opt;
	BOOL is_host_alive;


// 대화 상자 데이터입니다.
	enum { IDD = IDD_PING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ping_result;
	CComboBox m_ping_host;
	afx_msg void OnBnClickedDoPing();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
