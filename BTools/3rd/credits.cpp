//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// credits.cpp : implementation file
//**************************************************************************
// Note : 스크롤되는 화면에는 그림도 나타나게 할 수 있는데, 이 예제에서는 ID가 "BITMAP1" 인 비트맵 
//        하나를  사용했다. 여기서 주의할 점은 이 ID 에 따옴표도 포함된다는 것이다. 
//        ( "BITMAP1"   not   BITMAP1 ) 
//        그리고 pArrCredit[] 배열에 그림의 이름을 넣을 때 파일명과 escape sequence 사이에 빈 공간을 두면
//        안된다. ("BITMAP1\b" = 옳은 경우, "BITMAP1   \b" = 틀린 경우  )

#include "stdafx.h"
//#include "resource.h"
#include "credits.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define SCROLLAMOUNT	-1
#define DISPLAY_SLOW	70
#define DISPLAY_MEDIUM	40
#define DISPLAY_FAST	10
#define DISPLAY_SPEED	DISPLAY_MEDIUM

#define RED       RGB(255,0,0)
#define GREEN     RGB(0,255,0)
#define BLUE      RGB(0,0,255)
#define WHITE     RGB(255,255,255)
#define YELLOW    RGB(255,255,0)
#define TURQUOISE	RGB(0,255,255)
#define PURPLE    RGB(255,0,255)
#define BLACK     RGB(0,0,0)

#define BACKGROUND_COLOR         WHITE
#define TOP_LEVEL_TITLE_COLOR    RED
#define TOP_LEVEL_GROUP_COLOR    YELLOW
#define GROUP_TITLE_COLOR        TURQUOISE
#define NORMAL_TEXT_COLOR        BLACK

// 폰트의 높이를 정하는 부분
#define TOP_LEVEL_TITLE_HEIGHT    21   
#define TOP_LEVEL_GROUP_HEIGHT    19     
#define GROUP_TITLE_HEIGHT        17     
#define NORMAL_TEXT_HEIGHT        15

// 출력할 문자배열과 함께 쓰일 escape sequence를 정의하는 부분
// 큰 글자에서 작은 글자순으로 표시되어 있다. 
#define TOP_LEVEL_TITLE		'\n'
#define TOP_LEVEL_GROUP   '\r'
#define GROUP_TITLE       '\t'
#define NORMAL_TEXT       '\f' 
#define DISPLAY_BITMAP		'\b'

#define ARRAYCOUNT	14          // 출력될 문장의 갯수

// 출력될 문장을 고정배열에 넣는다.
char *pArrCredit[] = { 	"nfone Credits\n",
                        "",
                        "res\\IDB_BITMAP15\f",    // 비트맵 ID에는 따옴표도 포함되어 있다.
						"",
                        "Programmer\n",
						"Jason Lee\f",
						"",
						"Special Thanks to\t",
                        "benny pjo@pj\t",
						"",
                        "Members@KTN\t",
                        "HRLEE, WKPARK, YJCHOI, IKJANG, HCKIM\f",
                        "",
						""};

/////////////////////////////////////////////////////////////////////////////
// CCredits dialog

CCredits::CCredits(CWnd* pParent /*=NULL*/)
	: CDialog(CCredits::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCredits)
	//}}AFX_DATA_INIT
}

void CCredits::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCredits)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCredits, CDialog)
	//{{AFX_MSG_MAP(CCredits)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCredits message handlers

void CCredits::OnOK() 
{
	KillTimer(DISPLAY_TIMER_ID);
	CDialog::OnOK();
}

