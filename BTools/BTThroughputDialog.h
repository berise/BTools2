#pragma once

#include "3rd/PropertyView.h"



// BTThroughputDialog ��ȭ �����Դϴ�.

class BTThroughputDialog : public CPropertyViewPage
{
	DECLARE_DYNAMIC(BTThroughputDialog)

public:
	BTThroughputDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~BTThroughputDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_THROUGHPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
