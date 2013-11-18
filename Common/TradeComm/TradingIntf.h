/*******************************************************
  Դ��������:TradingIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����������ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradeDataBaseIntf.h"
#include "TradeAccountMngIntf.h"

#define TRADE_TIMEOUT  30
//typedef void (TradingCallBack)(ITrading *);
struct ITrading;


struct ITradingAsyncCallBack
{
	virtual void TradingAsyncCallBack(ITrading* pTrading, int iAction) = 0;
};


struct ITrading
{
	//�ͷ�
	virtual void Release() = 0;
	virtual void SetAction(const int iAction) = 0;
	virtual void SetAccount(IAccount* pAccount) = 0;
	//�Ƿ�ɹ�
	virtual BOOL GetSuccessful() = 0;
	//�������
	virtual void Clear() = 0;
	//NV�ֶ����ݶ�ȡ������
	virtual CString GetRequestNVData(const CString sName) = 0;
	virtual void SetRequestNVData(const CString sName, const CString sValue) = 0;
	//���������ݸ������ƶ�ȡ������
	virtual char* GetBinData(const CString sName, int& iDataLen) = 0;
	virtual void SetBinData(const CString scName, char* lpData, int iDataLen) = 0;
	//AR�ֶ����ݵĶ�ȡ������
	virtual CString GetRequestARData(const CString sName) = 0;
	virtual void SetRequestARData(const CString sName, const CString sValue) = 0;
	//ͬ������
	virtual BOOL TradingSync(const int iTimeOut = TRADE_TIMEOUT) = 0;
	//�첽����
	virtual	BOOL TradingAsync(ITradingAsyncCallBack * lpTradingAsyncCallBack, const int iTimeOut = -1) = 0;	
	//��ȡί�н��
	virtual IDataList * GetDataList() = 0;

	virtual void SetConnectHandle(long nConnectHandle) = 0;
	virtual CString GetLastError() = 0;
	virtual BOOL HaveError(CString &sMsg) = 0;
	virtual BOOL HaveData() = 0;
	virtual void FreeDataList() = 0;

	virtual void CallBack() = 0;
};