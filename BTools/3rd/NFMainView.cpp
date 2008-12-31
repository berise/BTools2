// NFMainView 메시지 처리기입니다.
// NFMainView.cpp : 구현 파일입니다.
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


// NFMainView 그리기입니다.

void NFMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// NFMainView 진단입니다.

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
CNFoneDoc* NFMainView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNFoneDoc)));
	return (CNFoneDoc*)m_pDocument;
}
#endif //_DEBUG




// NFMainView 메시지 처리기입니다.

int NFMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertyView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	
}
