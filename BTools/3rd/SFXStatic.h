// Copyright (C) 2009 by berise@gmail.com
// All rights reserved
//
// This is free software.
// This code may be used in compiled form in any way you desire. This  
// file may be redistributed unmodified by any means PROVIDING it is   
// not sold for profit without the authors written consent, and   
// providing that this notice and the authors name and all copyright   
// notices remains intact. If the source code in this file is used in   
// any  commercial application then a statement along the lines of   
// "Portions Copyright ?2002 Nic Wilson MUST be included in   
// the startup banner, "About" dialog & printed documentation.   
//
// No warrantee of any kind, expressed or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Release Date and Version:

#if !defined(SFX_STATIC__INCLUDED_)
#define SFX_STATIC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


	
#include "CWaterRoutine.h"
#include "CFireRoutine.h"
#include "CPlasmaRoutine.h"

#include "DIBSectionLite.h"


class CWaterRoutine;
class CFireRoutine;
class CPlasmaRoutine;

/////////////////////////////////////////////////////////////////////////////
// CSFXStatic window
// self drawing special effects.
// Simply drop a static control on dialog and replace it with CSFXStatic.
// new a effect routine & enjoy.
class CSFXStatic : public CStatic
{
// Construction
public:
	CSFXStatic();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	int Run(UINT nMilliSecond);
	BOOL Stop();

	int m_hTimer;

// Attributes
public:
	CWaterRoutine	*m_Water;
	CFireRoutine	*m_Fire;
	CPlasmaRoutine	*m_Plasma;

	CDIBSectionLite *m_src;
	CDIBSectionLite *m_dest;

// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSFXStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetWaterRoutine(CWaterRoutine *p);
	void SetFireRoutine(CFireRoutine *p);
	void SetPlasmaRoutine(CPlasmaRoutine *p);

	void SetBitmapResource(UINT bitmap);

	virtual ~CSFXStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSFXStatic)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(SFX_STATIC__INCLUDED_)
