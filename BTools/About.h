/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#ifndef ABOUT_H
#define ABOUT_H

#include "Resourceppc.h"

#include "3rd/MatrixStatic.h" // lcd like display
#include "3rd/SFXStatic.h"

#include "Util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	~CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
#ifdef _DEVICE_RESOLUTION_AWARE
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	CMatrixStatic m_matrix1;
	CMatrixStatic m_matrix2;
	CSFXStatic		m_cSFX;


	afx_msg void OnAboutmenu2Scroll2();
	afx_msg void OnAboutmenu2Stop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


#endif //ABOUT_H
