#pragma once

#include "..\..\Common\LogService.h"

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>


#define MAXLOGMSGLEN  1024 * 4

class CHSLogger: public ILogService
{
public:
	CHSLogger(void);
	~CHSLogger(void);

	void LogInit(CString strCfgFile);
	void SimpleLog(long nLogPriority,CString strLoggerName,CString strMsg);
	void FormatLog(long nLogPriority,CString strLoggerName,CString strFormat,va_list args);
	void HexdumpLog (long nLogPriority,CString strLoggerName,const char *buffer,size_t size,CString strText);

	UINT FormatHexdump(const char *buffer,size_t size,TCHAR *obuf,size_t obuf_sz);
protected:
	int m_nInitFlag;
};
