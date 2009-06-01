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

#pragma once

#ifndef _WIN32_WCE
	#error ScreenLib.h is only supported on Windows CE platforms.
#endif

#ifndef __cplusplus
	#error ScreenLib requires C++ compilation (use a .cpp suffix)
#endif 

class CScreenLib
{
public:

	enum DockType
	{
		dtFill,
		dtLeft,
		dtRight,
		dtTop,
		dtBottom
	};
	
	enum AlignType
	{
		atLeft,
		atRight,
		atTop,
		atBottom
	};

	enum SizeType
	{
		stWidth,
		stHeight
	};

	CScreenLib(void);
	~CScreenLib(void);

	// Dock a control (e.g. listview) to a screen edge or fill the entire screen
	static void DockControl(HWND hwndDlg, UINT nIDAffectedCtl, DockType nType = dtFill);

	// Move and resize a control or group of controls to fit optimally in the parent 
	// window leaving a small margin on the left and right
	static void OptimizeWidth(HWND hwndDlg, int cAffectedCtls, UINT nIDAffectedCtl, ...);

	// Resize a control to make it tall enough to fit optimally in the parent window 
	// leaving a small margin at the bottom
	static void OptimizeHeight(HWND hwndDlg, UINT nIDAffectedCtl);

	// Align a control or group of controls relative to one of the edges of a fixed control
	static void AlignControls(HWND hwndDlg, AlignType nType, int cAffectedCtls, UINT nIDFixedCtl, UINT nIDAffectedCtl, ...);
	
	// Make a control or group of controls the same size (width or height) as a fixed control
	static void MakeSameSize(HWND hwndDlg, SizeType nType, int cAffectedCtls, UINT nIDFixedCtl, UINT nIDAffectedCtl, ...);

#ifndef WIN32_PLATFORM_WFSP
	// This code is not applicable to Smartphone because buttons are not used in 
	// Smartphone user interfaces. Use for Pocket PC only.
	
	enum ButtonPlacement
	{
		bpBelow,
		bpRight
	};

	// Arrange a button or group of buttons relative to a fixed button
	static void ArrangeButtons(HWND hwndDlg, ButtonPlacement nType, int cAffectedCtls, UINT nIDFixedCtl, UINT nIDAffectedCtl, ...);

	// Move and resize a "primary control" and a button control on its right to optimally fit in 
	// the parent window. 
	static void OptimizeWidthWithRightButton(HWND hwndDlg, UINT nIDPrimaryCtl, UINT nIDButtonCtl, BOOL bOptimizePrimaryCtlHeight=FALSE);
#endif  /* !WIN32_PLATFORM_WFSP */
};