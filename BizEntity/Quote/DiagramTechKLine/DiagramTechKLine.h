// DiagramTechKLine.h : main header file for the DiagramTechKLine DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDiagramTechKLineApp
// See DiagramTechKLine.cpp for the implementation of this class
//

class CDiagramTechKLineApp : public CWinApp
{
public:
	CDiagramTechKLineApp();

	// ÖØÐ´
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};