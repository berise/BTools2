// BToolsDoc.h : CBToolsDoc Ŭ������ �������̽�
//


#pragma once

class CBToolsDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CBToolsDoc();
	DECLARE_DYNCREATE(CBToolsDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// �����Դϴ�.
public:
	virtual ~CBToolsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


