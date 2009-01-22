// FileEx.cpp: implementation of the CFileEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileEx::CFileEx(int nBufSize) : CFile() , m_pEncrypt(NULL) , m_pDecrypt(NULL)
{
	m_pBuf=new BYTE[nBufSize+16];
}

CFileEx::CFileEx(LPCTSTR lpszFileName, UINT nOpenFlags,int nBufSize) : CFile(lpszFileName, nOpenFlags) , m_pEncrypt(NULL) , m_pDecrypt(NULL)
{
	m_pBuf=new BYTE[nBufSize+16];
}

CFileEx::~CFileEx()
{
	if (m_pBuf)
		delete[] m_pBuf;
}

UINT CFileEx::Read(void* lpBuf, UINT nCount)
{
	if (m_pDecrypt)
	{
		UINT nRead=CFile::Read(m_pBuf,nCount);
		return (nRead>0) ? (*m_pDecrypt)(m_pBuf,nRead,(BYTE*)lpBuf,m_dwDecrypt) : 0;
	}
	else
		return CFile::Read(lpBuf,nCount);
}

void CFileEx::Write(const void* lpBuf, UINT nCount)
{
	if (m_pEncrypt)
		CFile::Write(m_pBuf,(*m_pEncrypt)((const BYTE*)lpBuf,nCount,m_pBuf,m_dwEncrypt));
	else
		CFile::Write(lpBuf,nCount);
}

void CFileEx::SetEncryption(FileEncryptProc *pEncrypt,DWORD dwEncryptParam,FileEncryptProc *pDecrypt,DWORD dwDecryptParam)
{
	m_pEncrypt=pEncrypt;
	m_pDecrypt=pDecrypt;
	m_dwEncrypt=dwEncryptParam;
	m_dwDecrypt=dwDecryptParam;
}