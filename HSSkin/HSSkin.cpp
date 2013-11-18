// HSSkin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HSSkin.h"
#include "HSWinner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma   data_seg( "._hsskin_data") 
IHSSkinObj*	pHSSkinObj = NULL; 
#pragma   data_seg() 

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CHSSkinApp

BEGIN_MESSAGE_MAP(CHSSkinApp, CWinApp)
END_MESSAGE_MAP()


// CHSSkinApp construction

CHSSkinApp::CHSSkinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CHSSkinApp object

CHSSkinApp theApp;


// CHSSkinApp initialization

BOOL CHSSkinApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

IHSSkinObj* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	return pHSSkinObj;
}

int PASCAL InitEntity()
{
	if (!pHSSkinObj)
	{
		pHSSkinObj = new CHSSkinObj;
	}
	return TRUE;
}

int PASCAL UninitEntity()
{
	pHSSkinObj->Release();
	pHSSkinObj = NULL;
	return TRUE;
}

int PASCAL OnFrameNotify(int nFlag)
{
	if (SN_BEFORE_FRAME_CLOSE == nFlag)
	{
		pHSSkinObj->FreeCache();
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
CHSSkinObj::CHSSkinObj()
{
	m_hModule = NULL;

}

CHSSkinObj::~CHSSkinObj()
{

}

void CHSSkinObj::Release()
{
	delete this;
}

void CHSSkinObj::FreeCache()
{
	if (m_hModule)
	{
		FreeLibrary (m_hModule);
	}
}

BOOL CHSSkinObj::LoadSkin(CString sFileName)
{
	typedef int(CALLBACK *EntryFunc)(PCHAR);
	BOOL bTmp = FALSE;
	if (!m_hModule)
	{
		m_hModule  = LoadLibrary ("Skin.dll");
	}
	if (m_hModule)
	{
		EntryFunc myFunc = (EntryFunc)GetProcAddress(m_hModule, "LoadSkin");
		if (myFunc != NULL)
		{
			//bTmp = (myFunc(hPM, (int)iIndex, iValue) == 0);
			CHAR szName [ MAX_PATH ];
			CString strName = "Skins\\DefaultSkin.skn";
			strcpy (szName, strName);
			bTmp = myFunc(szName) == 1;
		}
	}
	return bTmp;
}

BOOL CHSSkinObj::SetActive(BOOL bFlag)
{
	typedef int(CALLBACK *EntryFunc)(int);
	BOOL bTmp = FALSE;
	if (m_hModule)
	{
		EntryFunc myFunc = (EntryFunc)GetProcAddress(m_hModule, "SetActive");
		if (myFunc != NULL)
		{
			int nFlag = 0;
			if (bFlag)
			{
				nFlag = 1;
			}
			bTmp = myFunc(nFlag) == 1;
		}
	}
	return bTmp;
}

BOOL CHSSkinObj::SetSkinFormType(int nType )
{
	typedef int(CALLBACK *EntryFunc)(int);
	BOOL bTmp = FALSE;
	if (m_hModule)
	{
		EntryFunc myFunc = (EntryFunc)GetProcAddress(m_hModule, "SetSkinFormType");
		if (myFunc != NULL)
		{
			bTmp = myFunc(nType) == 1;
		}
	}
	return bTmp;
}

void CHSSkinObj::SkinForm(HWND nHandle)
{
	typedef int(CALLBACK *EntryFunc)(HWND);
	if (m_hModule)
	{
		EntryFunc myFunc = (EntryFunc)GetProcAddress(m_hModule, "SkinForm");
		if (myFunc != NULL)
		{
			myFunc(nHandle);
		}

	}
}