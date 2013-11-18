#pragma once

#include "..\..\Common\HsJyComm.h"

typedef HJC_ENV_INTERFACE (*NetEngineJYCreateEI)( int, const char *,int, const char *,	const char *, const char *, const char *);
typedef int (*NetEngineJYSetProxy)(HJC_ENV_INTERFACE,int,const char *,int ,const char *,const char *,int );
typedef int (*NetEngineJYConnect)(HJC_ENV_INTERFACE, UINT );
typedef int (*NetEngineJYReceive)(HJC_ENV_INTERFACE,char *,UINT&, UINT);
typedef int (*NetEngineJYSend)(HJC_ENV_INTERFACE, char *,UINT, UINT);
typedef HJC_PACKET_MANAGER (*NetEngineJYCreatePM)(void);
typedef int (*NetEngineJYParse)(HJC_PACKET_MANAGER,const char *,UINT);
typedef int (*NetEngineJYGetHeader)(HJC_PACKET_MANAGER, int, UINT &);
typedef void (*NetEngineJYDestroyEI)(HJC_ENV_INTERFACE);
typedef int (*NetEngineJYCompose)( HJC_PACKET_MANAGER,  char*, UINT&);
typedef int (*NetEngineJYSetHeader)( HJC_PACKET_MANAGER , int, UINT);
typedef int (*NetEngineJYGetProtoUnit)( HJC_PACKET_MANAGER,	 const char *, char* , UINT &);
typedef void (*NetEngineJYDestroyPM)( HJC_PACKET_MANAGER );
typedef int (*NetEngineJYGetMac)(HJC_ENV_INTERFACE, int, char *, int &);

struct NetEngineJYAPI{
public:
	NetEngineJYCreateEI JYCreateEI;
	NetEngineJYSetProxy JYSetProxy;
	NetEngineJYConnect  JYConnect;
	NetEngineJYReceive  JYReceive;
	NetEngineJYSend    JYSend;
	NetEngineJYCreatePM JYCreatePM;
	NetEngineJYParse    JYParse;
	NetEngineJYGetHeader   JYGetHeader;
	NetEngineJYDestroyEI JYDestroyEI;
	NetEngineJYCompose JYCompose;
	NetEngineJYSetHeader JYSetHeader;
	NetEngineJYGetProtoUnit JYGetProtoUnit;
	NetEngineJYDestroyPM JYDestroyPM;
	NetEngineJYGetMac JYGetMac;

	NetEngineJYAPI()
	{
		JYDestroyPM = NULL;
		JYGetProtoUnit = NULL;
		JYSetHeader = NULL;
		JYGetHeader = NULL;
		JYConnect = NULL;
		JYCreateEI = NULL;
		JYCreatePM = NULL;
		JYParse = NULL;
		JYReceive = NULL;
		JYSend = NULL;
		JYSetProxy = NULL;
		JYDestroyEI = NULL;
		JYCompose = NULL;
		JYGetMac = NULL;
	}
	BOOL Loaded()
	{
		return (JYGetHeader && JYConnect && JYCreateEI && JYCreatePM && JYCompose && JYGetProtoUnit
			&& JYParse && JYReceive && JYSend && JYSetProxy && JYDestroyEI && JYSetHeader && JYDestroyPM && JYGetMac);
	}

public:
	char* GetData(HJC_PACKET_MANAGER pm,CString name,UINT &len)
	{
		char* lpDataBuf = NULL;
		if (JYGetProtoUnit != NULL)
		{
			if (JYGetProtoUnit(pm, (LPTSTR)(LPCTSTR)name, lpDataBuf, len) >= 0)
			{
				lpDataBuf = new char[len];
				if (JYGetProtoUnit(pm, (LPTSTR)(LPCTSTR)name, lpDataBuf, len) <= 0)
				{
					delete[] lpDataBuf;
					THROW("解包失败！");
					return NULL;
				}
			}
		}
		return lpDataBuf;
	}
	char* GetPackData(HJC_PACKET_MANAGER pm, UINT &len)
	{
		char* lpPackBuf = NULL;
		int iPackLen = 0;
		if (JYCompose != NULL)
		{			
			if (JYCompose(pm, lpPackBuf, len) >= 0)
			{
				lpPackBuf = new char[len];
				if (JYCompose(pm, lpPackBuf, len) <= 0)
				{
					delete[] lpPackBuf;
					THROW("打包失败！");
					return NULL;
				}
			}
		}
		return lpPackBuf;
	}
};