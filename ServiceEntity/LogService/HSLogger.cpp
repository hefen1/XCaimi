#include "StdAfx.h"
#include "HSLogger.h"
using namespace log4cplus;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHSLogger::CHSLogger(void)
{
	m_nInitFlag = 0;
}

CHSLogger::~CHSLogger(void)
{
}

void CHSLogger::LogInit(CString strCfgFile)
{
	if(m_nInitFlag == 0 )
	{
		try 
		{ 
			PropertyConfigurator::doConfigure(strCfgFile.GetBuffer()); 
		} 
		catch ( ...) 
		{ 
		} 
		
		m_nInitFlag = 1;
	}
}

void CHSLogger::SimpleLog(long nLogPriority,CString strLoggerName,CString strMsg)
{
	Logger logger = Logger::getInstance(strLoggerName.GetBuffer());

	if(!logger.isEnabledFor(nLogPriority) )
		return;

	logger.log(nLogPriority, strMsg.GetBuffer());
}

void CHSLogger::FormatLog(long nLogPriority,CString strLoggerName,CString strFormat,va_list args)
{
	Logger logger = Logger::getInstance(strLoggerName.GetBuffer());	
	if(!logger.isEnabledFor(nLogPriority) )
		return;

	CString strTemp;
	strTemp.FormatV( strFormat, args );

	logger.log(nLogPriority, strTemp.GetBuffer());
}

void CHSLogger::HexdumpLog (long nLogPriority,CString strLoggerName,
							const char *buffer,size_t size,CString strText)
{
	Logger logger = Logger::getInstance(strLoggerName.GetBuffer());	
	if(!logger.isEnabledFor(nLogPriority) )
		return;

	const size_t buf_sz = MAXLOGMSGLEN;
	char *buf = new char[buf_sz];
	memset(buf,0,sizeof(char) * buf_sz);

	const size_t len = FormatHexdump(buffer, size, buf, buf_sz / sizeof (TCHAR) - 1);
	CString strTemp;
	strTemp.Format("%s-HEXDUMP %d bytes \n%s",strText,size,buf);

	delete[] buf; 
	logger.log(nLogPriority, strTemp.GetBuffer());
}

UINT CHSLogger::FormatHexdump(const char *buffer,size_t size,TCHAR *obuf,size_t obuf_sz)
{
	u_char c;
	TCHAR textver[16 + 1];

	// We can fit 16 bytes output in text mode per line, 4 chars per byte.
	size_t maxlen = (obuf_sz / 68) * 16;

	if (size > maxlen)
		size = maxlen;

	size_t i;

	size_t lines = size / 16;
	for (i = 0; i < lines; i++)
	{
		size_t j;
		for (j = 0 ; j < 16; j++)
		{
			c = (u_char) buffer[(i << 4) + j];    // or, buffer[i*16+j]
			sprintf (obuf,_T("%02x "),c);
			obuf += 3;
			if (j == 7)
			{
				sprintf (obuf,_T(" "));++obuf;
			}
			textver[j] = isprint (c) ? c : '.';
		}

		textver[j] = 0;

		sprintf (obuf,_T("  %s\n"),textver);

		while (*obuf != '\0')
			++obuf;
	}

	if (size % 16)
	{
		for (i = 0 ; i < size % 16; i++)
		{
			c = (u_char) buffer[size - size % 16 + i];
			sprintf (obuf,_T("%02x "),c);
			obuf += 3;
			if (i == 7)
			{
				sprintf (obuf,_T(" "));
				++obuf;
			}
			textver[i] = isprint (c) ? c : '.';
		}

		for (i = size % 16; i < 16; i++)
		{
			sprintf (obuf,_T("   "));
			obuf += 3;
			if (i == 7)
			{
				sprintf (obuf,_T(" "));
				++obuf;
			}
			textver[i] = ' ';
		}

		textver[i] = 0;
		sprintf (obuf,_T("  %s\n"),textver);
	}
	return size;
}