#pragma once

#include "3rd/PropertyView.h"



// BTThroughputDialog 대화 상자입니다.

class BTThroughputDialog : public CPropertyViewPage
{
	DECLARE_DYNAMIC(BTThroughputDialog)

public:
	BTThroughputDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~BTThroughputDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_THROUGHPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
