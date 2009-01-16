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

	// Windows Mobile 6.1.4 Professional������ CPropertyView::AdjustPagePosition��
	// ScrollSize���� ������ �߻��Ѵ�. 
	// ���� : SetScrollSize�� ���ڷ� pPage->m_szMin �� ���� �ʹ� ũ��
	// �ذ� : �Ʒ��� ���� ScrollView�� FALSE�� ó��
	// ���� : ���� ����(WM 5.0, 6.0������ ��� ������ ������ �ʴ´�. ���ϴ� ������ ó���ȴ�.
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
