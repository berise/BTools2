// BTools2.cpp : main source file for BTools2.exe
//

#include "stdafx.h"

#include "resourceppc.h"

#include "BTools2View.h"
#include "AboutDlg.h"
#include "BTools2Frame.h"
#include "atltime.h"
#include "ARM_PLUGIN.h"
#include "util.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	/*
	// Program Expiration routines
	CTime expirationTime(2009,	// year
		10,					// month
		31,					// date
		23,					// hour 24
		59,					// minute
		59);				// second
	CTime currentTime = CTime::GetCurrentTime();

    CTimeSpan leftTime = expirationTime - currentTime;   

	// 사용 기간이 남았을 경우
	if(leftTime.GetTotalSeconds() > 0)
    {
		 CString msg;
		 msg = L"";
		 msg += L"This is a trial version of BTools2\n";
		 msg += expirationTime.Format(L"Expiration date :\n %Y-%m-%d  %H:%M:%S\n\n");
		 msg += leftTime.Format(L"%D day(s) and\n %H:%M:%S left" );
     
		 // 사용 기간이 3일 미만 남았을 경우
		 if(leftTime.GetDays() < 7)
		 {
			 msg = leftTime.Format(L"This software will expire after %D day(s)");// %H Hour(s) %M Minute(s)");
			 AtlMessageBox(NULL, msg.GetBuffer(), L"Warning");
		 }
		 //AfxMessageBox(msg);
    }
    else // 사용 기간이 만료된 경우
    {
		CString msg("This is a trial version of BTools2\n"
				"If you want to use this software more\n"
				"Please contact to me.\nberise@gmail.com\n"
				"Thank you for your interest\n");

		msg += expirationTime.Format(L"Expiration date :\n %Y-%m-%d  %H:%M\n\n");
		msg += leftTime.Format(L"%D day(s) and\n %H:%M:%S left" );
        //msg.Format(L"This software is submitted for the Windows Mobile Contest");
		AtlMessageBox(NULL, msg.GetBuffer(), L"Warning");
		return FALSE;
	 }
	 */

#ifdef __TSTORE_ARM__
	/*
	// ARM
	T_RETURN ret;
	TAPPID *APPID = TSTORE_APPLICATION_ID;

	bool aSuccess=true;

	T_RETURN res; 
	ARM_PLUGIN_Initialize(&res,APPID);
	if (res.code == ARM_SUCCESS) 
	{
		ARM_PLUGIN_CheckLicense(&res); 
		if (res.code == ARM_SUCCESS) 
		{ 
			ARM_PLUGIN_RequestLicense(&res); 
			if (res.code !=ARM_SUCCESS) 
			{ //실패시 메시 구현 
				aSuccess=false; 
			} 
		} 
		else 
		{//실패시 메시 구현 
			aSuccess=false;

			TCHAR wszMsg[1024];

			switch(res.code)
			{
			case CLICENSE_DENY:
			case CLICENSE_NOT_EXIST:
			case CLICENSE_EXPIRED:
			case CLICENSE_INVALID:
				{
					if(ret.pMsg)
					{
						ansi_to_unicode(ret.pMsg, strlen(ret.pMsg), wszMsg, 1024);
						::AtlMessageBox(NULL, wszMsg, L"Check License");
					}
				}

			default:
				;
			}
		} 
	} 
	else 
	{ 
		aSuccess=false; 
	} 
	ARM_PLUGIN_Uninitialize(&res); 
	*/

	//if (! aSuccess) PostQuitMessage(0); 

#endif

	//CAboutDlg aboutDlg;	aboutDlg.DoModal();
    //
	HRESULT hRes = CBTools2Frame::ActivatePreviousInstance(hInstance, lpstrCmdLine);

	if(FAILED(hRes) || S_FALSE == hRes)
	{
		return hRes;
	}

	hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	AtlInitCommonControls(ICC_DATE_CLASSES);
	SHInitExtraControls();

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = CBTools2Frame::AppRun(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

