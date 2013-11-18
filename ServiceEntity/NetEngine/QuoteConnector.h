#pragma once
#include "connectorimp.h"

class CQuoteConnector : public CConnector
{
public:
	CQuoteConnector(OnConnectNotify noti=NULL);
	virtual ~CQuoteConnector(void);

public:
	virtual int SendRcv(const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen);
	virtual UINT AsyncSend(const char* szBuf,UINT nBufLen,DWORD TimeOut = -1);
	
protected:
	virtual UINT GetPacketID(const char* szBuf,UINT nBufLen);
	virtual void DoReceive(const char* szBuf,UINT nBufLen);
	virtual void DoDivision(const char* szBuf,UINT nBufLen);
	// Event function 
	virtual void OnEvent(UINT uEvent);
	// Run function - Thread
	virtual void RunReceive();
	virtual void RunSend();
	virtual void RunCallBack();
	// Data function
	virtual UINT ReadData(char *pBuffer, UINT dwSize, DWORD dwTimeout);
	virtual UINT SendData(const char *pBuffer, UINT dwCount, DWORD dwTimeout);
	virtual void Init();
	
	virtual UINT SyncSend(const char* ReqBuf,UINT nReqLen);
	virtual int  Receive(char*& RcvBuf,UINT& nRcvLen,DWORD nTimeOut,UINT packID);

private:
	CDataBuffer  m_PacketHead;             //存储包头和包长度
	CDataBuffer  m_ReceivedData;			//临时存储一个包
};
