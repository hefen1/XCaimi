// DataSource.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "DataSource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma   data_seg( ".hs_datasource_data") 
	CDataSourceImpl*	ds_pi_DataSourceImpl = NULL; 
	int					ds_nRefCount_DataSource = 0;
	DWORD				ds_dwID_firstProcess = 0;
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

// CDataSourceApp
/*
BEGIN_MESSAGE_MAP(CDataSourceApp, CWinApp)
END_MESSAGE_MAP()


// CDataSourceApp construction

CDataSourceApp::CDataSourceApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDataSourceApp object

//CDataSourceApp theApp;


// CDataSourceApp initialization

BOOL CDataSourceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

*/

extern "C" IDataSource* WINAPI	GetServiceObject(const char* szName,const char* szPrefix)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (ds_pi_DataSourceImpl == NULL )
	{
		ds_pi_DataSourceImpl = new CDataSourceImpl;
//		ds_pi_DataSourceImpl = (CDataSourceImpl*)GlobalAlloc(GMEM_SHARE, sizeof(CDataSourceImpl));
		ds_dwID_firstProcess = GetCurrentProcessId();
	}
	else
	{
/*		if (ds_dwID_firstProcess > 0)
		{
			HANDLE hFirstProcess = OpenProcess(PROCESS_VM_READ, NULL, ds_dwID_firstProcess);
			if (hFirstProcess != NULL)
			{
				DWORD dwCurProcessID = GetCurrentProcessId();
				HANDLE hCurrentProcess = OpenProcess(PROCESS_VM_READ, NULL, dwCurProcessID);
				if (hCurrentProcess != NULL)
				{
					HANDLE hCurrentDataSourceImpl = NULL;
					if(DuplicateHandle(hFirstProcess, (HANDLE)ds_pi_DataSourceImpl, hCurrentProcess, &hCurrentDataSourceImpl, 0, FALSE, DUPLICATE_SAME_ACCESS ))
					{
						ds_nRefCount_DataSource ++;
						return (IDataSource*)hCurrentDataSourceImpl;
					}
					else
					{
						DWORD dwErr = GetLastError();
						int a = 0;
					}
				}
				CloseHandle(hCurrentProcess);
			}
			CloseHandle(hFirstProcess);
		}*/
	}
	return ds_pi_DataSourceImpl;	
}

extern "C" int WINAPI InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ds_nRefCount_DataSource ++;
	return 0;
}

extern "C" int WINAPI UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ds_nRefCount_DataSource --;
	if(ds_nRefCount_DataSource <= 0)
	{
		delete ds_pi_DataSourceImpl;
		ds_pi_DataSourceImpl = NULL;
	}
	return 0;
}