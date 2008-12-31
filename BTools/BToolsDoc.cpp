// BToolsDoc.cpp : CBToolsDoc 클래스의 구현
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

// CBToolsDoc 생성/소멸

CBToolsDoc::CBToolsDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CBToolsDoc::~CBToolsDoc()
{
}

BOOL CBToolsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CBToolsDoc serialization

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void CBToolsDoc::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// CBToolsDoc 진단

#ifdef _DEBUG
void CBToolsDoc::AssertValid() const
{
	CDocument::AssertValid();
}
#endif //_DEBUG


// CBToolsDoc 명령

