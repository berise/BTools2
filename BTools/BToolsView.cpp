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

	AddPage(&bt_ping, L"iPing");
	AddPage(&bt_iperf_client, L"&iClient");
	AddPage(&m_iperf_server, L"&iServer");

	//EnableStackedTabs(TRUE);

	SetActivePage(2);

	return 0;
}
