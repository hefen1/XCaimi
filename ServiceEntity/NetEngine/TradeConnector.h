#pragma once
#include "connectorimp.h"
#include "..\..\Common\HsJyComm.h"

#define INVALID_HANDLE			0		//句柄创建失败

class CTradeConnector: public CConnector
{
public:
	CTradeConnector(OnConnectNotify noti=NULL);
	virtual ~CTradeConnector(void);

public:
	virtual int SendRcv(const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen);
	virtual UINT AsyncSend(const char* szBuf,UINT nBufLen,DWORD TimeOut = -1);
	virtual BOOL Connect(void *ptr);
	virtual void CloseConn();//关闭连接及线程
	virtual CString GetMacAdd();
	virtual bool IsOpen();	   

protected:
	virtual UINT GetPacketID(const char* szBuf,UINT nBufLen);
	virtual void DoReceive(const char* szBuf,UINT nBufLen);
	virtual void OnEvent(UINT uEvent);
	// Run function - override to implement a new behaviour
	virtual void RunReceive();
	virtual void RunSend();
	virtual void RunCallBack();

	virtual UINT SyncSend(const char* ReqBuf,UINT nReqLen);
	virtual int  Receive(char*& RcvBuf,UINT& nRcvLen,DWORD nTimeOut,UINT packID);

	
private:
	HJC_ENV_INTERFACE m_conn;
	CString m_sUserName;
	CString m_sPWD;
	TradeCert *m_pTradeCert;
};