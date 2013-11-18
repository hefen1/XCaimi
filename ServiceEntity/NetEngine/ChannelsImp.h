#pragma once
#include "NetEngineStructDefine.h"

class CChannelsImp :public IChannels
{
public:
	CChannelsImp(void);
	~CChannelsImp(void);
	/********************************************************************************
	* 函数功能 :同步发送接收数据包	
	* 函数参数 :long handle:连接句柄
	*           const char* ReqBuf: 发送数据包指针
	*           int nReqLen:  发送的数据包长度
	*           long nTimeOut: 超时时间
	*           const char* RcvBuf: 接收数据包指针（用于保存数据）
	*           int nRcvLen:  接收的数据包长度
	*  返回值  :成功返回0，反之为-1	
	*******************************************************************************/ 
	virtual int SendRcv(long handle,const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen);
	virtual int FreeMemory(long handle,char *pMem);
	/********************************************************************************
	* 函数功能 :异步发送数据包,应答通过RegisterProcessor注册的处理函数自行处理	
	* 函数参数 :const char* szBuf: 发送数据包指针
	*           int nBufLen:  发送的数据包长度
	*			dword timeout: 超时时间 -1不超时
	*  返回值  :成功返回0，反之为-1	
	*******************************************************************************/
	virtual int AsyncSend(long handle,const char* szBuf,UINT nBufLen,DWORD TimeOut = -1);


	/********************************************************************************
	* 函数功能 :注册异步应答包处理函数
	* 函数参数 :long handle:连接句柄
	*           OnPacketReceive processor: 处理函数指针
	*  返回值  :成功返回0，反之为FALSE	
	*******************************************************************************/
	virtual int RegisterProcessor(long handle,OnPacketReceive processor);
	virtual int UnRegisterProcessor(long handle,OnPacketReceive processor);
	/********************************************************************************
	* 函数功能 :取最近的错误信息
	* 函数参数 :long handle:连接句柄
	*  返回值  :通号handleh上的错误信息
	*******************************************************************************/
	virtual CString GetLastEorror(long handle);
	virtual UINT GetPacketID(long handle);
	
	virtual Server_type GetServerTypeByHandle(long handle);
};
