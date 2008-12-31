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
// iperfView.h : interface of the CIperfView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPERFVIEW_H__F5C6C78D_A493_484C_AE48_7D4AC43E1F11__INCLUDED_)
#define AFX_IPERFVIEW_H__F5C6C78D_A493_484C_AE48_7D4AC43E1F11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CIperfView : public CView
{
protected: // create from serialization only
	CIperfView();
	DECLARE_DYNCREATE(CIperfView)

// Attributes
public:
	CIperfDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIperfView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIperfView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef WINCE //Windows Mobile -- berise
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CIperfView)
	afx_msg void OnFileIperf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in iperfView.cpp
inline CIperfDoc* CIperfView::GetDocument()
   { return (CIperfDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPERFVIEW_H__F5C6C78D_A493_484C_AE48_7D4AC43E1F11__INCLUDED_)
