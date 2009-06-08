#include "stdafx.h"
#include <atltime.h>
#include "Util.h"


// ************************************************************************************************
// @Func:	MultiToWide
// 
// @Param:	[IN]	UINT uiCodePage
//			[IN]	LPCSTR lpchzSrc
//			[IN]	int cchTextSrc
//			[OUT]	LPTSTR *lppchzSrc
// 
// @Return:	int
//			변환된 WideChar의 길이
// 
// @Remark:	
// 
// @Desc:	Multibyte를 WideChar로 변환한다.
// ************************************************************************************************
/*
	// usage : MultiToWide (CP_ACP, msgItem.lpchzBody, -1, &lpszValue);
	int MultiToWide( IN UINT uiCodePage, IN LPCSTR lpchzSrc, IN int cchTextSrc, OUT LPTSTR *lppszDst ) {

	int iLen;
	int iIndex;
	UINT uiCodePages[6] = {
		949,
		uiCodePage,
		0xC705,			// 50949	한국어 자동선택
		0xFDE8,			// 65000	UTF7
		0xFDE9,			// 65001	UTF8
		GetACP(),
	};


	iIndex = 0;
	for( iIndex = 0; iIndex < sizeof(uiCodePages)/sizeof(uiCodePages[0]); iIndex++ ) {
		iLen = MultiByteToWideChar( uiCodePages[iIndex], 0, lpchzSrc, cchTextSrc, *lppszDst, 0 );
		if( iLen >= 0 ) {
			break;
		}
	}
	if( iIndex == sizeof(uiCodePages)/sizeof(uiCodePages[0]) )
	{		
		return -1;
	}

	*lppszDst = (LPTSTR)LocalAlloc( LPTR, (iLen+1) * sizeof(TCHAR) );
	if( *lppszDst == NULL ) {
		
		return -1;
	}

	iLen = MultiByteToWideChar( uiCodePages[iIndex], 0, lpchzSrc, cchTextSrc, *lppszDst, iLen );
	if( iLen <= 0 ) {
		
		return iLen;
	}	
	return iLen;
}
*/


wchar_t* ansi_to_unicode(const char *s, size_t len,
				    wchar_t *buf, size_t buf_count)
{
    //PJ_ASSERT_RETURN(s && buf, NULL);

    len = MultiByteToWideChar(CP_ACP, 0, s, len, 
			      buf, buf_count);
    buf[len] = 0;
    return buf;
}


char* unicode_to_ansi( const wchar_t *wstr, size_t len,
				  char *buf, size_t buf_size)
{
    //PJ_ASSERT_RETURN(wstr && buf, NULL);

    len = WideCharToMultiByte(CP_ACP, 0, wstr, len, buf, buf_size, NULL, NULL);
    buf[len] = '\0';
    return buf;
}







char *GetCharBuffer(CString s)
{
	char *d;
	int n=s.GetLength();
	d=new char[n+1];
	int i;
	for (i=0;i<n;i++)
		d[i]=(char )s.GetAt(i);
	d[n]=0;
	return d;
}



int GetCurTimeW(TCHAR *s, int fmt)
{
	CTime t;
	t =  CTime::GetCurrentTime();
	switch(fmt) {
		case 0:	
			wsprintf(s,L"%04d%02d%02d%02d%02d%02d", 
				t.GetYear(), t.GetMonth(), t.GetDay() ,t.GetHour(), t.GetMinute(), t.GetSecond() );
			break;
		case 1:	
			wsprintf(s,L"%02d/%02d %02d:%02d:%02d", 
					t.GetMonth(), t.GetDay() ,t.GetHour(), t.GetMinute(), t.GetSecond() );
			break;
		case 2:	
			wsprintf(s,L"%04d%02d%02d",
					t.GetYear(), t.GetMonth(), t.GetDay() );
			break;
		case 3:	
			wsprintf(s,L"%02d:%02d:%02d", 
					t.GetHour(), t.GetMinute(), t.GetSecond() );
			break;
	}
	return 1;
}



