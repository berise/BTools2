// BTools.h : BTools ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resourceppc.h"

// CBToolsApp:
// �� Ŭ������ ������ ���ؼ��� BTools.cpp�� �����Ͻʽÿ�.
//

class CBToolsApp : public CWinApp
{
public:
	CBToolsApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
public:
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
};

extern CBToolsApp theApp;
