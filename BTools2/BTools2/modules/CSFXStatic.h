//#include "resourceppc.h"
#include "DIBSectionLite.h"
#include "CWaterRoutine.h"
#include "CFireRoutine.h"

class CSFXStatic: public CWindowImpl<CSFXStatic, CStatic>//, public CWinDataExchange<CSFXStatic>
{
public:
  // Construction

  CSFXStatic();
  virtual ~CSFXStatic();
 
  // Maps

  BEGIN_MSG_MAP(CSFXStatic) 
	MSG_WM_CREATE(OnCreate)
	MSG_WM_DESTROY(OnDestroy)
	MSG_WM_PAINT(OnPaint)
	MSG_WM_TIMER(OnTimer)
	MSG_WM_MOUSEMOVE(OnMouseMove)
  END_MSG_MAP()
 
     //MSG_WM_INITDIALOG(OnInitDialog)
	 // CHAIN_MSG_MAP( CWindowImpl<CSFXStatic, CStatic> )
  /*
  BEGIN_DDX_MAP(CSFXStatic)
    DDX_RADIO(IDC_BLUE, m_nColor)
    DDX_RADIO(IDC_ALYSON, m_nPicture)
  END_DDX_MAP()
  */
 
  // Message handlers
  int OnCreate(LPCREATESTRUCT lpCreateStruct);
  void OnDestroy();
  void OnPaint(CDC dc);
  void OnMouseMove(UINT nFlags, CPoint point);



  CDIBSectionLite m_src, m_dest;
  CWaterRoutine m_Water;
  CFireRoutine	m_Fire;
  LRESULT OnTimer(UINT_PTR nIDEvent);
};
