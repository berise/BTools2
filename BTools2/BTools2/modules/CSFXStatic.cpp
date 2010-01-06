#include "stdafx.h"
#include "CSFXStatic.h"

const int SFX_TIMER_UPDATE = 999;
const int SFX_TIMER_WATER = SFX_TIMER_UPDATE + 1;


// Parent window must call destroy & clean CSFXStatic
// or you'll see ERROR - Object deleted before window was destroyed
CSFXStatic::CSFXStatic() {};
CSFXStatic::~CSFXStatic()
{
	
};



int CSFXStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CBitmap bm;
	CDIBSectionLite bmTemp;
	//bmTemp.Load(L"\\ee.bmp");
	bmTemp.SetBitmap(IDB_BITMAP3);

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
	m_Fire.m_iAlpha = 70;// want a 30% alpha
	//m_Fire.m_iAlpha = 60;// want a 30% alpha
	m_Fire.m_iHeight =  bmSize.cy;
	m_Fire.m_iWidth = bmSize.cx;
	m_Fire.InitFire();


	SetTimer(SFX_TIMER_UPDATE, 40, NULL);
	SetTimer(SFX_TIMER_WATER, 5000, NULL);


	// random water drop.
	srand(GetTickCount());

	//
	SetMsgHandled(TRUE);

	MoveWindow(0, 0, bmSize.cx, bmSize.cy);
	return 0;
}

void CSFXStatic::OnDestroy()
{
	KillTimer(SFX_TIMER_UPDATE);
	KillTimer(SFX_TIMER_WATER);
}


void CSFXStatic::OnPaint(CDC dc)
{
	CPaintDC pdc(m_hWnd);
	m_dest.Draw(&pdc, CPoint(0, 0));
	//m_dest.Draw(&pdc, CPoint(0, 250));
}

LRESULT CSFXStatic::OnTimer(UINT_PTR nIDEvent)
{

	CSize bmSize = m_dest.GetSize();

	if(nIDEvent == SFX_TIMER_UPDATE)
	{
		//if(!m_bRunWater)
		//	m_Water.FlattenWater();// flatten it so it appears we turned it off...

		m_Water.Render((DWORD*)m_src.GetDIBits(),(DWORD*)m_dest.GetDIBits());

		//if(m_bRunFire)
		//	m_Fire.Render((DWORD*)m_dest.GetDIBits(),bmSize.cx,bmSize.cy);

		//if(m_bRunPlasma)
		//	m_Plasma.Render((DWORD*)m_dest.GetDIBits(),402,120,402);
		Invalidate(FALSE);
	}

	if(nIDEvent == SFX_TIMER_WATER)
	{
		CRect rcPicture;

		rcPicture.left = 0;
		rcPicture.top =0;
		rcPicture.right = rcPicture.left + m_src.GetWidth();
		rcPicture.bottom = rcPicture.top + m_src.GetHeight();

		CPoint point;
		point.x = rand() % m_src.GetWidth();
		point.y = rcPicture.top + rand() % m_src.GetHeight();

		if(rcPicture.PtInRect(point) == TRUE)
		{
			// since dibs are drawn upside down we need to flip the y position (for it to look right)
			point.y = rcPicture.bottom - point.y;

			m_Water.HeightBlob(point.x,point.y,20,40,m_Water.m_iHpage);
			//m_Water.SineBlob(point.x,point.y,20,40,m_Water.m_iHpage);
		}

	}

	return 0;
}



// mouse operations
void CSFXStatic::OnMouseMove(UINT nFlags, CPoint point)
{
		// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rcPicture;

	rcPicture.left = 0;
	rcPicture.top =0;
	rcPicture.right = rcPicture.left + m_src.GetWidth();
	rcPicture.bottom = rcPicture.top + m_src.GetHeight();

	if(rcPicture.PtInRect(point) == TRUE)
	{
		// since dibs are drawn upside down we need to flip the y position (for it to look right)
		point.y = rcPicture.bottom - point.y;

		m_Water.HeightBlob(point.x,point.y,10,30,m_Water.m_iHpage);
		//m_Water->HeightBox(point.x,point.y,10,30,m_Water->m_iHpage);
		//m_Water->SineBlob(point.x,point.y,15,60,m_Water->m_iHpage);
		//m_Water->WarpBlob(point.x,point.y,5,10,m_Water->m_iHpage);
	}
	

}