//************************************************************************
//	 InitDialog
//
//	 	Setup the display rect and start the timer.
//************************************************************************
BOOL CCredits::OnInitDialog() 
{
	CDialog::OnInitDialog();
	BOOL bRet;
    	UINT nRet;
	
	nCurrentFontHeight = NORMAL_TEXT_HEIGHT;
	
	CClientDC dc(this);
	bRet = m_dcMem.CreateCompatibleDC(&dc);
		
	m_bProcessingBitmap=FALSE;
	
	nArrIndex=0;
	nCounter=1;
	nClip=0;
	m_bFirstTime=TRUE;
	m_bDrawText=FALSE;
	m_hBmpOld = 0;
	
	m_pDisplayFrame=(CWnd*)GetDlgItem(IDC_DISPLAY_STATIC);	

	// If you assert here, you did not assign your static display control
	// the IDC_ value that was used in the GetDlgItem(...). This is the
  // control that will display the credits.
	_ASSERTE(m_pDisplayFrame);
				 
	m_pDisplayFrame->GetClientRect(&m_ScrollRect);


	nRet = SetTimer(DISPLAY_TIMER_ID,DISPLAY_SPEED,NULL);
	 _ASSERTE(nRet != 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
} 

//************************************************************************
//	 OnTimer
//
//	 	On each of the display timers, scroll the window 1 unit. Each 20
//      units, fetch the next array element and load into work string. Call
//      Invalidate and UpdateWindow to invoke the OnPaint which will paint 
//      the contents of the newly updated work string.
//************************************************************************
void CCredits::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != DISPLAY_TIMER_ID) {
		CDialog::OnTimer(nIDEvent);
		return;
	}

	if (!m_bProcessingBitmap)
	if (nCounter++ % nCurrentFontHeight == 0) {     // every x timer events, show new line
		nCounter=1;
		m_szWork = pArrCredit[nArrIndex++];
		if (nArrIndex > ARRAYCOUNT-1) nArrIndex=0;
		nClip = 0;
		m_bDrawText=TRUE;
		}
	
	m_pDisplayFrame->ScrollWindow(0,SCROLLAMOUNT,&m_ScrollRect,&m_ScrollRect);
	nClip = nClip + abs(SCROLLAMOUNT);	
	
    	CRect r;
	CWnd* pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
    	ASSERT_VALID(pWnd);
    	pWnd->GetClientRect(&r);
    	pWnd->ClientToScreen(r);
    	ScreenToClient(&r);
    	InvalidateRect(r,FALSE); // FALSE does not erase background

    	CDialog::OnTimer(nIDEvent);
}

