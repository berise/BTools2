/*
 * Copyright (c) 2006 Rutgers, The State University of New Jersey
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that (i) the above copyright notice and this permission notice
 * appear in all copies of the software and related documentation. (ii)
 * the name of Rutgers University may not be used in any advertising
 * or publicity relating to the software without the specific, prior
 * written permission of Rutgers University.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL RUTGERS UNIVERSITY BE LIABLE FOR ANY SPECIAL,
 * INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY
 * THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
#if !defined(AFX_IPERFDLG_H__5D311226_020A_4F53_BD49_62AADC635C78__INCLUDED_)
#define AFX_IPERFDLG_H__5D311226_020A_4F53_BD49_62AADC635C78__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// IperfDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIperfDlg dialog

#include "Client.hpp"
#include "Reporter.h"

class CIperfDlg : public CDialog,CReporter
{
// Construction
public:
	CIperfDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIperfDlg)
	enum { IDD = IDD_IPERF_DLG };
	CString	m_csHostName;
	double	m_uiInterval;
	int		m_uiTotalTime;
	CString	m_csReportFile;
	CString	m_csErrorFile;
	//}}AFX_DATA

	void virtual PrintBuffer(char *buffer,char *speed);
	void virtual ClientFinished();

	FILE *m_fStatistics;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIperfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL m_bClientStarted;
	Client *m_pIPerfClient;

	ext_Settings* m_pSettings;

	// Generated message map functions
	//{{AFX_MSG(CIperfDlg)
	afx_msg void OnStart();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPERFDLG_H__5D311226_020A_4F53_BD49_62AADC635C78__INCLUDED_)
