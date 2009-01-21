// BToolsView.h : CBToolsView 클래스의 인터페이스
//

#include "3rd/PropertyView.h"

#include "BTPingDialog.h"
//#include "BTThroughputDialog.h"
#include "BTIPerfClient.h"
#include "BTIPerfServer.h"

#pragma once

class CBToolsView : public CPropertyView
{
protected: // serialization에서만 만들어집니다.
	CBToolsView();
	DECLARE_DYNCREATE(CBToolsView)

// 특성입니다.
public:
	CBToolsDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:

// 구현입니다.
public:
	virtual ~CBToolsView();
#ifdef _DEBUG
#ifndef WINCE
	virtual void AssertValid() const;
#endif  // WINCE
#endif


public:
	CImageList	m_ImageList16x16;
	BTPingDialog	m_ping;
	//BTThroughputDialog bt_throughput;
	BTIPerfClient m_iperf_client;
	BTIPerfServer m_iperf_server;


protected:



// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePing();
	afx_msg void OnFileIperfclient();
	afx_msg void OnFileIperfserver();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // BToolsView.cpp의 디버그 버전
inline CBToolsDoc* CBToolsView::GetDocument() const
   { return reinterpret_cast<CBToolsDoc*>(m_pDocument); }
#endif