//************************************************************************
//	 OnPaint
//
//	 	Send the newly updated work string to the display rect.
//************************************************************************
void CCredits::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	PAINTSTRUCT ps;
	CDC* pDc = m_pDisplayFrame->BeginPaint(&ps);
	
	pDc->SetBkMode(TRANSPARENT);

	//*********************************************************************
	//	FONT SELECTION
  CFont m_fntArial;
	CFont* pOldFont;
	BOOL bSuccess;
	
	BOOL bUnderline;
	BOOL bItalic;

	if (!m_szWork.IsEmpty())
	switch (m_szWork[m_szWork.GetLength()-1] ) {
	case NORMAL_TEXT:
	default:
    bItalic = FALSE;
		bUnderline = FALSE;
		nCurrentFontHeight = NORMAL_TEXT_HEIGHT;
    bSuccess = m_fntArial.CreateFont(NORMAL_TEXT_HEIGHT, 0, 0, 0, 
                                     FW_THIN, bItalic, bUnderline, 0, 
                                     ANSI_CHARSET,  OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS,  PROOF_QUALITY, 
                                     VARIABLE_PITCH | 0x04 | FF_DONTCARE,  (LPSTR)"Arial");                 
		pDc->SetTextColor(NORMAL_TEXT_COLOR);
		pOldFont  = pDc->SelectObject(&m_fntArial);
		break;

	case TOP_LEVEL_GROUP:
		bItalic = FALSE;
		bUnderline = FALSE;
		nCurrentFontHeight = TOP_LEVEL_GROUP_HEIGHT;
		bSuccess = m_fntArial.CreateFont(TOP_LEVEL_GROUP_HEIGHT, 0, 0, 0, 
                                     FW_BOLD, bItalic, bUnderline, 0,  ANSI_CHARSET,
                                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,  PROOF_QUALITY,                                       VARIABLE_PITCH | 0x04 | FF_DONTCARE, (LPSTR)"Arial");                               pDc->SetTextColor(TOP_LEVEL_GROUP_COLOR);
		pOldFont  = pDc->SelectObject(&m_fntArial);
		break;

	case GROUP_TITLE:
		bItalic = FALSE;
		bUnderline = FALSE;
		nCurrentFontHeight = GROUP_TITLE_HEIGHT;
		bSuccess = m_fntArial.CreateFont(GROUP_TITLE_HEIGHT, 0, 0, 0, 
                                     FW_BOLD, bItalic, bUnderline, 0, 
                                     ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                     VARIABLE_PITCH | 0x04 | FF_DONTCARE, (LPSTR)"Arial");           
		pDc->SetTextColor(GROUP_TITLE_COLOR);
		pOldFont  = pDc->SelectObject(&m_fntArial);
		break;
		
	case TOP_LEVEL_TITLE:
		bItalic = FALSE;
		bUnderline = TRUE;
		nCurrentFontHeight = TOP_LEVEL_TITLE_HEIGHT;
		bSuccess = m_fntArial.CreateFont(TOP_LEVEL_TITLE_HEIGHT, 0, 0, 0, 
                                     FW_BOLD, bItalic, bUnderline, 0,  
                                     ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS, PROOF_QUALITY,                       
                                     VARIABLE_PITCH | 0x04 | FF_DONTCARE,  (LPSTR)"Arial");                    pDc->SetTextColor(TOP_LEVEL_TITLE_COLOR);
		pOldFont  = pDc->SelectObject(&m_fntArial);
		break;
		
	case DISPLAY_BITMAP:
	        if (!m_bProcessingBitmap) {
	                CString szBitmap = m_szWork.Left(m_szWork.GetLength()-1);
	                 if (!m_bmpWork.LoadBitmap((const char *)szBitmap)) {
	                        CString str; 
	                        str.Format("Could not find bitmap resource \"%s\". "
                                     "Be sure to assign the bitmap a QUOTED resource name", szBitmap); 
	                        KillTimer(DISPLAY_TIMER_ID); 
	                        MessageBox(str); 
	                        return; 
	                   }
	                   m_bmpCurrent = &m_bmpWork;
	                   m_bmpCurrent->GetObject(sizeof(BITMAP), &m_bmpInfo);
			
                     m_size.cx = m_bmpInfo.bmWidth; // width of dest rect
	                   RECT workRect;
	                   m_pDisplayFrame->GetClientRect(&workRect);
	                   m_pDisplayFrame->ClientToScreen(&workRect);
	                   ScreenToClient(&workRect);
                     // upper left point of dest
	                   m_pt.x = (workRect.right - 
                            ((workRect.right-workRect.left)/2) - (m_bmpInfo.bmWidth/2));
	                   m_pt.y = workRect.bottom;
				
                     pBmpOld = m_dcMem.SelectObject(m_bmpCurrent);
                     if (m_hBmpOld == 0)   m_hBmpOld = (HBITMAP) pBmpOld->GetSafeHandle();
                     m_bProcessingBitmap = TRUE;
	            }
              break;
	     }

       CBrush bBrush(BACKGROUND_COLOR);
       CBrush* pOldBrush;
       pOldBrush  = pDc->SelectObject(&bBrush);
       // Only fill rect comprised of gap left by bottom of scrolling window
       r=m_ScrollRect;
       r.top = r.bottom-abs(SCROLLAMOUNT); 
       pDc->DPtoLP(&r);
	
       if (m_bFirstTime) {
            m_bFirstTime=FALSE;
            pDc->FillRect(&m_ScrollRect,&bBrush);

       } else
            pDc->FillRect(&r,&bBrush);
	
       r=m_ScrollRect;
       r.top = r.bottom-nClip;
		
       if (!m_bProcessingBitmap) {
            int x = pDc->DrawText((const char *)m_szWork, m_szWork.GetLength()-1, &r, 
                                   DT_TOP| DT_CENTER| DT_NOPREFIX | DT_SINGLELINE);	
            m_bDrawText=FALSE;
       
       } else {
            dc.StretchBlt( m_pt.x, m_pt.y-nClip, m_size.cx, nClip, &m_dcMem, 0, 0, 
                           m_bmpInfo.bmWidth-1, nClip, SRCCOPY );
            if (nClip > m_bmpInfo.bmHeight) {
                 m_bmpWork.DeleteObject();
                 m_bProcessingBitmap = FALSE;
                 nClip=0;
                 m_szWork.Empty();
                 nCounter=1;
            }
                 pDc->SelectObject(pOldBrush);
                 bBrush.DeleteObject();
                 m_pDisplayFrame->EndPaint(&ps);
                 return;
       }
  	   pDc->SelectObject(pOldBrush);
       bBrush.DeleteObject();
	
       if (!m_szWork.IsEmpty()) {
            pDc->SelectObject(pOldFont);
            m_fntArial.DeleteObject();
	     }
	     m_pDisplayFrame->EndPaint(&ps);
	     // Do not call CDialog::OnPaint() for painting messages
}

void CCredits::OnDestroy() 
{
      CDialog::OnDestroy();
	
    	m_dcMem.SelectObject(CBitmap::FromHandle(m_hBmpOld));
    	m_bmpWork.DeleteObject();
}

