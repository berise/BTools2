//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

#include "StdAfx.h"
#include "ScreenLib.h"

#define MARGIN_SIZE 7

CScreenLib::CScreenLib(void)
{
}

CScreenLib::~CScreenLib(void)
{
}

// Dock a control (e.g. listview) to a screen edge or fill the entire screen
// hwndDlg: Handle of the parent window
// nIDAffectedCtl: ID of the affected control
// nType: Type of docking requested (see DockType enum)

void CScreenLib::DockControl(HWND hwndDlg, UINT nIDAffectedCtl, DockType nType /*=dtFill*/)
{
	ASSERT(hwndDlg != NULL);
	RECT rcClient;
	::GetClientRect(hwndDlg, &rcClient);

	// Verify the affected control and obtain its rect
	HWND hwndCtl = ::GetDlgItem(hwndDlg, nIDAffectedCtl);
	ASSERT(hwndCtl != NULL);
	RECT rcCtl;
	::GetWindowRect(hwndCtl, &rcCtl);

	int nLeft, nTop, nWidth, nHeight;

	switch (nType)
	{
		case dtLeft:
			nLeft = 0;
			nTop = 0;
			nWidth = rcCtl.right - rcCtl.left;
			nHeight = rcClient.bottom - rcClient.top;
			break;

		case dtRight:
			nLeft = (rcClient.right - rcClient.left) - (rcCtl.right - rcCtl.left);
			nTop = 0;
			nWidth = rcCtl.right - rcCtl.left;
			nHeight = rcClient.bottom - rcClient.top;
			break;

		case dtTop:
			nLeft = 0;
			nTop = 0;
			//nWidth = rcClient.right - rcClient.left;
			// keep original width
			nWidth = rcCtl.right - rcCtl.left;
			nHeight = rcCtl.bottom - rcCtl.top;
			break;

		case dtBottom:
			nLeft = 0;
			nTop = rcClient.bottom - (rcCtl.bottom - rcCtl.top);
			nWidth = rcClient.right - rcClient.left;
			nHeight = rcCtl.bottom - rcCtl.top;
			break;

		case dtFill:
			__fallthrough;
			

		default:
			nLeft = 0;
			nTop = 0;
			nWidth = rcClient.right - rcClient.left;
			nHeight = rcClient.bottom - rcClient.top;
			break;
	}

	::MoveWindow(hwndCtl, nLeft, nTop, nWidth, nHeight, TRUE);
}

// Align a control or group of controls relative to one of the edges of a fixed control
// hwndDlg: Handle of the parent window
// nType: Type of alignment requested (see AlignType enum)
// cAffectedCtls: Number of controls affected
// nIDFixedCtl: ID of the fixed control
// nIDAffectedCtl: ID's of the affected control(s)

void CScreenLib::AlignControls(
	HWND hwndDlg, 
	AlignType nType, 
	int cAffectedCtls, 
	UINT nIDFixedCtl, 
	UINT nIDAffectedCtl,
	...)
{
	ASSERT(hwndDlg != NULL);
	ASSERT(cAffectedCtls > 0); // must have at least 1 movable control
	
	// Verify the fixed control and obtain its rect
	HWND hwndFixedCtl = ::GetDlgItem(hwndDlg, nIDFixedCtl);
	ASSERT(hwndFixedCtl != NULL);
	RECT rcFixed;
	::GetWindowRect(hwndFixedCtl, &rcFixed);
	
	// Initialize variable arguments.
	va_list marker;
	va_start(marker, nIDAffectedCtl);     

	UINT nIDCurrentCtl = nIDAffectedCtl;

	while (cAffectedCtls-- > 0)
	{
		// Verify the current movable control and obtain its rect
		HWND hwndIDCurrent = ::GetDlgItem(hwndDlg, nIDCurrentCtl);
		ASSERT(hwndIDCurrent != NULL);
		RECT rcCurrent;
		::GetWindowRect(hwndIDCurrent, &rcCurrent);

		POINT ptMoveTo;

		switch (nType)
		{
			case atLeft:
				ptMoveTo.x = rcFixed.left;
				ptMoveTo.y = rcCurrent.top;
				break;

			case atRight:
				ptMoveTo.x = rcFixed.right - (rcCurrent.right - rcCurrent.left);			
				ptMoveTo.y = rcCurrent.top;
				break;
	
			case atTop:
				ptMoveTo.x = rcCurrent.left;
				ptMoveTo.y = rcFixed.top;
				break;

			case atBottom:
				ptMoveTo.x = rcCurrent.left;
				ptMoveTo.y = rcFixed.bottom - (rcCurrent.bottom - rcCurrent.top);
				break;				
		}

		::ScreenToClient(hwndDlg, &ptMoveTo);
	
		::SetWindowPos(hwndIDCurrent, NULL, ptMoveTo.x, ptMoveTo.y, 
			0, 0, SWP_NOSIZE|SWP_NOZORDER);

		nIDCurrentCtl = va_arg(marker, UINT);
	}

	va_end(marker);
}

