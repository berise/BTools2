/*
 * Copyright (c) 2006 Rutgers, The State University of New Jersey
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that (i) the above copyright notice and this permission notice
 * appear in all copies of the software and related documentation. (ii)
 * the name of Rutgers University may not be used in any advertising
 * or publicity relating to the software without the specific, prior
 * written permission of Rutgers University.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL RUTGERS UNIVERSITY BE LIABLE FOR ANY SPECIAL,
 * INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY
 * THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
// iperfView.cpp : implementation of the CIperfView class
//

#include "stdafx.h"
#include "iperf.h"

#include "iperfDoc.h"
#include "iperfView.h"
#include "IperfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIperfView

IMPLEMENT_DYNCREATE(CIperfView, CView)

BEGIN_MESSAGE_MAP(CIperfView, CView)
	//{{AFX_MSG_MAP(CIperfView)
	ON_COMMAND(ID_FILE_IPERF, OnFileIperf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIperfView construction/destruction

CIperfView::CIperfView()
{
	// TODO: add construction code here

}

CIperfView::~CIperfView()
{
}

BOOL CIperfView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CIperfView drawing

void CIperfView::OnDraw(CDC* pDC)
{
	CIperfDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CIperfView diagnostics

#ifdef _DEBUG
void CIperfView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef WINCE //Windows Mobile 5.0 patch
void CIperfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif


CIperfDoc* CIperfView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIperfDoc)));
	return (CIperfDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIperfView message handlers

void CIperfView::OnFileIperf() 
{
	// TODO: Add your command handler code here
	CIperfDlg idlg;
	idlg.DoModal();
}
