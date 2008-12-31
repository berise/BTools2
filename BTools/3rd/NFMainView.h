#pragma once
#include "NFoneDoc.h"
#include "ui/PropertyView.h"

#include "NFAddress.h"


// NFMainView ���Դϴ�.

class NFMainView : public CPropertyView
{
	DECLARE_DYNCREATE(NFMainView)

protected:
	NFMainView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~NFMainView();

	// Ư���Դϴ�.
public:
	CNFoneDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:

	NFAddress	m_address;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnDestroy();
};


#ifndef _DEBUG  // NFoneView.cpp�� ����� ����
inline CNFoneDoc* NFMainView::GetDocument() const
   { return reinterpret_cast<CNFoneDoc*>(m_pDocument); }
#endif
