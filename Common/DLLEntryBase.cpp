/*******************************************************
  Դ��������:DLLEntryBase.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  DLL Entry Base Class
  ��    ��:  shenglq
  ��������:  20100720
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

