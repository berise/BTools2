// BTThroughputDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTThroughputDialog.h"


// BTThroughputDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(BTThroughputDialog, CPropertyViewPage)

BTThroughputDialog::BTThroughputDialog(CWnd* pParent /*=NULL*/)
	: CPropertyViewPage(BTThroughputDialog::IDD, 0,  pParent)
{

}

BTThroughputDialog::~BTThroughputDialog()
{
}

void BTThroughputDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyViewPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BTThroughputDialog, CPropertyViewPage)
END_MESSAGE_MAP()


// BTThroughputDialog �޽��� ó�����Դϴ�.
