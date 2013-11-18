#include "stdafx.h"
#include "TradePacker.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern NetEngineJYAPI g_NetJYAPI;

CTradePacker::CTradePacker()
{
	m_hPM = g_NetJYAPI.JYCreatePM();
}
CTradePacker::~CTradePacker()
{
	g_NetJYAPI.JYDestroyPM(m_hPM);
	m_hPM = 0;
}

int CTradePacker::ParseData(const char *pData,UINT len)
{
	return g_NetJYAPI.JYParse(m_hPM,pData,len);
}

int CTradePacker::GetHeader(EWholePacketHeader index, UINT &ivalue)
{
	return g_NetJYAPI.JYGetHeader(m_hPM,index,ivalue);
}

int CTradePacker::SetHeader(EWholePacketHeader index, UINT ivalue)
{
	return g_NetJYAPI.JYSetHeader(m_hPM,index,ivalue);
}

char* CTradePacker::GetPackData(UINT &len)
{
	return g_NetJYAPI.GetPackData(m_hPM,len);
}

char* CTradePacker::GetData(CString sName, UINT& iDataLen)
{
	return g_NetJYAPI.GetData(m_hPM,sName,iDataLen);
}