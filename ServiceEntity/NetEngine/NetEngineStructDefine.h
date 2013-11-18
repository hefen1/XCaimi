#pragma once
#include "..\..\Common\NetEngineInterface.h"

struct ConnectInfo
{
	char	    m_strServerID[128];		//服务器名称
	char	    m_strServerIP [128];	   // IP
	unsigned short  m_nPort;		  // port
	ConnectInfo();
	void Copy(ConnectInfo *conn);
};
struct SiteInfo
{
	Server_type m_cType;	 //服务器类型
	CArray<ConnectInfo*,ConnectInfo*> m_ayConn;
	ConnectInfo *m_pDefault;
	SiteInfo();
	~SiteInfo();
	void Free();
	void Copy(SiteInfo *pSite);
	ConnectInfo *Find(char *pChar);
	ConnectInfo *FindNext(char *pChar);
	BOOL SetDefault(char *pName);
	BOOL IsDefault(char *pName);
	BOOL Delete(ConnectInfo *pConn);
};

struct ServerInfo
{
	CArray<SiteInfo*,SiteInfo*> m_aySites;
	ProxyInfo *m_pProxy;
	ServerInfo();
	~ServerInfo();
	void Free();
	ConnectInfo *GetDefult(Server_type type);
	ConnectInfo *GetConnInfo(Server_type type,char *pName);
	ConnectInfo *GetNextConnInfo(Server_type type,char *pName);
	void AddConn(Server_type type,ConnectInfo *conn,BOOL bdefault);
	BOOL DeleteConn(Server_type type,ConnectInfo *pConn);
	SiteInfo *GetSite(Server_type type);
	void RemoveSite(Server_type type);
};

class BetsRange
{
private:
	CList<RangeItem> m_onlineRatioList;
	CList<RangeItem> m_reqHandleTimeList;
public:
	void SetOnlineRadioRange(RangeItem *pOnline,int count);
	void SetHandleTimeRange(RangeItem *pTime,int count);
	float GetOnLineRatioBets(float fVlaue);
	float GetReqHandleTimeBets(float fVlaue);
};

