#include "stdafx.h"
#include "SFXStatic.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p); (p)=NULL; } }
#endif // SAFE_DELETE


#ifndef SAFE_FREE
#define SAFE_FREE(p) { if(p) { free(p); (p) = NULL; } }
#endif // SAFE_FREE

/////////////////////////////////////////////////////////////////////////////
// CSFXStatic

CSFXStatic::CSFXStatic()
:m_src(NULL), 
m_dest(NULL),
m_Water(NULL),
m_Fire(NULL),
m_Plasma(NULL)
{
	m_src = new CDIBSectionLite();
	m_dest = new CDIBSectionLite();
	m_Water	= new CWaterRoutine;
	m_Fire	= new CFireRoutine;
	m_Plasma	= new CPlasmaRoutine;
}

CSFXStatic::~CSFXStatic()
{
	//Stop();
	SAFE_DELETE(m_Water);
	SAFE_DELETE(m_Fire);
	SAFE_DELETE(m_Plasma);

	if(m_src != NULL)	{		delete m_src;	m_src = NULL;	}
	if(m_dest != NULL)	{		delete m_dest;	m_dest = NULL;	}

}

BOOL CSFXStatic::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, UINT nID) 
{
  BOOL result ;
  

  ASSERT(pParentWnd->GetSafeHwnd() != NULL);
  result = CStatic::Create(L"Special Effect Control", dwStyle, rect, pParentWnd, nID);
  /*
  static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;
  result = CWnd::CreateEx( WS_EX_CLIENTEDGE, // | WS_EX_WINDOWEDGE,  WS_EX_STATICEDGE, 
                          className, L"SFXWnd", dwStyle, 
                          rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
                          pParentWnd->GetSafeHwnd(), (HMENU)nID) ;
						  */

  //SetWindowText(L"Special Effect Control");
  m_hTimer = 0;

  return result ;
}


BEGIN_MESSAGE_MAP(CSFXStatic, CStatic)
	//{{AFX_MSG_MAP(CSFXStatic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
ON_WM_MOUSEMOVE()
ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSFXStatic message handlers

void CSFXStatic::SetBitmapResource(UINT nBitmap)
{
//	m_iImagematrix = bitmap;
	// release�ǿ����� ���⼭ _AfxDispatchCmdMsg �Լ��� ������. ������ ����???
	// CStatic�� �θ� ������� ���ϰ� ������ ������ ���� �ʴ´�.
	// ���� CWnd���� �� ó���ؾ� �� ���� �ֳ� ����...
	CDIBSectionLite dib_temp;
	dib_temp.SetBitmap(nBitmap);
	//m_dest->SetBitmap(nBitmap);	
	CSize bmSize = dib_temp.GetSize();

	// ������ ���� �ʱ�ȭ
	SetWindowPos(NULL, 0, 0, bmSize.cx, bmSize.cy, 	SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);

	// CDIBSection requires ClientDC to convert 24bits to 32bits bitmap.
	CClientDC dc(this);	

	m_src->Create32Bit(&dc, dib_temp, bmSize.cx, bmSize.cy); 	
	m_dest->Create32Bit(&dc, dib_temp, bmSize.cx, bmSize.cy); 	
	
	// init SFX routines
	m_Water->Create(bmSize.cx, bmSize.cy);
	m_Water->FlattenWater();// flatten it so it appears we turned it off...

	// come on baby light my fire.. (ok create our fire object)
	m_Fire->m_iAlpha = 70;// want a 30% alpha
	//m_Fire->m_iAlpha = 60;// want a 30% alpha
	m_Fire->m_iHeight =  bmSize.cy;
	m_Fire->m_iWidth = bmSize.cx;
	//m_Fire->InitFire();

	// finally the plasma
	//m_Plasma->Create(bmSize.cx, bmSize.cy);
	//m_Plasma->m_iAlpha = 20;// 30 % alpha

	
}

void CSFXStatic::OnTimer(UINT nIDEvent) 
{
	CClientDC dc(this);

	//�ڵ� ����� ó�� 
	if(nIDEvent == 1000)
	{
		srand(GetTickCount());

		CRect rcPicture;

		rcPicture.left = 0;
		rcPicture.top = m_src->GetHeight();
		rcPicture.right = rcPicture.left + m_src->GetWidth();
		rcPicture.bottom = rcPicture.top + m_src->GetHeight();

		CPoint point;
		point.x = rand() % m_src->GetWidth();
		point.y = rcPicture.top + rand() % m_src->GetHeight();
		
		point.y = rcPicture.bottom - point.y;
		//m_Water->WarpBlob(point.x,point.y,15,30,m_Water->m_iHpage);
		//m_Water->SineBlob(point.x,point.y,15,30,m_Water->m_iHpage);
		m_Water->HeightBlob(point.x,point.y,20,40,m_Water->m_iHpage);
	}


	CSize bmSize = m_dest->GetSize();
 
	m_Water->Render((DWORD*)m_src->GetDIBits(),(DWORD*)m_dest->GetDIBits());
	//m_Fire->Render((DWORD*)m_dest->GetDIBits(),bmSize.cx, bmSize.cy);

	m_dest->Draw(&dc, CPoint(0, 0));
} 

//
void CSFXStatic::OnPaint() 
{
	CPaintDC dc(this);
	m_dest->Draw(&dc, CPoint(0, 0));
}


int CSFXStatic::Run(UINT nMilliSecond)
{
	// Time trigger
	m_hTimer = SetTimer(999, nMilliSecond, NULL);
	
	//�ڵ� �� ȿ��
	SetTimer(999+1,  8000, NULL);

	return m_hTimer;
}

BOOL CSFXStatic::Stop()
{
	BOOL ret = -1;
	if(m_hTimer != 0)
	{
		ret = KillTimer(m_hTimer);
		m_hTimer = 0;
	}

	return ret;
}

void CSFXStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rcPicture;

	rcPicture.left = 0;
	rcPicture.top =0;
	rcPicture.right = rcPicture.left + m_src->GetWidth();
	rcPicture.bottom = rcPicture.top + m_src->GetHeight();

	if(rcPicture.PtInRect(point) == TRUE)
	{
		// since dibs are drawn upside down we need to flip the y position (for it to look right)
		point.y = rcPicture.bottom - point.y;

		m_Water->HeightBlob(point.x,point.y,10,20,m_Water->m_iHpage);
		//m_Water->HeightBox(point.x,point.y,10,30,m_Water->m_iHpage);
		//m_Water->SineBlob(point.x,point.y,15,60,m_Water->m_iHpage);
		//m_Water->WarpBlob(point.x,point.y,5,10,m_Water->m_iHpage);
	}
	
	//Invalidate(FALSE);
	CWnd::OnMouseMove(nFlags, point);
}
void CSFXStatic::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Stop();
}
