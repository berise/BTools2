// NFMainView �޽��� ó�����Դϴ�.
// NFMainView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "NFone.h"
#include "NFMainView.h"
#include "nsua.h"	 // NFxx SUA


// NFMainView

IMPLEMENT_DYNCREATE(NFMainView, CPropertyView)

NFMainView::NFMainView()
:CPropertyView()
{

}

NFMainView::~NFMainView()
{
}

BEGIN_MESSAGE_MAP(NFMainView, CPropertyView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// NFMainView �׸����Դϴ�.

void NFMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// NFMainView �����Դϴ�.

#ifdef _DEBUG
void NFMainView::AssertValid() const
{
	CPropertyView::AssertValid();
}

#ifndef _WIN32_WCE
void NFMainView::Dump(CDumpContext& dc) const
{
	CPropertyView::Dump(dc);
}
#endif
#endif //_DEBUG


#ifdef _DEBUG
CNFoneDoc* NFMainView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNFoneDoc)));
	return (CNFoneDoc*)m_pDocument;
}
#endif //_DEBUG




// NFMainView �޽��� ó�����Դϴ�.

int NFMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	//AddPage(&m_log_dlg, "&Log");
	// Address, Organization, Hierarchy
	AddPage(&m_address, "&Address");

	SetActivePage(0); 	

	return 0;
}

void NFMainView::OnSize(UINT nType, int cx, int cy)
{
	CPropertyView::OnSize(nType, cx, cy);

	CRect r;
	GetClientRect(r);
	//m_log_dlg.MoveWindow(0, 20, r.Width(), r.Height());

	// TODO: Add your message handler code here
}

void NFMainView::OnDestroy()
{
	CPropertyView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	
}
