#include "stdafx.h"
#include "CSFXPage_w.h"

const int SFX_TIMER_ID = 999;

CSFXPage::CSFXPage() {};
CSFXPage::~CSFXPage() {};

int CSFXPage::OnApply()
{
  return 0;//DoDataExchange(true) ? PSNRET_NOERROR : PSNRET_INVALID;
}

BOOL CSFXPage::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
	CBitmap bm;
	CDIBSectionLite bmTemp;
	bmTemp.SetBitmap(IDB_BITMAP1);

	CSize bmSize = bmTemp.GetSize();

	// dc to make a 32bit bitmap.
	CClientDC dc(m_hWnd);

	m_src.Create32Bit(&dc, bmTemp, bmSize.cx, bmSize.cy); 	
	m_dest.Create32Bit(&dc, bmTemp, bmSize.cx, bmSize.cy);


	m_Water.Create(bmSize.cx, bmSize.cy);
	m_Water.FlattenWater();// flatten it so it appears we turned it off...

	// mouse point
	//m_pt.x =   m_pt.y = 0;

	// come on baby light my fire.. (ok create our fire object)
	m_Fire.m_iAlpha = 80;// want a 30% alpha
	//m_Fire.m_iAlpha = 60;// want a 30% alpha
	m_Fire.m_iHeight =  bmSize.cy;
	m_Fire.m_iWidth = bmSize.cx;
	m_Fire.InitFire();


	SetTimer(SFX_TIMER_ID, 30, NULL);

	return TRUE;
}


void CSFXPage::OnPaint(CDC dc)
{
	CPaintDC pdc(m_hWnd);
	m_dest.Draw(&pdc, CPoint(0, 0));
	//m_dest.Draw(&pdc, CPoint(0, 250));
}

LRESULT CSFXPage::OnTimer(UINT_PTR nIDEvent)
{

	CSize bmSize = m_dest.GetSize();

	if(nIDEvent == SFX_TIMER_ID)
	{
		//if(!m_bRunWater)
		//	m_Water.FlattenWater();// flatten it so it appears we turned it off...

		m_Water.Render((DWORD*)m_src.GetDIBits(),(DWORD*)m_dest.GetDIBits());

		//if(m_bRunFire)
			m_Fire.Render((DWORD*)m_dest.GetDIBits(),bmSize.cx,bmSize.cy);

		//if(m_bRunPlasma)
		//	m_Plasma.Render((DWORD*)m_dest.GetDIBits(),402,120,402);

		/*CClientDC dc(m_hWnd);
		CPoint ptOrigin(15,20);
		m_dest.Draw(&dc,ptOrigin);
		*/
		Invalidate(FALSE);
	}
	return 0;
}