// Make a control or group of controls the same size (width or height) as a fixed control
// hwndDlg: Handle of the parent window
// nType: Type of sizing requested (see SizeType enum)
// cAffectedCtls: Number of controls that will be resized
// nIDFixedCtl: ID of the fixed control
// nIDAffectedCtl: ID's of the affected controls

void CScreenLib::MakeSameSize(
	HWND hwndDlg, 
	SizeType nType, 
	int cAffectedCtls, 
	UINT nIDFixedCtl, 
	UINT nIDAffectedCtl, 
	...)
{
	ASSERT(hwndDlg != NULL);
	ASSERT(cAffectedCtls > 0); // must have at least 1 movable control

	// Verify the fixed control and obtain its rect
	HWND hwndFixedCtl = ::GetDlgItem(hwndDlg, nIDFixedCtl);
	ASSERT(hwndFixedCtl != NULL);
	RECT rcFixed;
	::GetWindowRect(hwndFixedCtl, &rcFixed);
	
	// Initialize variable arguments.
	va_list marker;
	va_start(marker, nIDAffectedCtl);     

	UINT nIDCurrentCtl = nIDAffectedCtl;

	while (cAffectedCtls-- > 0)
	{
		// Verify the current movable control and obtain its rect
		HWND hwndIDCurrent = ::GetDlgItem(hwndDlg, nIDCurrentCtl);
		ASSERT(hwndIDCurrent != NULL);
		RECT rcCurrent;
		::GetWindowRect(hwndIDCurrent, &rcCurrent);

		int nWidth, nHeight;

		switch (nType)
		{
			case stWidth:
				nWidth = rcFixed.right - rcFixed.left;
				nHeight	= rcCurrent.bottom - rcCurrent.top;
				break;

			case stHeight:
				nWidth = rcCurrent.right - rcCurrent.left;
				nHeight	= rcFixed.bottom - rcFixed.top;
				break;
		}

		::SetWindowPos(hwndIDCurrent, NULL, 0, 0, nWidth, nHeight, 
			SWP_NOMOVE|SWP_NOZORDER);

		nIDCurrentCtl = va_arg(marker, UINT);
	}

	va_end(marker);
}

// Move and resize a control or group of controls to fit optimally in the parent 
// window leaving a small margin on the left and right
// hwndDlg: Handle of the parent window
// cAffectedCtls: Number of controls that will be moved and resized
// nIDAffectedCtl: ID's of controls that will be moved and resized

void CScreenLib::OptimizeWidth(
	HWND hwndDlg, 
	int cAffectedCtls, 
	UINT nIDAffectedCtl, 
	...)
{
	ASSERT(hwndDlg != NULL);
	ASSERT(cAffectedCtls > 0); // must have at least 1 affected control

	RECT rcClient;
	::GetClientRect(hwndDlg, &rcClient);
	
	int nMargin = DRA::SCALEX(MARGIN_SIZE);
	int nNewCtlWidth = (rcClient.right - rcClient.left) - (nMargin * 2); 

	// Initialize variable arguments.
	va_list marker;
	va_start(marker, nIDAffectedCtl);     

	UINT nIDCurrentCtl = nIDAffectedCtl;

	while (cAffectedCtls-- > 0)
	{
		// Verify the current movable control and obtain its rect
		HWND hwndIDCurrent = ::GetDlgItem(hwndDlg, nIDCurrentCtl);
		ASSERT(hwndIDCurrent != NULL);
		RECT rcCurrent;
		::GetWindowRect(hwndIDCurrent, &rcCurrent);

		POINT ptMoveTo;
		ptMoveTo.x = rcCurrent.left;
		ptMoveTo.y = rcCurrent.top;
		::ScreenToClient(hwndDlg, &ptMoveTo);

		::MoveWindow(hwndIDCurrent, nMargin, ptMoveTo.y, 
			nNewCtlWidth, rcCurrent.bottom - rcCurrent.top, TRUE);
	
		nIDCurrentCtl = va_arg(marker, UINT);
	}
}

// Resize a control to make it tall enough to fit optimally in the parent window 
// leaving a small margin at the bottom
// hwndDlg: Handle of the parent window
// nIDAffectedCtl: ID of the control that will be resized

