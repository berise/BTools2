#pragma once


char *unicode_to_ansi( TCHAR *unicode, char *ansi);
TCHAR *ansi_to_unicode(const char *ansi);

char *GetCharBuffer(CString s); // client


// log functions

static TCHAR btools_log_dir[MAX_PATH];
static TCHAR btools_log_file[MAX_PATH]; // 260

// WriteLog must be called after Setuplog
void WriteLog(wchar_t *fmt, ...);

TCHAR * SetupLog(TCHAR *dir, TCHAR *file);

int HsWriteCeFile(TCHAR *fname, char *buf, int len, TCHAR *mode);
int GetCurTimeW(TCHAR *s, int fmt);

void GetLocalIP(HWND hWnd);