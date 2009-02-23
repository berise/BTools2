// BToolsView.cpp : CBToolsView 클래스의 구현
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

// CBToolsView 생성/소멸

CBToolsView::CBToolsView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CBToolsView::~CBToolsView()
{
}

BOOL CBToolsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	return CPropertyView::PreCreateWindow(cs);
}


// CBToolsView 그리기
void CBToolsView::OnDraw(CDC* /*pDC*/)
{
	CBToolsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}



// CBToolsView 진단

#ifdef _DEBUG
#ifndef WINCE
void CBToolsView::AssertValid() const
{
	CPropertyView::AssertValid();
}
#endif

CBToolsDoc* CBToolsView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBToolsDoc)));
	return (CBToolsDoc*)m_pDocument;
}
#endif //_DEBUG


// CBToolsView 메시지 처리기

int CBToolsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

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


	// Windows Mobile 6.1.4 Professional에서는 CPropertyView::AdjustPagePosition의
	// ScrollSize에서 오류가 발생한다. 
	// 원인 : SetScrollSize의 인자로 pPage->m_szMin 의 값이 너무 크다
	// 해결 : 아래와 같이 ScrollView를 FALSE로 처리
	// 참고 : 이전 버전(WM 5.0, 6.0에서는 모두 문제가 생기지 않는다. 피하는 것으로 처리되는 듯 보인다.
	EnableScrollView(FALSE);

	AddPage(&m_ping, L"&Ping", 0);
	AddPage(&m_iperf_client, L"&Client", 1);
	AddPage(&m_iperf_server, L"&Server", 2);

	//EnableStackedTabs(TRUE); // 보기 싫게 보임
	SetActivePage(0);

	return 0;
}

void CBToolsView::OnFilePing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetActivePage(0);
}

void CBToolsView::OnFileIperfclient()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetActivePage(1);
}

void CBToolsView::OnFileIperfserver()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SetActivePage(2);
}


/*!
	\brief AddPage를 한 것에 대한 쌍을 만들어야 한다. 그렇지 않으면,
	아래 경고가 나타난다. 이를 제거하기 위해서는 적당한 곳에서 RemovePage해야 한다.

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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	/*
	RemovePage(&m_ping);
	RemovePage(&m_iperf_client);
	RemovePage(&m_iperf_server);
	*/
	int nCount = GetPageCount();

	for(int i = 0; i < nCount; i++)
		RemovePage(0);

	
}
