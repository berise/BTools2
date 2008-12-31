// BTThroughputDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BTools.h"
#include "BTThroughputDialog.h"


// BTThroughputDialog 대화 상자입니다.

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


// BTThroughputDialog 메시지 처리기입니다.
