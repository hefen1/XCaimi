// TradeFund.h : main header file for the TradeFund DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTradeFundApp
// See TradeFund.cpp for the implementation of this class
//

class CTradeFundApp : public CWinApp
{
public:
	CTradeFundApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
