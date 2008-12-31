#include "stdafx.h"
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


TCHAR *ansi_to_unicode(const char *ansi)
{
	static TCHAR buf [ 128 * 2];
	memset(buf, 0x00, sizeof(buf));
	int len = strlen(ansi);
	USES_CONVERSION;
	wcscpy(buf,A2W(ansi));
	return buf;
}

TCHAR *ansi_to_unicode( char *ansi, TCHAR *unicode = NULL)
{
	static TCHAR buf [ 128 * 2];
	memset(buf, 0x00, sizeof(buf));
	int len = strlen(ansi);
	USES_CONVERSION;
	wcscpy(buf,A2W(ansi));
	if(unicode != NULL)
		wcscpy(unicode, A2W(ansi));

	return buf;
}




char *unicode_to_ansi( TCHAR *unicode, char *ansi)
{
//	static char buf [ 128 * 2];
	//memset(buf, 0x00, sizeof(buf));
	USES_CONVERSION;
	strcpy(ansi,W2A(unicode));

	return ansi;
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
	memset(btools_log_file, '\0', MAX_PATH);
	TCHAR timebuf[32];
	
	GetCurTimeW(timebuf,0);
	BOOL ret = CreateDirectory (dir, NULL);
	if(ret == 0) // can not create log directory
	{
		1;
	}

	wsprintf(btools_log_file, L"%s\\%-8.8s_%s", dir, timebuf, file);

	return btools_log_file;
}

void WriteLog(wchar_t *fmt, ...)
{
	// do nothing, if log_file is null
	if(btools_log_file == NULL)
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
	unicode_to_ansi(wbuf, buff);

	TCHAR timebuf[32];
	char timebuf_ansi[32];
	GetCurTimeW(timebuf,3);
	unicode_to_ansi(timebuf, timebuf_ansi);

    sprintf(logbuf, "%s %s\n", timebuf_ansi, buff);
	HsWriteCeFile(btools_log_file, logbuf, strlen(logbuf)*sizeof(char), L"a+");	
}



