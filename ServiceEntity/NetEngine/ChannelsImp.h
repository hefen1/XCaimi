#pragma once
#include "NetEngineStructDefine.h"

class CChannelsImp :public IChannels
{
public:
	CChannelsImp(void);
	~CChannelsImp(void);
	/********************************************************************************
	* �������� :ͬ�����ͽ������ݰ�	
	* �������� :long handle:���Ӿ��
	*           const char* ReqBuf: �������ݰ�ָ��
	*           int nReqLen:  ���͵����ݰ�����
	*           long nTimeOut: ��ʱʱ��
	*           const char* RcvBuf: �������ݰ�ָ�루���ڱ������ݣ�
	*           int nRcvLen:  ���յ����ݰ�����
	*  ����ֵ  :�ɹ�����0����֮Ϊ-1	
	*******************************************************************************/ 
	virtual int SendRcv(long handle,const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen);
	virtual int FreeMemory(long handle,char *pMem);
	/********************************************************************************
	* �������� :�첽�������ݰ�,Ӧ��ͨ��RegisterProcessorע��Ĵ��������д���	
	* �������� :const char* szBuf: �������ݰ�ָ��
	*           int nBufLen:  ���͵����ݰ�����
	*			dword timeout: ��ʱʱ�� -1����ʱ
	*  ����ֵ  :�ɹ�����0����֮Ϊ-1	
	*******************************************************************************/
	virtual int AsyncSend(long handle,const char* szBuf,UINT nBufLen,DWORD TimeOut = -1);


	/********************************************************************************
	* �������� :ע���첽Ӧ���������
	* �������� :long handle:���Ӿ��
	*           OnPacketReceive processor: ������ָ��
	*  ����ֵ  :�ɹ�����0����֮ΪFALSE	
	*******************************************************************************/
	virtual int RegisterProcessor(long handle,OnPacketReceive processor);
	virtual int UnRegisterProcessor(long handle,OnPacketReceive processor);
	/********************************************************************************
	* �������� :ȡ����Ĵ�����Ϣ
	* �������� :long handle:���Ӿ��
	*  ����ֵ  :ͨ��handleh�ϵĴ�����Ϣ
	*******************************************************************************/
	virtual CString GetLastEorror(long handle);
	virtual UINT GetPacketID(long handle);
	
	virtual Server_type GetServerTypeByHandle(long handle);
};
