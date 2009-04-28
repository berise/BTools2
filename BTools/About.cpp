#include "stdafx.h"
#include "About.h"
#include "../btools_version.h"


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
#ifdef _DEVICE_RESOLUTION_AWARE
	ON_WM_SIZE()
#endif
	ON_COMMAND(ID_ABOUTMENU2_SCROLL2, &CAboutDlg::OnAboutmenu2Scroll2)
	ON_COMMAND(ID_ABOUTMENU2_STOP, &CAboutDlg::OnAboutmenu2Stop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD){}
CAboutDlg::~CAboutDlg(){}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MATRIX1, m_matrix1);
	DDX_Control(pDX, IDC_MATRIX2, m_matrix2);
	DDX_Control(pDX, IDC_STATIC_CONTEST_LOGO, m_cSFX);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_cVersion);
}


//TODO:
//IDEA: changing msg on the fly. OnTimer callback changes msg. ^^
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//The second way. Using SHCreateMenuBar function.
	SHMENUBARINFO info;
	memset(&info, 0, sizeof(info));
	info.cbSize = sizeof(info);
	info.hwndParent = m_hWnd;
	info.dwFlags = 0;
	info.nToolBarId = IDR_ABOUTDLG_MENU;//IDR_MAINFRAME;//IDR_ABOUTDLG_MENU;
	info.dwFlags = SHCMBF_HMENU;
	info.hInstRes = GetModuleHandle(NULL);//::AfxGetInstanceHandle();
	info.nBmpId = 0;
	info.cBmpImages = 0;
	SHCreateMenuBar(&info);

	/*
	// Configure the menu
  SHMENUBARINFO mbi;
  memset(&mbi, 0, sizeof(mbi));
  mbi.cbSize = sizeof(mbi);
  mbi.hWndParent = hWnd; // the dialog's handle
  mbi.nToolBarId = IDR_MENU; // the menu resource id
  mbi.hInstRes = GetModuleHandle(NULL);
  mbi.dwFlags = SHCMBF_HMENU;
 
  // Create the menu
  SHCreateMenuBar(&mbi);
  */
	
	//TODO:String Flipping must be implemented by other method, not here. It's just an idea for future feature.
	// initialize matrix 1
	// Your must call SetResourceBitmap, SetSize before SetText

	int n_blue_lines = 11;		// determine number matrix lines
	int n_blue_char_per_line = 22;	//	number of chars in line

	m_matrix1.SetNumberOfLines(n_blue_lines);
	m_matrix1.SetXCharsPerLine(n_blue_char_per_line);
	m_matrix1.SetSize(CMatrixStatic::TINY );
	m_matrix1.SetBitmapResource(IDB_MATRIX_TINY_BLUE);
	m_matrix1.AdjustClientXToSize(n_blue_char_per_line);
	m_matrix1.AdjustClientYToSize(n_blue_lines);	
	//
	//m_matrix1.SetText(_T(" !\"#$%&'()*+,-./0123456789;:<=>?@ABCDEFGHIJKLMNO PQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
	//				 0123456789012345678901
	CString msg(	"+--------------------+"//0
					"|BTools for Pocket PC|"//1
					"| - WM 5, WM6.x      |"//2
					"|-------------------+|"//3//4
					"| - Iperf 1.7.0      |"//5//6
					"|------+-----+-------|"//8
					"|Copyright(R)|Elberet|"//9
					"| All Rights |      h|"//0
					"|  Reserved  +      @|"//1
					"+--------------+-----+");//2

	/*
	int n_blue_lines = 15;		// determine number matrix lines
	int n_blue_char_per_line = 44;	//	number of chars in line

	m_matrix1.SetNumberOfLines(n_blue_lines);
	m_matrix1.SetXCharsPerLine(n_blue_char_per_line);
	m_matrix1.SetSize(CMatrixStatic::TINY );
	m_matrix1.SetBitmapResource(IDB_MATRIX_TINY_BLUE);
	m_matrix1.AdjustClientXToSize(n_blue_char_per_line);
	m_matrix1.AdjustClientYToSize(n_blue_lines);
	// about message for real device(M490)
	CString msg(	"+------------------------------------------+"//0
					"|BTools for Pocket PC                      |"//1
					"|Limited Version for Windows Mobile        |"//2
					"|  Contest for T*Omnia                     |"//3
					"|---------------------+--------------------|"//5 4
					"| Features ;                               |"//6
					"|  = ping with visualization               |"//7
					"|  = Iperf for  Windows Mobile             |"//8
					"|     (based on iperf version 1.7.0)       |"//9		
					"|------+------+----------------------------|"//0
					"|Copyrights(R)|        ????????            |"//1
					"| All Rights  |                            |"//2
					"| Reserved    +berise _@_ gmail _dot_ com >|"//3
					"+--------------+---------------------------+");//4

					/*
	int nLine = 9;
	int nChar = 44;
	
	m_matrix2.SetNumberOfLines(nLine);
	m_matrix2.SetXCharsPerLine(nChar);
	m_matrix2.SetSize(CMatrixStatic::TINY);
	m_matrix2.SetBitmapResource(IDB_MATRIX_TINY_RED);
	m_matrix2.AdjustClientXToSize(nChar);
	m_matrix2.AdjustClientYToSize(nLine);
	CString msg2(	"+--------------+---------------------------+"
					"|Reporting bug would be really appreciated.|"
					"|=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_|"
					"|Do you know that you can drag the T*Omnia?|"
					"+------------------------------------------+"
					"                                            "
					"berise the developer                        "
					"Dlvl;54 $;-9M HP;3(7) Pw;4(42) AC;-9 Exp;16 "// index for ';', ':' is misplaced
					"                                            "// ';' shows ':' and ':' shows ';'
					);
		*/
	

	// Uncomment next code to test flicker free blitting
	// It looks like working with MemDC.h !o!
	//m_matrix1.DoScroll(50, CMatrixStatic::DOWN);

	int nLine = 3;
	int nChar = 22;
	
	m_matrix2.SetNumberOfLines(nLine);
	m_matrix2.SetXCharsPerLine(nChar);
	m_matrix2.SetSize(CMatrixStatic::TINY);
	m_matrix2.SetBitmapResource(IDB_MATRIX_TINY_RED);
	m_matrix2.AdjustClientXToSize(nChar);
	m_matrix2.AdjustClientYToSize(nLine);

	//				 0123456789012345678901
	CString msg2(	//"+--------------+-----+"					
					//"|Do you know that you|"
					//"|can drag the logo   |"
					//"+--------------------+"
					//"                      "
					"berise the developer  "
					"Dlvl;54 $;99 HP;68(97)"
					"Pw;54(72) AC;-9 Exp;16");


	m_matrix1.SetText(msg);
	m_matrix2.SetText(msg2);

	// SFX
	m_cSFX.SetBitmapResource(IDB_BITMAP1);// no effects on their logos
	//m_cSFX2.SetBitmapResource(IDB_OMNIA);

	// Start effect 1 seconds later
	SetTimer(999, 1000, NULL); 

	CString szVersion;
	szVersion.Format(_T("iperf for Windows Mobile\nBTools version 0.9."));
	szVersion += BTOOLS_SVN_VERSION;
	m_cVersion.SetWindowText(szVersion);
	
	return TRUE;	// 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
			// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



