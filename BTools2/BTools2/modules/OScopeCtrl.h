// OScopeCtrl.h : header file
//

#ifndef __OScopeCtrl_H__
#define __OScopeCtrl_H__

#include "atlmisc.h"
#include "atlcrack.h"
//#include <vector>

/////////////////////////////////////////////////////////////////////////////
// COScopeCtrl window

class COScopeCtrl: public CWindowImpl<COScopeCtrl, CStatic>
{
// Construction
public:
  COScopeCtrl();


  DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(COScopeCtrl)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()



// Attributes
public:
	void AutoYRange(const double nMinMax);
  double AppendPoint(double dNewPoint);
  void SetRange(double dLower, double dUpper, int nDecimalPlaces=1);
  void SetXUnits(CString string);
  void SetYUnits(CString string);
  void SetGridColor(COLORREF color);
  void SetPlotColor(COLORREF color);
  void SetBackgroundColor(COLORREF color);
  void InvalidateCtrl();
  void DrawPoint();
  void Reset();

  // Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COScopeCtrl)
  public:
  //virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWindow* pParentWnd, UINT nID=NULL);
  //}}AFX_VIRTUAL

// Implementation
public:
  int m_nShiftPixels;          // amount to shift with each new point 
  int m_nYDecimals;

  CString m_strXUnitsString;
  CString m_strYUnitsString;

  COLORREF m_crBackColor;        // background color
  COLORREF m_crGridColor;        // grid color
  COLORREF m_crPlotColor;        // data color  
  
  double m_dCurrentPosition;   // current position
  double m_dPreviousPosition;  // previous position

  virtual ~COScopeCtrl();

  // Generated message map functions
protected:
  void OnPaint(CDCHandle);
  void OnSize(UINT nType, CSize size); 


  int m_nHalfShiftPixels;
  int m_nPlotShiftPixels;
  int m_nClientHeight;
  int m_nClientWidth;
  int m_nPlotHeight;
  int m_nPlotWidth;

  double m_dLowerLimit;        // lower bounds
  double m_dUpperLimit;        // upper bounds
  double m_dRange;
  double m_dVerticalFactor;
  
  CRect  m_rectClient;
  CRect  m_rectPlot;
  CPen   m_penPlot;
  CBrush m_brushBack;

  CDC     m_dcGrid;
  CDC     m_dcPlot;
  HBITMAP m_pbitmapOldGrid;
  HBITMAP m_pbitmapOldPlot;
  CBitmap m_bitmapGrid;
  CBitmap m_bitmapPlot;


  int m_nBufferSize;  
  int m_nDataIndex;  
  bool m_bWrapped;

  //std::vector<double> m_DataPoints;
  double *m_pdblDataPoints;
};

/////////////////////////////////////////////////////////////////////////////
#endif
