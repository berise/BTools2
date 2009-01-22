// FileEx.h: interface for the CFileEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEEX_H__F85DCA61_CE1C_11D5_AEE6_00B0D0B8715B__INCLUDED_)
#define AFX_FILEEX_H__F85DCA61_CE1C_11D5_AEE6_00B0D0B8715B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef UINT (FileEncryptProc)(const BYTE *pBuffer,UINT dwSize,BYTE *pDestination,DWORD dwParam);

class CFileEx  : public CFile
{
public:
	CFileEx(int nBufSize=4096);
	CFileEx(LPCTSTR lpszFileName, UINT nOpenFlags,int nBufSize=4096);
	virtual ~CFileEx();
	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write( const void* lpBuf, UINT nCount );
	void SetEncryption(FileEncryptProc *pEncrypt,DWORD dwEncryptParam,FileEncryptProc *pDecrypt,DWORD dwDecryptParam);
protected:
	BYTE* m_pBuf;
	FileEncryptProc *m_pEncrypt,*m_pDecrypt;
	DWORD	m_dwEncrypt,m_dwDecrypt;
};

#endif // !defined(AFX_FILEEX_H__F85DCA61_CE1C_11D5_AEE6_00B0D0B8715B__INCLUDED_)
