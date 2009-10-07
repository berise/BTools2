// AboutDlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resourceppc.h"
#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	AtlCreateEmptyMenuBar(m_hWnd);

	m_matrix.Create(m_hWnd);//GetDlgItem(IDC_STATIC_MATRIX));

	
	CRect r;
	GetDlgItem(IDC_STATIC_MATRIX).GetWindowRect(r);
	ScreenToClient(r);
	m_matrix.MoveWindow(r);


	int nX = 20, nY = 8;
	m_matrix.SetNumberOfLines(nY);
	m_matrix.SetXCharsPerLine(nX);
	m_matrix.SetSize(CMatrixStatic::TINY, IDB_BITMAP1);
	m_matrix.AdjustClientXToSize(nX);
	m_matrix.AdjustClientYToSize(nY);
	//m_matrix.SetText(_T(" !\"#$%&'()*+,-./0123456789;:<=>?@ABCDEFGHIJKLMNO PQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
	/////////////////012345678901234567890
	CString msg(	"+------------------+"
					"|BTools2 for Windows"
					"|    Mobile V, VI.x|"
					"+-----------+------+"
					"| Find out your    |"
					"|  data limits     |"
					"| berise@gmail.com |"
					"+------------------+");
	
	m_matrix.SetText(msg);

	return bHandled = FALSE;
}

