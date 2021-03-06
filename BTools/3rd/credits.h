// credits.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCredits dialog
#include "../resource.h"

#define     DISPLAY_TIMER_ID  150     // timer id

class CCredits : public CDialog
{
// Construction
public:
	CCredits(CWnd* pParent = NULL);   // standard constructor

  RECT  m_ScrollRect,r;             // rect of Static Text frame
  int nArrIndex,nCounter;           // work ints
  CString     m_szWork;             // holds display line
	BOOL       m_bFirstTime;
	BOOL       m_bDrawText;
	int         	nClip;
	int         	nCurrentFontHeight;
	CWnd*      m_pDisplayFrame;
  CBitmap    m_bmpWork;             // bitmap holder
  CBitmap*   pBmpOld;               // other bitmap work members
	CBitmap*    m_bmpCurrent;
	HBITMAP   m_hBmpOld;
  CSize      m_size;                // drawing helpers
	CPoint 	   m_pt;
	BITMAP 	   m_bmpInfo;
	CDC 	   m_dcMem;
	BOOL 	   m_bProcessingBitmap;

// Dialog Data
	//{{AFX_DATA(CCredits)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCredits)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCredits)
  virtual void OnOK();                 // <-----클래스 위저드에서 추가한 부분
  afx_msg void OnPaint();              // <-----클래스 위저드에서 추가한 부분
  virtual BOOL OnInitDialog();         // <-----클래스 위저드에서 추가한 부분
  afx_msg void OnTimer(UINT nIDEvent); // <-----클래스 위저드에서 추가한 부분
  afx_msg void OnDestroy();            // <-----클래스 위저드에서 추가한 부분
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


