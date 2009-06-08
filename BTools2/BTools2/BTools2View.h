// BTools2View.h : interface of the CBTools2View class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlcrack.h>
#include <atlmisc.h>
#include "resourceppc.h"
#include "OScopeCtrl.h"
#include "ScreenLib.h"
#include "BTPingPage.h"
#include "BTIPerfClientPage.h"
#include "BTIPerfServerPage.h"

class CBTools2View : 
	public CPropertySheetImpl<CBTools2View>
{
public:
	DECLARE_WND_CLASS(NULL)
	

	BOOL PreTranslateMessage(MSG* pMsg);

public:

	CBTPingPage m_Page1;
	CIPerfClientPage m_Page2;
	CIPerfServerPage m_Page3;
	

	CBTools2View();

	BEGIN_MSG_MAP(CBTools2View)		
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
	END_MSG_MAP()

	//


	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
};

