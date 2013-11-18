#include "stdafx.h"
#include "NetEngineStructDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConnectInfo::ConnectInfo()
{
	memset(this,0,sizeof(ConnectInfo));
}
void ConnectInfo::Copy(ConnectInfo *conn)
{
	if (conn)
	{
		m_nPort = conn->m_nPort;
		memcpy(m_strServerIP,conn->m_strServerIP,128);
		memcpy(m_strServerID,conn->m_strServerID,128);
	}
}


SiteInfo::SiteInfo()
{
	m_cType = CEV_Connect_HQ;
	m_pDefault = NULL;
}
SiteInfo::~SiteInfo()
{
	Free();
}
void SiteInfo::Free()
{
	for (int i=0; i<m_ayConn.GetCount(); i++)
	{
		ConnectInfo *pSrv = m_ayConn.GetAt(i);
		delete pSrv;
	}
	m_ayConn.RemoveAll();
}
void SiteInfo::Copy(SiteInfo *pSite)
{
	if (pSite)
	{
		Free();
		m_cType = pSite->m_cType;
		for (int i=0; i<pSite->m_ayConn.GetCount(); i++)
		{
			ConnectInfo *pSrv = new ConnectInfo;
			pSrv->Copy(pSite->m_ayConn.GetAt(i));
			m_ayConn.Add(pSrv);
		}
	}
}
ConnectInfo *SiteInfo::Find(char *pChar)
{
	for (int i=0; i<m_ayConn.GetCount(); i++)
	{
		ConnectInfo *pSrv = m_ayConn.GetAt(i);
		if (!strcmp(pChar,pSrv->m_strServerID))
		{
			return pSrv;
		}
	}
	return NULL;
}
ConnectInfo *SiteInfo::FindNext(char *pChar)
{
	for (int i=0; i<m_ayConn.GetCount(); i++)
	{
		ConnectInfo *pSrv = m_ayConn.GetAt(i);
		if ( pChar)
		{
			if (!strcmp(pChar,pSrv->m_strServerID))
			{
				if ( i+1 < m_ayConn.GetCount())
				{
					pSrv = m_ayConn.GetAt(i+1);
					return pSrv;
				}
				return NULL;
			}
		}
		else
		{
			ConnectInfo *pSrv = m_ayConn.GetAt(0);
			return pSrv;
		}
	}
	return NULL;
}
BOOL SiteInfo::SetDefault(char *pName)
{
	for(int i=0; i<m_ayConn.GetCount(); i++)
	{
		ConnectInfo *pSrv = m_ayConn.GetAt(i);
		if (!strcmp(pName,pSrv->m_strServerID))
		{
			m_pDefault = pSrv;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL SiteInfo::IsDefault(char *pName)
{
	if (!m_pDefault)
	{
		return FALSE;
	}
	if (!strcmp(m_pDefault->m_strServerID,pName))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL  SiteInfo::Delete(ConnectInfo *pConn)
{
	for (int i=0; i<m_ayConn.GetCount(); i++)
	{
		ConnectInfo *pC = m_ayConn.GetAt(i);
		if (pConn == pC)
		{
			delete pC;
			m_ayConn.RemoveAt(i);
			if (pConn == m_pDefault)
			{
				if (m_ayConn.GetCount() > 0)
				{
					m_pDefault = m_ayConn.GetAt(0);
				}
				else
				{
					m_pDefault = NULL;
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}
ServerInfo::ServerInfo()
{
	m_pProxy = NULL;
}
ServerInfo::~ServerInfo()
{
	Free();
}
void ServerInfo::Free()
{
	if (m_pProxy)
	{
		delete m_pProxy;
		m_pProxy = NULL;
	}
	for(int i = 0; i<m_aySites.GetCount(); i++)
	{
		SiteInfo *pSite = m_aySites.GetAt(i);
		delete pSite;
	}
	m_aySites.RemoveAll();
}
ConnectInfo *ServerInfo::GetDefult(Server_type type)
{
	SiteInfo *pSite = GetSite(type);
	if (pSite)
	{
		ConnectInfo *pConn = pSite->m_pDefault;
		if (!pConn)
		{
			pConn = pSite->m_ayConn.GetAt(0);
		}
		return pConn;
	}
	return NULL;
}
ConnectInfo *ServerInfo::GetConnInfo(Server_type type,char *pName)
{
	if (!pName)
	{
		return NULL;
	}
	SiteInfo *pSite = GetSite(type);
	if (pSite)
	{
		return pSite->Find(pName);
	}
	return NULL;
}
void ServerInfo::AddConn(Server_type type,ConnectInfo *conn,BOOL bdefault)
{
	if (!conn)
	{
		return;
	}
	SiteInfo  *pSite = GetSite(type);
	if (!pSite)
	{
		pSite = new SiteInfo;
		m_aySites.Add(pSite);
		pSite->m_cType = type;
	}
	ConnectInfo *pConn = new ConnectInfo;
	pConn->Copy(conn);
	pSite->m_ayConn.Add(pConn);
	if (bdefault)
	{
		pSite->m_pDefault = pConn;
	}
}
BOOL ServerInfo::DeleteConn(Server_type type,ConnectInfo *pConn)
{
	SiteInfo *pSite = GetSite(type);
	if (pSite)
	{
		return pSite->Delete(pConn);
	}
	return FALSE;
}
SiteInfo *ServerInfo::GetSite(Server_type type)
{
	for (int i=0; i<m_aySites.GetCount(); i++)
	{
		SiteInfo *pSite = m_aySites.GetAt(i);
		if (pSite->m_cType == type)
		{
			return pSite;
		}
	}
	return NULL;
}
void ServerInfo::RemoveSite(Server_type type)
{
	for (int i=0; i<m_aySites.GetCount(); i++)
	{
		SiteInfo *pSite = m_aySites.GetAt(i);
		if (pSite->m_cType == type)
		{
			m_aySites.RemoveAt(i);
			delete pSite;
			return;
		}
	}
}


void BetsRange::SetOnlineRadioRange(RangeItem *pOnline,int count)
{
	if (pOnline && count > 0)
	{
		for (int i=0;i < count; i++)
		{
			RangeItem item = pOnline[i];
			m_onlineRatioList.AddTail(item);
		}
	}	
}
void BetsRange::SetHandleTimeRange(RangeItem *pHandleTime,int count)
{
	if (pHandleTime && count > 0)
	{
		for (int i=0;i < count; i++)
		{
			RangeItem item = pHandleTime[i];
			m_reqHandleTimeList.AddTail(item);
		}
	}	
}
float BetsRange::GetOnLineRatioBets(float fVlaue)
{
	POSITION pos = m_onlineRatioList.GetHeadPosition();
	for (int i=0;i < m_onlineRatioList.GetCount();i++)
	{
		RangeItem item = m_onlineRatioList.GetNext(pos);
		if(fVlaue >= item.fBegin && fVlaue < item.fEnd)
			return item.fBets;
	}
	return -1;
}
float BetsRange::GetReqHandleTimeBets(float fVlaue)
{
	POSITION pos = m_reqHandleTimeList.GetHeadPosition();
	for (int i=0;i < m_reqHandleTimeList.GetCount();i++)
	{
		RangeItem item = m_reqHandleTimeList.GetNext(pos);
		if(fVlaue >= item.fBegin && fVlaue < item.fEnd)
			return item.fBets;
	}
	return -1;
}