int HsWriteCeFile(TCHAR *fname, char *buf, int len, TCHAR *mode)
{
	size_t wlen=0;
	FILE *fp=NULL;
	
	if((fp = _wfopen(fname, mode)) == NULL)   {
        goto fail;
    }
	if( (wlen=fwrite(buf, 1, len, fp))!=len) {
		fclose(fp);
        goto fail;
    }
	fclose(fp);

    return(wlen);
fail:;
    return(0);
}




TCHAR * SetupLog(TCHAR *dir, TCHAR *file)
{
	static TCHAR buffer[256];
	memset(buffer, '\0', MAX_PATH);
	TCHAR timebuf[32];
	
	GetCurTimeW(timebuf,0);
	BOOL ret = CreateDirectory (dir, NULL);
	

	wsprintf(buffer, L"%s\\%-8.8s_%s", dir, timebuf, file);

	return buffer;
}

void WriteLog(TCHAR *pLogFile, wchar_t *fmt, ...)
{
	// do nothing, if log_file is null
	if(pLogFile == NULL)
		return;

	va_list args;
    char buff[1024], logbuf[1024];
	wchar_t wbuf[1024];
	//	char tmp[32];


	memset(wbuf, 0x00, sizeof(wbuf));
	va_start(args, fmt);
	_vsnwprintf(wbuf, sizeof(wbuf), fmt,  args);
	va_end(args);

	memset(buff, 0x00, sizeof(buff));
	unicode_to_ansi(wbuf, _tcslen(wbuf), buff, 1024);

	TCHAR timebuf[32];
	char timebuf_ansi[32];
	GetCurTimeW(timebuf,3);
	unicode_to_ansi(timebuf, _tcslen(timebuf), timebuf_ansi, 32);

    sprintf(logbuf, "%s %s\n", timebuf_ansi, buff);
	HsWriteCeFile(pLogFile, logbuf, strlen(logbuf)*sizeof(char), L"a+");	
}




