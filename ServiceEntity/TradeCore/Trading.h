/*******************************************************
  Դ��������:Trading.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����������ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradeDataBase.h"
#include "TradingIntf.h"
#include "PackEntry.h"
#include "Ar1Packet.h"
#include "NetEngineInterface.h"
#include "DataSourceDefine.h"
#include "DataSourceSinkDefine.h"

class CTrading : virtual public ITrading, public IDataSourceSinkEx
{
public:
	CTrading();
	virtual ~CTrading();
	//�ͷ�
	void Release();
	void SetAction(const int iAction);
	void SetAccount(IAccount* pAccount);
	//�Ƿ�ɹ�
	BOOL GetSuccessful();
	//�������
	void Clear();
	//NV�ֶ����ݶ�ȡ������
	CString GetRequestNVData(const CString sName);
	void SetRequestNVData(const CString sName, const CString sValue);
	//���������ݸ������ƶ�ȡ������
	char* GetBinData(const CString sName, int& iDataLen);
	void SetBinData(const CString scName, char* lpData, int iDataLen);
	CString GetBinStr(const CString sName);
	//AR�ֶ����ݵĶ�ȡ������
	CString GetRequestARData(const CString sName);
	void SetRequestARData(const CString sName, const CString sValue);
	//ͬ������
	BOOL TradingSync(const int iTimeOut = TRADE_TIMEOUT);
	//�첽����
	BOOL TradingAsync(ITradingAsyncCallBack * lpTradingAsyncCallBack, const int iTimeOut = -1);	
	//��ȡί�н��
	IDataList * GetDataList();
	void SetConnectHandle(long nConnectHandle);
	CString GetLastError();
	BOOL HaveError(CString &sMsg);
	BOOL HaveData();
	int GetHeader(EWholePacketHeade eValue);
	void FreeDataList();
	void CallBack();
	//IDataSourceSinkEx begin
public:
	virtual BOOL HSDataSourceSink_OnCommNotify(void* pData);
	virtual BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);
	virtual BOOL HSDataSourceSinkEx_RecvDataNotify();
	//IDataSourceSinkEx end

protected:
	virtual void OnMMTimer();//
	static void CALLBACK TradingTimerCallBack(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)

	{

		CTrading * pThis=(CTrading*)dwUser;//��thisָ����ʵ����ָ��

		pThis->OnMMTimer();//����Ҫ�ص��ĳ�Ա����

	}

	virtual IDataList* NewDataList();
	IDataList* m_DataList;
private:
	//��������ذ���Ϣ
	char * TradePack(int& iPackLen);
	BOOL ParseData(char* lpBuf, const UINT iBufLen);
	int m_iAction;
	IAccount* m_Account;
	CMapStringToString* m_NVList;
	CMapStringToString* m_ArDataList;
	CPacker* m_Packer;
	CAr1Packet* m_Ar1Packet;
	IChannels* m_pChannels;
	long m_lConnectHandle;
	CString m_sLastError;
	IDataSource* m_DataSource;
	long m_DataSourceHandle;
	ITradingAsyncCallBack* m_TradingAsyncCallBack;
	UINT m_nPackID;
	BOOL m_Successful;
	UINT m_timerId;
	BOOL m_bAsyncCallBackFlag;
};