// BToolsDoc.h : CBToolsDoc 클래스의 인터페이스
//


#pragma once

class CBToolsDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CBToolsDoc();
	DECLARE_DYNCREATE(CBToolsDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// 구현입니다.
public:
	virtual ~CBToolsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};


