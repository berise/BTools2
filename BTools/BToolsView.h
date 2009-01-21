// BToolsView.h : CBToolsView Ŭ������ �������̽�
//

#include "3rd/PropertyView.h"

#include "BTPingDialog.h"
//#include "BTThroughputDialog.h"
#include "BTIPerfClient.h"
#include "BTIPerfServer.h"

#pragma once

class CBToolsView : public CPropertyView
{
protected: // serialization������ ��������ϴ�.
	CBToolsView();
	DECLARE_DYNCREATE(CBToolsView)

// Ư���Դϴ�.
public:
	CBToolsDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:

// �����Դϴ�.
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



// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePing();
	afx_msg void OnFileIperfclient();
	afx_msg void OnFileIperfserver();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // BToolsView.cpp�� ����� ����
inline CBToolsDoc* CBToolsView::GetDocument() const
   { return reinterpret_cast<CBToolsDoc*>(m_pDocument); }
#endif

