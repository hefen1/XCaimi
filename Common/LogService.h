#pragma once

const int OFF_LOG_LEVEL     = 60000;
const int FATAL_LOG_LEVEL   = 50000;
const int ERROR_LOG_LEVEL   = 40000;
const int WARN_LOG_LEVEL    = 30000;
const int INFO_LOG_LEVEL    = 20000;
const int DEBUG_LOG_LEVEL   = 10000;
const int TRACE_LOG_LEVEL   = 0;

#define OBJ_LOGSEVICE   _T("LogService.HSLogger")

struct ILogService 
{
	virtual void SimpleLog(long nLogPriority,CString strLoggerName,CString strMsg) = 0;

	virtual void FormatLog(long nLogPriority,CString strLoggerName,CString strFormat,va_list args) = 0;

	virtual void HexdumpLog (long nLogPriority,CString strLoggerName,const char *buffer,size_t size,
		CString strText) = 0;
};