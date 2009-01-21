#pragma once
#include "resourceppc.h"

#include "afxwin.h"

#include "3rd/PropertyView.h"
#include "3rd/OScopeCtrl.h"
#include "3rd/XGroupBox.h"

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




// BTPingDialog ��ȭ �����Դϴ�.

class BTPingDialog : public CPropertyViewPage
{
	DECLARE_DYNAMIC(BTPingDialog)

public:
	BTPingDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~BTPingDialog();

public:
	CPingThread *ping_thread;
	CPingOption opt;
	BOOL is_host_alive;

	// visualization
	COScopeCtrl m_OScopeCtrl;


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox	m_lbPingResult;
	CComboBox m_cbHosts;
	CXGroupBox m_xgroup1;
	CXGroupBox m_sHost;


	afx_msg void OnBnClickedDoPing();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	
};