void CScreenLib::OptimizeHeight(HWND hwndDlg, UINT nIDAffectedCtl)
{
	ASSERT(hwndDlg != NULL);
	RECT rcClient;
	::GetClientRect(hwndDlg, &rcClient);

	// Verify the affected control and obtain its rect
	HWND hwndCtl = ::GetDlgItem(hwndDlg, nIDAffectedCtl);
	ASSERT(hwndCtl != NULL);
	RECT rcCtl;
	::GetWindowRect(hwndCtl, &rcCtl);
	
	POINT ptMoveTo;
	ptMoveTo.x = rcCtl.left;
	ptMoveTo.y = rcCtl.top;
	::ScreenToClient(hwndDlg, &ptMoveTo);

	::SetWindowPos(hwndCtl, NULL, 0, 0,
		rcCtl.right - rcCtl.left,
		rcClient.bottom - rcClient.top - ptMoveTo.y - rcCtl.left,
		SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
}

#ifndef WIN32_PLATFORM_WFSP
// The following code is not applicable to Smartphone because buttons 
// are not used in Smartphone user interfaces. Use for Pocket PC only.

// Arrange a button or group of buttons relative to a fixed button
// hwndDlg: Handle of the parent window

void CScreenLib::ArrangeButtons(
	HWND hwndDlg, 
	ButtonPlacement nType, 
	int cAffectedCtls, 
	UINT nIDFixedCtl, 
	UINT nIDAffectedCtl, 
	...)
{
	ASSERT(hwndDlg != NULL);

	// Verify the fixed control and obtain its rect
	HWND hwndFixedCtl = ::GetDlgItem(hwndDlg, nIDFixedCtl);
	ASSERT(hwndFixedCtl != NULL);
	RECT rcFixed;
	::GetWindowRect(hwndFixedCtl, &rcFixed);

	int nMargin = DRA::SCALEX(MARGIN_SIZE);

	// Initialize variable arguments.
	va_list marker;
	va_start(marker, nIDAffectedCtl);     

	UINT nIDCurrentCtl = nIDAffectedCtl;

	while (cAffectedCtls-- > 0)
	{
		// Verify the current movable control and obtain its rect
		HWND hwndIDCurrent = ::GetDlgItem(hwndDlg, nIDCurrentCtl);
		ASSERT(hwndIDCurrent != NULL);
		RECT rcCurrent;
		::GetWindowRect(hwndIDCurrent, &rcCurrent);

		POINT ptMoveTo;

		switch (nType)
		{
			case bpBelow:
				ptMoveTo.x = rcFixed.left;
				ptMoveTo.y = rcFixed.bottom + nMargin;
				break;
	
			case bpRight:
				ptMoveTo.x = rcFixed.right + nMargin;
				ptMoveTo.y = rcFixed.top;
				break;	
		}

		::ScreenToClient(hwndDlg, &ptMoveTo);

		::SetWindowPos(hwndIDCurrent, NULL, ptMoveTo.x, ptMoveTo.y, 
			0, 0, SWP_NOSIZE|SWP_NOZORDER);

		// Obtain the rect of the control that was just moved
		::GetWindowRect(hwndIDCurrent, &rcCurrent);
		
		// Set rcFixed to rcCurrent so that the next control in the loop 
		// is placed relative to the last moved control
		rcFixed = rcCurrent;

		nIDCurrentCtl = va_arg(marker, UINT);
	}
}

// Move and resize a "primary control" and a button control on its right to optimally fit in 
// the parent window. 
// hwndDlg: Handle of the parent window

void CScreenLib::OptimizeWidthWithRightButton(
	HWND hwndDlg, 
	UINT nIDPrimaryCtl, 
	UINT nIDButtonCtl, 
	BOOL bOptimizePrimaryCtlHeight/*=FALSE*/)
{
	ASSERT(hwndDlg != NULL);
	RECT rcClient;
	::GetClientRect(hwndDlg, &rcClient);

	// Verify the primary control and obtain its rect
	HWND hwndPrimaryCtl = ::GetDlgItem(hwndDlg, nIDPrimaryCtl);
	ASSERT(hwndPrimaryCtl != NULL);
	RECT rcPrimary;
	::GetWindowRect(hwndPrimaryCtl, &rcPrimary);

	// Verify the button control and obtain its rect
	HWND hwndButtonCtl = ::GetDlgItem(hwndDlg, nIDButtonCtl);
	ASSERT(hwndButtonCtl != NULL);
	RECT rcButton;
	::GetWindowRect(hwndButtonCtl, &rcButton);

	int nMargin = DRA::SCALEX(MARGIN_SIZE);

	int nPrimaryCtlWidth =	
		(rcClient.right - rcClient.left) - 
		(rcButton.right - rcButton.left) - 
		(nMargin * 3);

	int nPrimaryCtlHeight = rcPrimary.bottom - rcPrimary.top;

	POINT ptMoveTo;
	ptMoveTo.x = nMargin;
	ptMoveTo.y = rcPrimary.top;
	::ScreenToClient(hwndDlg, &ptMoveTo);

	// Set the primary control's width...
	::MoveWindow(hwndPrimaryCtl, ptMoveTo.x, ptMoveTo.y, 
		nPrimaryCtlWidth, nPrimaryCtlHeight, TRUE);

	// Adjust the primary control's height if necessary

	if (bOptimizePrimaryCtlHeight)
	{
		OptimizeHeight(hwndDlg, nIDPrimaryCtl);
	}

	// Now position the button to the right of the primary control.
	// But first, obtain the new rect of the primary control.

	::GetWindowRect(hwndPrimaryCtl, &rcPrimary);

	ptMoveTo.x = rcPrimary.right;
	ptMoveTo.y = rcPrimary.top;
	::ScreenToClient(hwndDlg, &ptMoveTo);

	::SetWindowPos(hwndButtonCtl, NULL, 
		ptMoveTo.x + nMargin, ptMoveTo.y, 0, 0, 
		SWP_NOSIZE|SWP_NOZORDER);
}

#endif  /* !WIN32_PLATFORM_WFSP */