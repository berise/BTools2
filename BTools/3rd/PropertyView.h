/*=============================================================================
 * This is an MFC based extension class.
 * Copyright (C) 2000 YEAsoft Int'l.
 * All rights reserved.
 *=============================================================================
 * Copyright (c) 2000 YEAsoft Int'l (Leo Moll, Andrea Pennelli).
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *=============================================================================
 * FILENAME		:	PropertyView.h
 * PURPOSE		:	Declaration of a "property sheet"-like view
 * SCOPE		:	Property View Classes
 * HISTORY		: =============================================================
 * 
 * $Log: PropertyView.h,v $
 * Revision 1.9  2001/06/05 10:54:08  leo
 * Impemented more compatible construction
 *
 * Revision 1.8  2001/04/18 08:50:55  leo
 * Made CPropertyView::PreCreateTabControl virtual
 *
 * Revision 1.7  2001/01/04 17:14:02  leo
 * Improved page name handling
 *
 * Revision 1.6  2000/12/20 14:02:56  leo
 * Made the base class of CPropertyView customisable
 *
 * Revision 1.5  2000/12/12 15:34:08  leo
 * Added scrolling support
 *
 * Revision 1.4  2000/09/28 16:15:26  leo
 * Implemented OnUpdate and OnInitialUpdate handlers
 *
 * Revision 1.3  2000/09/24 18:30:27  leo
 * Behaviour now emulates better a property sheet
 *
 * Revision 1.2  2000/09/14 16:57:50  leo
 * Added last implementation details
 *
 * Revision 1.1  2000/09/13 17:45:58  leo
 * Property Sheet like View Class
 *
 *============================================================================*/
#if !defined(AFX_PROPERTYVIEW_H__6650112D_6CB4_45F9_B1E9_A297ACD33DBB__INCLUDED_)
#define AFX_PROPERTYVIEW_H__6650112D_6CB4_45F9_B1E9_A297ACD33DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*=============================================================================
 * FORWARD CLASS DECLARATIONS
 *============================================================================*/
class CPropertyView;
class CPropertyViewPage;

/*=============================================================================
 * BASE CLASS DEFINITION
 *============================================================================*/
#ifndef baseCPropertyView
#define baseCPropertyView		CScrollView
#endif
#ifndef baseCPropertyViewPage
#define baseCPropertyViewPage	CDialog
#endif

/*=============================================================================
 * PROPERTY VIEW PAGE CLASS
 *============================================================================*/
class CPropertyViewPage : public baseCPropertyViewPage
{
	DECLARE_DYNAMIC				(CPropertyViewPage)

// Construction
public:
	CPropertyViewPage			();
	CPropertyViewPage			(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
#ifndef WINCE
	CPropertyViewPage			(UINT nIDTemplate, UINT nIDCaption = 0);
#else
	CPropertyViewPage			 (UINT nIDTemplate, UINT nIDCaption, CWnd *pParent);
#endif


// Dialog Data
	//{{AFX_DATA(CPropertyViewPage)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CPropertyView *				GetView					() const;

// Operations
public:
	void						Construct				(UINT nIDTemplate, UINT nIDCaption = 0);
	void						Construct				(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
	virtual void				SetModified				(BOOL bModified = TRUE);
	virtual BOOL				GetModified				() const { return m_bModified; }

// Overrides
public:
	virtual BOOL				OnSetActive				();
	virtual BOOL				OnKillActive			();
	virtual void				OnInitialUpdate			() { UpdateData (FALSE); OnUpdate (NULL, 0, NULL); }
	virtual void				OnUpdate				(CView* pSender, LPARAM lHint, CObject* pHint) { }
	virtual BOOL				OnApply					();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyViewPage)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void						CommonConstruct			(LPCTSTR lpszTemplateName, UINT nIDCaption);

	// Generated message map functions
	//{{AFX_MSG(CPropertyViewPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Implementation
protected:
	CPropertyView *				m_pView;
	BOOL						m_bModified;
	BOOL						m_bFirstSetActive;
	CSize						m_szMin;
	UINT						m_nIDCaption;
	friend class				CPropertyView;
};

/*=============================================================================
 * THE VIEW CLASS
 *============================================================================*/
class CPropertyView : public baseCPropertyView
{
// Construction
protected:
	CPropertyView				();
	DECLARE_DYNCREATE			(CPropertyView)

// Attributes
public:
	int							GetActiveIndex			() const;
	int							GetPageIndex			(CPropertyViewPage *pPage) const;
	int							GetPageCount			() const;
	CPropertyViewPage *			GetPage					(int nPage) const;
	CPropertyViewPage *			GetActivePage			() const { return GetPage (GetActiveIndex ()); }
	BOOL						SetActivePage			(int nPage);
	BOOL						SetActivePage			(CPropertyViewPage *pPage);
	CTabCtrl *					GetTabControl			() { return &m_TabCtrl; }
	CImageList *				GetImageList			() const;
	CImageList *				SetImageList			(CImageList *pImageList);
	void						EnableStackedTabs		(BOOL bStacked);
	void						EnableScrollView		(BOOL bScrollView);

	BOOL						SetPageTitle			(int nPage, UINT nID);
	BOOL						SetPageTitle			(int nPage, LPCTSTR pszTitle);
	BOOL						GetPageTitle			(int nPage, LPTSTR lpszStringBuf, int nMaxCount) const;

// Operations
public:
	BOOL						AddPage					(CPropertyViewPage *pPage, int nImage = -1) { return AddPage (pPage, (LPCTSTR) NULL, nImage); }
	BOOL						AddPage					(CPropertyViewPage *pPage, UINT nID, int nImage);
	BOOL						AddPage					(CPropertyViewPage *pPage, LPCTSTR pszTitle, int nImage = -1);
	BOOL						RemovePage				(CPropertyViewPage *pPage);
	BOOL						RemovePage				(int nPage);
	BOOL						PressButton				(int nButton) { if ( nButton == PSBTN_APPLYNOW ) return Apply (); return FALSE; }
	BOOL						Apply					();
	virtual void				SetModified				(BOOL bChanged = TRUE);


// Overrides
public:
	virtual void				PreCreateTabControl		(CRect &rect, DWORD &dwStyle) { }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPropertyView		();
#ifdef _DEBUG
	virtual void				AssertValid				() const;
//	virtual void				Dump					(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg void OnTabChanging(NMHDR * pNotifyStruct, LRESULT* result);
	afx_msg void OnTabChanged(NMHDR * pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

// Implementation
protected:
	void						EnablePage				(CPropertyViewPage *pPage, BOOL bEnable = TRUE);
	BOOL						RemovePage				(CPropertyViewPage *pPage, int iIndex);
	void						AdjustPagePosition		(CPropertyViewPage *pPage);

// Implementation
protected:
	BOOL						m_bAdjusted;
    CTabCtrl					m_TabCtrl;
	int							m_nActivePage;
	BOOL						m_bStacked;
	BOOL						m_bScrollView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYVIEW_H__6650112D_6CB4_45F9_B1E9_A297ACD33DBB__INCLUDED_)
//
// EoF
////////
