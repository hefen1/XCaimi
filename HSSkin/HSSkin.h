// HSSkin.h : main header file for the HSSkin DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "HSSkinIntf.h"
#include "DLLEntryBase.h"


// CHSSkinApp
// See HSSkin.cpp for the implementation of this class
//

class CHSSkinApp : public CWinApp
{
public:
	CHSSkinApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
class CHSSkinObj : public IHSSkinObj
{
public:
	CHSSkinObj();
	~CHSSkinObj();
	void Release();
	void FreeCache();
	BOOL LoadSkin(CString sFileName);
	BOOL SetActive(BOOL bFlag);
	BOOL SetSkinFormType(int nType /* = 0 */);
	void SkinForm(HWND nHandle);
private:
	HMODULE m_hModule;
};

