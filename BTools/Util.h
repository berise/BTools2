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


// GUI geometry adjustment
void VerticalSpace(HWND hwndDlg, UINT nIDAlignCtrl, UINT nIDAffectedCtl, UINT nx);