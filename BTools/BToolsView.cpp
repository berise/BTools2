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

	// Windows Mobile 6.1.4 Professional에서는 CPropertyView::AdjustPagePosition의
	// ScrollSize에서 오류가 발생한다. 
	// 원인 : SetScrollSize의 인자로 pPage->m_szMin 의 값이 너무 크다
	// 해결 : 아래와 같이 ScrollView를 FALSE로 처리
	// 참고 : 이전 버전(WM 5.0, 6.0에서는 모두 문제가 생기지 않는다. 피하는 것으로 처리된다.
	EnableScrollView(FALSE);

	AddPage(&bt_ping, L"iPing");
	AddPage(&bt_iperf_client, L"&iClient");
	AddPage(&m_iperf_server, L"&iServer");

	//EnableStackedTabs(TRUE);

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
