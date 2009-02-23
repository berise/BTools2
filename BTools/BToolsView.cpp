// BToolsView.cpp : CBToolsView Ŭ������ ����
//

#include "stdafx.h"
#include "BTools.h"

#include "BToolsDoc.h"
#include "BToolsView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBToolsView

IMPLEMENT_DYNCREATE(CBToolsView, CPropertyView)

BEGIN_MESSAGE_MAP(CBToolsView, CPropertyView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PING, &CBToolsView::OnFilePing)
	ON_COMMAND(ID_FILE_IPERFCLIENT, &CBToolsView::OnFileIperfclient)
	ON_COMMAND(ID_FILE_IPERFSERVER, &CBToolsView::OnFileIperfserver)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CBToolsView ����/�Ҹ�

CBToolsView::CBToolsView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CBToolsView::~CBToolsView()
{
}

BOOL CBToolsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	return CPropertyView::PreCreateWindow(cs);
}


// CBToolsView �׸���
void CBToolsView::OnDraw(CDC* /*pDC*/)
{
	CBToolsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}



// CBToolsView ����

#ifdef _DEBUG
#ifndef WINCE
void CBToolsView::AssertValid() const
{
	CPropertyView::AssertValid();
}
#endif

CBToolsDoc* CBToolsView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBToolsDoc)));
	return (CBToolsDoc*)m_pDocument;
}
#endif //_DEBUG


// CBToolsView �޽��� ó����

int CBToolsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	// Init CPropertyView 
	CTabCtrl *tab = GetTabControl();
	
	HINSTANCE hInstance = ::AfxGetApp()->m_hInstance;
	
	m_ImageList16x16.Create(16, 16, ILC_COLOR32|ILC_MASK, 6, 3);
	m_ImageList16x16.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PING)) ); // 
	m_ImageList16x16.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPERF_CLIENT)) );
	m_ImageList16x16.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPERF_SERVER)) );
	m_ImageList16x16.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ABOUT1)) );
	m_ImageList16x16.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ABOUT2)) );

	/*
	m_ImageList32x32.Create(32, 32, ILC_COLOR32|ILC_MASK, 6, 3);
	m_ImageList32x32.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BTOOLS_32x32)) ); // IDI_PING
	m_ImageList32x32.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPERF_CLIENT_32)) );
	m_ImageList32x32.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPERF_SERVER_32)) );
	m_ImageList32x32.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ABOUT1)) );
	m_ImageList32x32.Add( ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ABOUT2)) );
	*/

	tab->SetImageList(&m_ImageList16x16);


	// Windows Mobile 6.1.4 Professional������ CPropertyView::AdjustPagePosition��
	// ScrollSize���� ������ �߻��Ѵ�. 
	// ���� : SetScrollSize�� ���ڷ� pPage->m_szMin �� ���� �ʹ� ũ��
	// �ذ� : �Ʒ��� ���� ScrollView�� FALSE�� ó��
	// ���� : ���� ����(WM 5.0, 6.0������ ��� ������ ������ �ʴ´�. ���ϴ� ������ ó���Ǵ� �� ���δ�.
	EnableScrollView(FALSE);

	AddPage(&m_ping, L"&Ping", 0);
	AddPage(&m_iperf_client, L"&Client", 1);
	AddPage(&m_iperf_server, L"&Server", 2);

	//EnableStackedTabs(TRUE); // ���� �Ȱ� ����
	SetActivePage(0);

	return 0;
}

void CBToolsView::OnFilePing()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	SetActivePage(0);
}

void CBToolsView::OnFileIperfclient()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	SetActivePage(1);
}

void CBToolsView::OnFileIperfserver()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	SetActivePage(2);
}


/*!
	\brief AddPage�� �� �Ϳ� ���� ���� ������ �Ѵ�. �׷��� ������,
	�Ʒ� ��� ��Ÿ����. �̸� �����ϱ� ���ؼ��� ������ ������ RemovePage�ؾ� �Ѵ�.

	<pre>
	Warning: calling DestroyWindow in CDialog::~CDialog --
	OnDestroy or PostNcDestroy in derived class will not be called


	
PRB: "Warning: Calling DestroyWindow in CWnd::~CWnd" Message
View products that this article applies to.
Article ID 	: 	105081
Last Review 	: 	November 21, 2006
Revision 	: 	2.1
This article was previously published under Q105081
SYMPTOMS
The following warnings are displayed when a debug version of a program is executed and TRACE() diagnostics are enabled:

   Warning: calling DestroyWindow in CWnd::~CWnd
   OnDestroy or PostNcDestroy in derived class will not be called
				

Back to the top
CAUSE
The above warnings are displayed by the CWnd::~CWnd() destructor. The warnings indicate the following:

? 	The programmer has most likely called "delete" on a CWnd object rather than DestroyWindow().
? 	An object derived from CWnd is not performing a DestroyWindow() call in its destructor.
? 	Because the DestroyWindow() call is executed in the destructor for CWnd, only the CWnd portion of the CWnd-derived object is remaining when the DestroyWindow() is called. Thus, if you have a CWnd-derived object and receive this warning, the derived object's virtual functions will not exist and the CWnd functions will be called instead.

Back to the top
RESOLUTION
MFC Technote #17 contains more information on how to properly destroy a Window object and states the following cardinal rule:

   To destroy a C++ Windows object, use DestroyWindow, not "delete".
				


Also, examining CWnd::~CWnd() will reveal that the foremost task of the destructor is to ensure that the associated Windows window object has been destroyed. If a program calls DestroyWindow() before the CWnd destructor is called, the m_hWnd data member will be NULL, the TRACE0() messages will not occur, and DestroyWindow() will not be called in CWnd::~CWnd(). 

	</pre>
*/
void CBToolsView::OnDestroy()
{
	CPropertyView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	/*
	RemovePage(&m_ping);
	RemovePage(&m_iperf_client);
	RemovePage(&m_iperf_server);
	*/
	int nCount = GetPageCount();

	for(int i = 0; i < nCount; i++)
		RemovePage(0);

	
}