#ifdef _DEVICE_RESOLUTION_AWARE
void CAboutDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	/*
	if (AfxIsDRAEnabled())
    	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_ABOUTBOX_WIDE) : MAKEINTRESOURCE(IDD_ABOUTBOX));
	}
	*/
	VerticalSpace(m_hWnd, IDC_MATRIX1, IDC_MATRIX2, -20);
	
	// place IDC_STATIC_CONTEST_LOGO just below IDC_MATRIX2
	VerticalSpace(m_hWnd, IDC_MATRIX2, IDC_STATIC_CONTEST_LOGO,  -2);
	VerticalSpace(m_hWnd, IDC_MATRIX2, IDC_STATIC_VERSION,  2);
	
	
}
#endif



void CAboutDlg::OnAboutmenu2Scroll2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	int direction = rand() % 4;
	m_matrix2.DoScroll(2000, direction);//CMatrixStatic::RIGHT);

	direction = rand() % 4;
	m_matrix1.DoScroll(5000, direction);//CMatrixStatic::RIGHT);

}

//void CAboutDlg::OnAboutmenuStop()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	m_matrix2.StopScroll();
//}

void CAboutDlg::OnAboutmenu2Stop()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_matrix1.StopScroll();
	m_matrix2.StopScroll();
}
void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == 999)
	{
		m_cSFX.Run(50);
		KillTimer(nIDEvent); // for one time timer.
	}

	CDialog::OnTimer(nIDEvent);
}
