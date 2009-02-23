#pragma once


char *unicode_to_ansi( TCHAR *unicode, char *ansi);
TCHAR *ansi_to_unicode(const char *ansi);

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