// BTools.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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



// CBToolsApp ����
CBToolsApp::CBToolsApp()
	: CWinApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CBToolsApp ��ü�Դϴ�.
CBToolsApp theApp;

// CBToolsApp �ʱ�ȭ

BOOL CBToolsApp::InitInstance()
{
    // CAPEDIT �� SIPPREF�� ���� ��� Windows Mobile ���� ��Ʈ���� �ʱ�ȭ�ϱ� ����
    // ���� ���α׷��� �ʱ�ȭ�ϴ� ���� SHInitExtraControls�� �� �� ȣ���ؾ� �մϴ�.
    SHInitExtraControls();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBToolsDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CBToolsView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

	return TRUE;	// ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
			// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CBToolsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CAboutDlg::OnAboutmenu2Scroll2()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	srand(GetTickCount()); // in CE use GetTickCount instead of time(null)

	int direction = rand() % 4;
	m_matrix2.DoScroll(200, direction);//CMatrixStatic::RIGHT);

	direction = rand() % 4;
	m_matrix1.DoScroll(2000, direction);//CMatrixStatic::RIGHT);
}

//void CAboutDlg::OnAboutmenuStop()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	m_matrix2.StopScroll();
//}

void CAboutDlg::OnAboutmenu2Stop()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_matrix1.StopScroll();
	m_matrix2.StopScroll();
}
