#include "stdafx.h"
#include "WinnerApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HMODULE CWinnerApplication::m_hAppModule = NULL;

void CHSParam::AddParam(CString strParamName,CString strValue)
{
	m_valueMap.SetAt(strParamName,strValue);
}

CString CHSParam::GetStringParam(CString strParamName,CString strDefault /*= _T("")*/)
{
	CString strValue;
	if ( m_valueMap.Lookup(strParamName,strValue) )
		return strValue;
	else
		return strDefault;
}

int CHSParam::GetIntegerParam(CString strParamName,int nDefault /* = 0*/)
{
	CString strValue;
	if ( m_valueMap.Lookup(strParamName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atoi(strValue);
	}
	return nDefault;
}

double CHSParam::GetDoubleParam(CString strParamName,double nDefault/* = 0*/)
{
	CString strValue;
	if ( m_valueMap.Lookup(strParamName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atof(strValue);
	}
	return nDefault;
}

CString CHSParam::GetXMLData()
{
	return GetStringParam(HSPARAM_XMLDATA);
}

void CHSParam::SetXMLData(CString str)
{
	AddParam(HSPARAM_XMLDATA,str);
}

HMODULE CWinnerApplication::GetAppHandle()
{
	CString strPath;
	DWORD dwSize=MAX_PATH;
	::GetModuleFileName(NULL,strPath.GetBuffer(MAX_PATH),dwSize); //AfxGetResourceHandle()
	strPath.ReleaseBuffer(dwSize);
	CString strAppPath=strPath.Left(strPath.ReverseFind('\\')+1);
	CString strAppFile = strAppPath + _T("Winner.exe");
	return ::GetModuleHandle(strAppFile);
}

HMODULE CWinnerApplication::GetModuleHandle(CString strModuleName)
{
	if(m_hAppModule == NULL )
		m_hAppModule = GetAppHandle();

	if(m_hAppModule)
	{
		static GETMODULEHANDLE pGetModuleHand = (GETMODULEHANDLE)::GetProcAddress(m_hAppModule,_T("HswGetModuleHandle"));
		if(pGetModuleHand)
		{
			return pGetModuleHand(strModuleName);
		}
	}
	return NULL;
}	

void* CWinnerApplication::GetObject(CString strObjectLocatin,CString strPerfix/* = _T("")*/)
{
	if(m_hAppModule == NULL )
		m_hAppModule = GetAppHandle();

	if(m_hAppModule)
	{
		static GETOBJECT pGetObject = (GETOBJECT)::GetProcAddress(m_hAppModule,_T("HswGetObject"));
		if(pGetObject)
		{
			return pGetObject(strObjectLocatin,strPerfix);
		}
	}
	return NULL;
}

void CWinnerApplication::SimpleLog(long nLogPriority,CString strLoggerName,CString strMsg)
{
	ILogService* pLog = (ILogService*)CWinnerApplication::GetObject(OBJ_LOGSEVICE);
	if(pLog)
	{
		pLog->SimpleLog(nLogPriority,strLoggerName,strMsg);
	}
}

void CWinnerApplication::FormatLog(long nLogPriority,CString strLoggerName,CString strFormat,...)
{
	ILogService* pLog = (ILogService*)CWinnerApplication::GetObject(OBJ_LOGSEVICE);
	if(pLog)
	{
		va_list argList;
		va_start( argList, strFormat);
		pLog->FormatLog(nLogPriority,strLoggerName,strFormat,argList);
		va_end( argList );
	}
}

void CWinnerApplication::HexdumpLog (long nLogPriority,CString strLoggerName,const char *buffer,size_t size,
						CString strText)
{
	ILogService* pLog = (ILogService*)CWinnerApplication::GetObject(OBJ_LOGSEVICE);
	if(pLog)
	{
		pLog->HexdumpLog(nLogPriority,strLoggerName,buffer,size,strText);
	}		
}