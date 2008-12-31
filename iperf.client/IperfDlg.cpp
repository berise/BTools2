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
// IperfDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "IperfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIperfDlg dialog

char *GetCharBuffer(CString s)
{
	char *d;
	int n=s.GetLength();
	d=new char[n+1];
	int i;
	for (i=0;i<n;i++)
		d[i]=(char )s.GetAt(i);
	d[n]=0;
	return d;
}

DWORD WINAPI StartThread(LPVOID lpParameter )

{
	Client *c=(Client *)lpParameter;

	c->Start();
	c->m_pReporter->ClientFinished();

	return 0;
}

CIperfDlg::CIperfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIperfDlg::IDD, pParent)
{
	m_pIPerfClient=0;
	m_bClientStarted=FALSE;

	m_pSettings = new ext_Settings();
	
	m_fStatistics = NULL;
	//{{AFX_DATA_INIT(CIperfDlg)
	m_csHostName = _T("");
	m_uiInterval = 0.0;
	m_uiTotalTime = 0;
	m_csReportFile = _T("\\My Documents\\iperf-stats.log");
	m_csErrorFile = _T("\\My Documents\\iperf.err");
	//}}AFX_DATA_INIT
}


void CIperfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIperfDlg)
	DDX_Text(pDX, IDC_HOST_NAME, m_csHostName);
	DDX_Text(pDX, IDC_INTERVAL, m_uiInterval);
	DDX_Text(pDX, IDC_TOTAL_TIME, m_uiTotalTime);
	DDX_Text(pDX, IDC_REPORT_FILE, m_csReportFile);
	DDX_Text(pDX, IDC_ERROR_FILE, m_csErrorFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIperfDlg, CDialog)
	//{{AFX_MSG_MAP(CIperfDlg)
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIperfDlg message handlers

void CIperfDlg::PrintBuffer(char *buffer,char *speed)
{
//	m_csReport.AddString((const unsigned short *)buffer);
	if (m_fStatistics !=NULL)
	{
		fprintf(m_fStatistics,"%s",buffer);
		fflush(m_fStatistics);
	}
	if (speed!=0)
	{
		CString ss(speed);
		GetDlgItem(IDC_CLIENT_VALUES)->SetWindowText(ss);
	}
//	GetGlobalLog()->LogEvent(EVENT_DEBUG,buffer);
}

void CIperfDlg::ClientFinished()
{
	GetDlgItem(IDC_CLIENT_STATUS)->SetWindowText(_T("Client Finished"));
	m_bClientStarted=FALSE;
	GetDlgItem(IDC_START)->SetWindowText(_T("Start"));
	fclose(m_fStatistics);
	m_fStatistics=NULL;
	fclose(winCEStderr);
	winCEStderr=NULL;
}

void CIperfDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	if (!m_bClientStarted)
	{
		m_bClientStarted=TRUE;
		GetDlgItem(IDC_START)->SetWindowText(_T("Stop"));
		UpdateData(TRUE);


		m_fStatistics = fopen(GetCharBuffer(m_csReportFile),"w");

		winCEStderr = fopen(GetCharBuffer(m_csErrorFile),"w");

		if (m_pIPerfClient!=0)
			delete m_pIPerfClient;

		Settings *pSettings = new Settings(m_pSettings);

		m_pSettings->mHost=GetCharBuffer(m_csHostName);
		m_pSettings->mInterval=m_uiInterval;

		if (m_uiTotalTime>1)
			m_pSettings->mAmount=-m_uiTotalTime*100;

		m_pIPerfClient = new Client(m_pSettings,this);

		m_pIPerfClient->InitiateServer();

		CreateThread(NULL,NULL,StartThread,m_pIPerfClient,0,NULL);

		GetDlgItem(IDC_CLIENT_STATUS)->SetWindowText(_T("Client Running"));

		//		StartThread(m_pIPerfClient);
		// Run the test
	}
	else
	{
		m_pIPerfClient->Sig_Interupt(0);
		m_bClientStarted=FALSE;
		GetDlgItem(IDC_START)->SetWindowText(_T("Start"));
	}
	
}

BOOL CIperfDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
