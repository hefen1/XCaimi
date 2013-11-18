// ConfigCore.h : main header file for the ConfigCore DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CConfigCoreApp
// See ConfigCore.cpp for the implementation of this class
//

class CConfigCoreApp : public CWinApp
{
public:
	CConfigCoreApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
