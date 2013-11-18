/*******************************************************
  源程序名称:DLLEntryBase.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  DLL Entry Base Class
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "DLLEntryBase.h"
#include "WinnerApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDLLEntryBase::CDLLEntryBase()
{
	m_hinstDLL = NULL;
}

CDLLEntryBase::~CDLLEntryBase(){}

HINSTANCE CDLLEntryBase::GetDLLhinst()
{
	if (m_hinstDLL == NULL)
	{
		m_hinstDLL = CWinnerApplication::GetModuleHandle(GetDLLFileName());
	}	
	return m_hinstDLL;
}

FARPROC CDLLEntryBase::GetLibProcAddress(const CString sProcName)
{
	return  GetProcAddress(GetDLLhinst(), sProcName);
}

