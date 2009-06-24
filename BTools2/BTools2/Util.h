#pragma once


// unicode <-> ansi conversion
wchar_t* ansi_to_unicode(const char *s, size_t len,
				    wchar_t *buf, size_t buf_count);

char* unicode_to_ansi( const wchar_t *wstr, size_t len,
				  char *buf, size_t buf_size);


char *GetCharBuffer(CString s); // client


// log functions
void WriteLog(TCHAR *pLogFile, wchar_t *fmt, ...);
TCHAR * SetupLog(TCHAR *dir, TCHAR *file);

int HsWriteCeFile(TCHAR *fname, char *buf, int len, TCHAR *mode);
int GetCurTimeW(TCHAR *s, int fmt);

void GetLocalIP(TCHAR *hostname, TCHAR *ip);

// LogToFile
// Writes szLog into the file named pszFilename
HRESULT LogToFileA(LPTSTR szLog, LPCTSTR pszFilename);
HRESULT LogToFile(TCHAR * szLog, TCHAR * pszFilename);

// GUI geometry adjustment
void VerticalSpace(HWND hwndDlg, UINT nIDAlignCtrl, UINT nIDAffectedCtl, UINT nx);