////////////////////////////////////////////////////////////
/// LogToFile 
/// Writes szLog into the file named pszFilename
HRESULT LogToFileA(LPTSTR szLog, LPCTSTR pszFilename)
{
    HRESULT hr = E_FAIL;
    
    //Open the handle to the file (and create it if it doesn't exist
    HANDLE hFile = CreateFile(pszFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
        goto Exit;

    //Set the pointer at the end so that we can append szLog
    DWORD dwFilePointer = SetFilePointer(hFile, 0, NULL, FILE_END);
    if (0xFFFFFFFF == dwFilePointer)
        goto Exit;

    //Write to the file
    DWORD dwBytesWritten = 0;
    BOOL bWriteFileRet = WriteFile(hFile, szLog, wcslen(szLog) * 2, &dwBytesWritten, NULL);
    if (!bWriteFileRet)
        goto Exit;

    //Flush the buffer
    BOOL bFlushFileBuffersRet = FlushFileBuffers(hFile);
    if (!bFlushFileBuffersRet)
        goto Exit;

    //Success
    hr = S_OK;

Exit:
    if (NULL != hFile)
        CloseHandle(hFile);

    return hr;
}


HRESULT LogToFile(TCHAR * szLog, TCHAR * pszFilename)
{
    HRESULT hr = E_FAIL;
    
    //Open the handle to the file (and create it if it doesn't exist
    HANDLE hFile = CreateFile(pszFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
        goto Exit;

    //Set the pointer at the end so that we can append szLog
    DWORD dwFilePointer = SetFilePointer(hFile, 0, NULL, FILE_END);
    if (0xFFFFFFFF == dwFilePointer)
        goto Exit;

    //Write to the file
    DWORD dwBytesWritten = 0;
    BOOL bWriteFileRet = WriteFile(hFile, szLog, wcslen(szLog) * 2, &dwBytesWritten, NULL);
    if (!bWriteFileRet)
        goto Exit;

    //Flush the buffer
    BOOL bFlushFileBuffersRet = FlushFileBuffers(hFile);
    if (!bFlushFileBuffersRet)
        goto Exit;

    //Success
    hr = S_OK;

Exit:
    if (NULL != hFile)
        CloseHandle(hFile);

    return hr;
}

void GetLocalIP(TCHAR *hostname, TCHAR *ip)
{
	//  PURPOSE:  
	//		- Obtain the current IP assign to the local PC
	//  PARAMETERS:
	//		- hWnd		:: Handle of the parent window.
	//  OPERATION:
	//		- Call the gethostname API
	//  RETURN VALUE:
	//      - NIL

	HOSTENT *LocalAddress;
	char	*Buff;
	TCHAR	*wBuff;


	// Create new string buffer
	Buff = new char[256];
	wBuff = new TCHAR[256];
	// Reset the string buffer
	memset(Buff, '\0', 256);
	memset(wBuff, TEXT('\0'), 256*sizeof(TCHAR));
	// Get computer name
	if (gethostname(Buff, 256) == 0)
	{
		// Convert computer name from MultiByte char to UNICODE
		mbstowcs(wBuff, Buff, 256);
		// Copy the machine name into the GUI control
		//SetWindowText(GetDlgItem(hWnd, IDC_MACHINE), wBuff);
		wcscpy(hostname, wBuff);


		// Get the local PC IP address
		LocalAddress = gethostbyname(Buff);
		// Reset the string buffer
		memset(Buff, '\0', 256);
		// Compose the obtain ip address
		sprintf(Buff, "%d.%d.%d.%d\0", LocalAddress->h_addr_list[0][0] & 0xFF, LocalAddress->h_addr_list[0][1] & 0x00FF, LocalAddress->h_addr_list[0][2] & 0x0000FF, LocalAddress->h_addr_list[0][3] & 0x000000FF);
		// Reset the wBuff
		memset(wBuff, TEXT('\0'), 256*sizeof(TCHAR));
		// Convert computer name from MultiByte char to UNICODE
		mbstowcs(wBuff, Buff, 256);
		// Set the ip address to edit control 1
		//SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), wBuff);
		wcscpy(ip, wBuff);
		// Set the default port number
		//SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), TEXT("5000"));
	}
	else
	{
		// Notify user about the error
		//MessageBox(hWnd, TEXT("Fail to get host name."), TEXT("MySocket"), MB_OK | MB_ICONEXCLAMATION);
		
	}

	//
	delete Buff;
}


// move hwndAlignee to just below hwnd apart nx (dialog unit)
// align points are left & bottom.
// 왼쪽 정렬은 제거. 상하 만 조절
void VerticalSpace(HWND hwndDlg, UINT nIDAlignCtrl, UINT nIDAffectedCtl, UINT nx)
{
	ASSERT(hwndDlg != NULL);

	HWND hwndAlign = ::GetDlgItem(hwndDlg, nIDAlignCtrl);
	ASSERT(hwndAlign!= NULL);
	RECT rcAlign;
	::GetWindowRect(hwndAlign, &rcAlign);

	POINT ptMoveTo;
	ptMoveTo.x = rcAlign.left;
	ptMoveTo.y = rcAlign.bottom;

	::ScreenToClient(hwndDlg, &ptMoveTo);

	// Verify the affected control and obtain its rect
	HWND hwndCtl = ::GetDlgItem(hwndDlg, nIDAffectedCtl);
	ASSERT(hwndCtl != NULL);
	RECT rcCtl;
	::GetWindowRect(hwndCtl, &rcCtl);
	POINT ptCtl;
	ptCtl.x = rcCtl.left;
	ptCtl.y = rcCtl.bottom;
	::ScreenToClient(hwndDlg, &ptCtl);


	::SetWindowPos(hwndCtl, NULL, ptCtl.x, ptMoveTo.y + nx,
			0, 0, SWP_NOSIZE|SWP_NOZORDER);

	/*
	::MoveWindow(hwndCtl, 
		ptMoveTo.x, 
		ptMoveTo.y + nx,
		rcCtl.right - rcCtl.left,
		rcCtl.bottom - rcCtl.top,
		TRUE);
	*/	
}
