#pragma once

#include "LogService.h"
#include "HSWinner.h"

//log name define
#define  LN_WINNER       _T("winner")

typedef HMODULE (PASCAL *GETMODULEHANDLE)(const char* szModuleName);
typedef HMODULE (PASCAL *GETOBJECT)(const char* szObjectLocation,const char* szPrefix);

#define HSPARAM_XMLDATA   _T("HSPARAM_XMLDATA")
class CHSParam: public IHSParam
{
private:
	CMapStringToString m_valueMap;
public:
	void AddParam(CString strParamName,CString strValue);

	virtual CString WINAPI GetStringParam(CString strParamName,CString strDefault = _T(""));
	virtual int WINAPI GetIntegerParam(CString strParamName,int nDefault = 0);
	virtual double WINAPI GetDoubleParam(CString strParamName,double nDefault = 0);
	virtual CString GetXMLData();
	virtual void SetXMLData(CString str);
};

class CWinnerApplication
{
private:
	static HMODULE m_hAppModule;
	static HMODULE GetAppHandle();
public:
	//根据指定名称获取模块句柄
	static HMODULE GetModuleHandle(CString strModuleName);
	//根据对象名获取对象实例
	static void* GetObject(CString strObjectLocatin,CString strPerfix = _T(""));


	static void SimpleLog(long nLogPriority,CString strLoggerName,CString strMsg);

	static void FormatLog(long nLogPriority,CString strLoggerName,CString strFormat,...);

	static void HexdumpLog (long nLogPriority,CString strLoggerName,const char *buffer,size_t size,
		CString strText);
};