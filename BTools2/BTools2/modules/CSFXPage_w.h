//#include "resourceppc.h"
#include "DIBSectionLite.h"
#include "CWaterRoutine.h"
#include "CFireRoutine.h"

class CSFXPage: public CStatic,
			public CWinDataExchange<CSFXPage>
{
public:
//  enum { IDD = IDD_SFX_PAGE };
 
  // Construction

  CSFXPage();
  ~CSFXPage();
 
  // Maps

  BEGIN_MSG_MAP_EX(CSFXPage)
    MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_PAINT(OnPaint)
	MSG_WM_TIMER(OnTimer)
    //CHAIN_MSG_MAP(CStatic)
  END_MSG_MAP_EX()
 
  /*
  BEGIN_DDX_MAP(CSFXPage)
    DDX_RADIO(IDC_BLUE, m_nColor)
    DDX_RADIO(IDC_ALYSON, m_nPicture)
  END_DDX_MAP()
  */
 
  // Message handlers

  BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
  void OnPaint(CDC dc);
 
  // Property page notification handlers

  int OnApply();
 
  // DDX variables

  int m_nColor, m_nPicture;


  CDIBSectionLite m_src, m_dest;
  CWaterRoutine m_Water;
  CFireRoutine	m_Fire;
  LRESULT OnTimer(UINT_PTR nIDEvent);
};
