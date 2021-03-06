// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change this value to use different versions
#define WINVER 0x0420
#include <atlbase.h>
#include <atlstr.h>

#define _WTL_NO_CSTRING
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <tpcshell.h>
#include <aygshell.h>
#pragma comment(lib, "aygshell.lib")

#include <atlframe.h>
#include <atlctrls.h>
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS
#include <atldlgs.h>
#define _WTL_CE_NO_ZOOMSCROLL
#define _WTL_CE_NO_FULLSCREEN
#define END_MSG_MAP_EX END_MSG_MAP
#include <atlwince.h>

#include <atlmisc.h>
#include <atlcrack.h>
#include <atlddx.h>
#include <atlctrls.h>

// project specific includes
#include "resourceppc.h"


// ini settings
const int MAX_ITEM	=	3;
#define INI_SECTION_PING	"Ping"
#define INI_SECTION_CLIENT	"iPref Client"
#define INI_SECTION_SERVER	"iPref Server"

#define TSTORE_APPLICATION_ID "OM00016302"
