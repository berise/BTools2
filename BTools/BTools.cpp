// BTools.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "MainFrm.h"

#include "BToolsDoc.h"
#include "BToolsView.h"
#include "afxwin.h"

#include "3rd/MatrixStatic.h" // AboutDlg

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBToolsApp

BEGIN_MESSAGE_MAP(CBToolsApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CBToolsApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()



// CBToolsApp 생성
CBToolsApp::CBToolsApp()
	: CWinApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CBToolsApp 개체입니다.
CBToolsApp theApp;

// CBToolsApp 초기화

BOOL CBToolsApp::InitInstance()
{
    // CAPEDIT 및 SIPPREF와 같은 모든 Windows Mobile 관련 컨트롤을 초기화하기 위해
    // 응용 프로그램을 초기화하는 동안 SHInitExtraControls를 한 번 호출해야 합니다.
    SHInitExtraControls();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBToolsDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CBToolsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
#ifdef _DEVICE_RESOLUTION_AWARE
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CMatrixStatic m_matrix1;
	CMatrixStatic m_matrix2;
	afx_msg void OnAboutmenu2Scroll2();
//	afx_msg void OnAboutmenuStop();
	afx_msg void OnAboutmenu2Stop();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MATRIX1, m_matrix1);
	DDX_Control(pDX, IDC_MATRIX2, m_matrix2);
}


//TODO:
//IDEA: What about changing msg on the fly. OnTimer callback changes msg. ^^
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





	
	// initialize matrix 1
	// Your must call SetResourceBitmap, SetSize before SetText
	int n_blue_lines = 14;		// determine number matrix lines
	int n_blue_char_per_line = 22;	//	number of chars in line

	m_matrix1.SetNumberOfLines(n_blue_lines);
	m_matrix1.SetXCharsPerLine(n_blue_char_per_line);
	m_matrix1.SetSize(CMatrixStatic::TINY );
	m_matrix1.SetBitmapResource(IDB_MATRIX_TINY_BLUE);
	m_matrix1.AdjustClientXToSize(n_blue_char_per_line);
	m_matrix1.AdjustClientYToSize(n_blue_lines);

	//TODO:String Flipping must be implemented by other method, not here. It's just an idea for future feature.
	//
	static bool bFlip = false;

	//m_matrix1.SetText(_T(" !\"#$%&'()*+,-./0123456789;:<=>?@ABCDEFGHIJKLMNO PQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
	//				01234567890123456789012345
	CString msg(	"+--------------------+"//0
					"|BTools for Pocket PC|"//1
					"| = Windows Mobile V |"//2
					"| = Windows Mobile VI|"//3
					"|-------------------+|"//5 4
					"| = ping             |"//6
					"| = Iperf client &   |"//7
					"|   server based on  |"//8
					"|   version I.VII.O  |"//9				
					"|------+-----+-------|"//0
					"|Copyright(R)|       |"//1
					"| All Rights |       |"//2
					"|  Reserved  +      @|"//3
					"+--------------------+");//4

	CString msg_f(	"+--------------------+"//0
					"|btOOLS FOR          |"//1
					"|   wINDOWS mOBILE 5 |"//2
					"|   wINDOWS mOBILE 6 |"//3
					"|-------------------+|"//5
					"| = ping             |"//6
					"| = Iperf client &   |"//7
					"|   server based on  |"//8
					"|   version I.VII.O  |"//9		
					"|                    |"//9				
					"|------+-----+-------|"//0					
					"|cOPYRIGHT(r)|ELBERET|"//1
					"| aLL rIGHTS |Elberet|"//2
					"|  rESERVED  +NoSpac@|"//3
					"+=-=-=-=-=-=-=-=-=-=-+");//4

	if(bFlip == false)
	{
		m_matrix1.SetText(msg);
		bFlip = true;
	}
	else
	{
		m_matrix1.SetText(msg_f);
		bFlip = false;
	}

	// Uncomment next code to test flicker free blitting
	// It looks like working with MemDC.h !o!
	//m_matrix1.DoScroll(50, CMatrixStatic::DOWN);


	CRect mr;
	m_matrix1.GetClientRect(mr);
	
	//ClientToScreen(&mr);
	m_matrix2.MoveWindow(0, mr.bottom-4, mr.Width(), 20);// matrix2 will be adjusted by AdjustClientX[Y]Zie function.
	m_matrix2.SetNumberOfLines(5);
	m_matrix2.SetXCharsPerLine(22);
	m_matrix2.SetSize(CMatrixStatic::TINY);
	m_matrix2.SetBitmapResource(IDB_MATRIX_TINY_RED);
	m_matrix2.AdjustClientXToSize(22);
	m_matrix2.AdjustClientYToSize(5);
	//m_matrix2.SetText(_T(" !\"#$%&'()*+,-./0123456789;:<=>?@ABCDEFGHIJKLMNO PQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
	CString msg2(	"berise the developer  "
					"Dlvl;54 $;-99 HP;3(17)" // index for ';', ':' is mismatched in MatrixStatic.cpp
					"Pw;34(42) AC;-9 Exp;16"	// ';' shows ':' and ':' shows ';'.
					"* Limited Version of  "
					" BTools for WMCONTEST!"
					);
	m_matrix2.SetText(msg2);
	//m_matrix2.DoScroll(5000, CMatrixStatic::RIGHT);

	return TRUE;	// 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
			// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
#ifdef _DEVICE_RESOLUTION_AWARE
	ON_WM_SIZE()
#endif
	ON_COMMAND(ID_ABOUTMENU2_SCROLL2, &CAboutDlg::OnAboutmenu2Scroll2)
//	ON_COMMAND(ID_ABOUTMENU_STOP, &CAboutDlg::OnAboutmenuStop)
ON_COMMAND(ID_ABOUTMENU2_STOP, &CAboutDlg::OnAboutmenu2Stop)
END_MESSAGE_MAP()

#ifdef _DEVICE_RESOLUTION_AWARE
void CAboutDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
    	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_ABOUTBOX_WIDE) : MAKEINTRESOURCE(IDD_ABOUTBOX));
	}
}
#endif

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CBToolsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CAboutDlg::OnAboutmenu2Scroll2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	srand(GetTickCount()); // in CE use GetTickCount instead of time(null)

	int direction = rand() % 4;
	m_matrix2.DoScroll(200, direction);//CMatrixStatic::RIGHT);

	direction = rand() % 4;
	m_matrix1.DoScroll(2000, direction);//CMatrixStatic::RIGHT);
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
