#pragma once
#include "NFoneDoc.h"
#include "ui/PropertyView.h"

#include "NFAddress.h"


// NFMainView 뷰입니다.

class NFMainView : public CPropertyView
{
	DECLARE_DYNCREATE(NFMainView)

protected:
	NFMainView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~NFMainView();

	// 특성입니다.
public:
	CNFoneDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
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


#ifndef _DEBUG  // NFoneView.cpp의 디버그 버전
inline CNFoneDoc* NFMainView::GetDocument() const
   { return reinterpret_cast<CNFoneDoc*>(m_pDocument); }
#endif
