// BToolsDoc.cpp : CBToolsDoc Ŭ������ ����
//

#include "stdafx.h"
#include "BTools.h"

#include "BToolsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBToolsDoc

IMPLEMENT_DYNCREATE(CBToolsDoc, CDocument)

BEGIN_MESSAGE_MAP(CBToolsDoc, CDocument)
END_MESSAGE_MAP()

// CBToolsDoc ����/�Ҹ�

CBToolsDoc::CBToolsDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CBToolsDoc::~CBToolsDoc()
{
}

BOOL CBToolsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

// CBToolsDoc serialization

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void CBToolsDoc::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// CBToolsDoc ����

#ifdef _DEBUG
void CBToolsDoc::AssertValid() const
{
	CDocument::AssertValid();
}
#endif //_DEBUG


// CBToolsDoc ���

