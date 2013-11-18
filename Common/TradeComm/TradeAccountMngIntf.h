/*******************************************************
  Դ��������:TradeAccountMngIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �˺Ź������ӿ�
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "..\NetEngineInterface.h"
#include "TradeConfigObjIntf.h"
//#include "TradeDataCoreIntf.h"
//#include "BusinessLogicIntf.h"

struct IBusinessLogic;
struct ITrading;
struct ITradeDataCore;

//                  ����       L1        L2      ��Ѷ      ����
enum EAccountType {atTrade, atLevel1, atLevel2, atInfo, atCenter,atFuture};

//					  �ı�	   ����   ɾ��
enum EAccountChange {acChange, acAdd, acDel};

struct IAccount
{
	//�ͷ�
	virtual void Release() = 0;
	virtual void FreeCache() = 0;
	//�˺�ID�ڿͻ��˵�һ����ʶ
	virtual int GetID() = 0;
	//���ӽӿ�
	//virtual IAsyncTCPClient * GetCommunication(void) = 0;
	//virtual void SetCommunication(const IAsyncTCPClient * lpValue) = 0; 
	//�ͻ������õ�Ӫҵ����
	virtual CString GetCellNo() = 0;
	virtual void SetCellNo(const CString Value) = 0;
	//��̨Ӫҵ����
	virtual CString GetBranchNo() = 0;
	virtual void SetBranchNo(const CString Value) = 0;

	//����ת��ID
	virtual int GetServerID() = 0;
	virtual void SetServerID(const int Value) = 0;
	//���߿ͻ���
	virtual int GetOnlineID() = 0;
	virtual void SetOnlineID(const int Value) = 0;
	//��������
	virtual int GetOnlineToken() = 0;
	virtual void SetOnlineToken(const int Value) = 0;
	//���뷵�ص�����IP
	virtual int GetClientAddr() = 0;
	virtual void SetClientAddr(const int Value) = 0;
	//�˺��Ƿ��¼
	virtual BOOL GetIsLogined() = 0;
	virtual void SetIsLogined(const BOOL Value) = 0;
	//���׵�¼�˺�����
	virtual CString GetLoginAccountType() = 0;
	virtual void SetLoginAccountType(const CString Value) = 0; 

	//����ǹɶ��ʺŵ�¼��Ҫ�г�����
	virtual CString GetLoginMarketType() = 0;
	virtual void SetLoginMarketType(const CString Value) = 0; 

	//��¼�˺�
	virtual CString GetLoginAccount() = 0;
	virtual void SetLoginAccount(const CString Value) = 0; 
	//��������
	virtual CString GetPassword() = 0;
	virtual void SetPassword(const CString Value) = 0;
	//�ͻ���
	virtual CString GetClientID() = 0;
	virtual void SetClientID(const CString Value) = 0;
	//�������ʽ��˺�
	virtual CString GetAccountName() = 0;
	virtual void SetAccountName(const CString Value) = 0;
	//�ͻ�����
	virtual CString GetUserName() = 0;
	virtual void SetUserName(const CString Value) = 0; 
	//�˺��Ƿ�����
	virtual BOOL GetIsLocked() = 0;
	virtual void SetIsLocked(const BOOL Value) = 0;
	//����¼��Ϣ
	virtual CString GetLastLoginInfo() = 0;
	virtual void SetLastLoginInfo(const CString Value) = 0; 
	//��¼ʧ����Ϣ
	virtual CString GetLoginFailCause() = 0;
	virtual void SetLoginFailCause(const CString Value) = 0; 
	//ҵ���߼��ӿ�
	virtual IBusinessLogic * GetBusinessLogic() = 0;
	//������������
	virtual ITradeDataCore * GetTradeDataCore() = 0;

	virtual CMapStringToString* GetNVSession() = 0;
	virtual CMapStringToString* GetArSession() = 0;
	//��½
	virtual BOOL Login(CString &sMsg, BOOL bReConnect = FALSE) = 0;
	//��ȡ���Ӿ��
	virtual long GetConnectHandle() = 0;
	//�������Ӿ��
	virtual void SetConnectHandle(const long lValue) = 0;
	//��ȡ���ӳɹ���ʶ
	virtual BOOL IsConnected() = 0;

	virtual CString GetJrName() = 0;
	virtual void SetJrName(const CString Value) = 0;

	//virtual ITradeMarketList* GetTradeMarketList() = 0;

	virtual void AddTradingAsync(ITrading* pTrading) = 0;
	virtual void RemoveTradingAsync(ITrading* pTrading) = 0;

	//�˺�����
	virtual EAccountType GetAccountType() = 0;
	//�Զ���½
	virtual BOOL AutoLogin() = 0;
	virtual BOOL AutoQuoteLogin() = 0;
	virtual ITradeConfigObj* GetTradeConfigObj() = 0;
	//��ȡ�ʺŲ����б�
	virtual ICCommParamList* GetAccountParamList() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IAccountChange
{
	virtual void OnAccountChange(IAccount* lpValue, EAccountChange nChageType) = 0;
	virtual void OnAccountStatus(IAccount* lpValue) = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IAccountMng
{
	//�ͷ�
	virtual void Release() = 0;
	virtual void FreeCache(EAccountType nType) = 0;
	virtual void FreeCacheAll() = 0;

	virtual int GetCount(EAccountType nType) = 0;
	//����������ȡ�˺�
	virtual IAccount * GetAccount(const int iIndex, EAccountType nType) = 0;
	//����ID��ȡ�˺�
	virtual IAccount * GetAccountByID(const int iID, EAccountType nType) = 0;
	//�½��ʺ�
	virtual IAccount * NewAccount(EAccountType nType) = 0;
	//�����˺�
	virtual void Add(IAccount * lpValue) = 0;	
	//ɾ���˺�
	virtual void DeleteAccount(IAccount * lpValue) = 0;
	//��ȡ��ǰ�˺�
	virtual IAccount * GetCurrentAccount(EAccountType nType) = 0;
	//���õ�ǰ�˺�
	virtual void SetCurrentAccount(IAccount * lpValue) = 0;
	virtual BOOL OnNetConnectNotify(ConnectNotify* pNotify) = 0;
	//ע���˺Ÿı��¼�
	virtual void RegAccountChange(IAccountChange* lpValue) = 0;
	//ע���˺Ÿı��¼�
	virtual void UnRegAccountChange(IAccountChange* lpValue) = 0;
	virtual void CallAccountStatus(IAccount* lpValue) = 0;
	//ͳ�Ƶ�ǰ�ʺ�����
	virtual int GetCurrentAccountCount() = 0;
	//����������ȡ��ǰ�ʺ�
	virtual IAccount * GetCurrentAccountByIndex(int iIndex) = 0;
	//���ݵ�½���ص��˺ţ�����Ϊ�ʽ��˺ţ��Ϳͻ������õ�Ӫҵ��
	virtual IAccount * GetAccountByNameAndCellNo(CString sAccountName, CString sCellNo, EAccountType nType) = 0;
};
