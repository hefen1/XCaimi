/*******************************************************
  Դ��������:TradeAccountMng.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �˺Ź������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradeAccountMngIntf.h"
#include "DataSourceDefineEx.h"
//#include "BusinessLogic.h"

class CAccount : public IAccount
{
public:
	CAccount();
    ~CAccount();
	void Release();
	void FreeCache();
	//��ȡ�ʺ�ID
	int GetID();
	//�ͻ������õ�Ӫҵ����
	CString GetCellNo();
	void SetCellNo(const CString Value);
	//��̨Ӫҵ����
	CString GetBranchNo();
	void SetBranchNo(const CString Value);
	//����ת��ID
    int GetServerID();
	void SetServerID(const int Value);
	//���߿ͻ���
	int GetOnlineID();
	void SetOnlineID(const int Value);
	//��������
	int GetOnlineToken();
	void SetOnlineToken(const int Value);
	//���뷵�ص�����IP
	int GetClientAddr();
	void SetClientAddr(const int Value);
	//�˺��Ƿ��¼
	BOOL GetIsLogined();
	void SetIsLogined(const BOOL Value);
	//��¼�˺�����
	CString GetLoginAccountType();
	void SetLoginAccountType(const CString Value); 

	//����ǹɶ��ʺŵ�¼��Ҫ�г�����
	CString GetLoginMarketType();
	void SetLoginMarketType(const CString Value); 

	//��¼�˺�
	CString GetLoginAccount();
	void SetLoginAccount(const CString Value); 
	//��������
	CString GetPassword();
	void SetPassword(const CString Value);
	//�ͻ���
	CString GetClientID();
	void SetClientID(const CString Value);
	//�������ʽ��˺�
	CString GetAccountName();
	void SetAccountName(const CString Value);
	//�ͻ�����
	CString GetUserName();
	void SetUserName(const CString lpValue); 
	//�˺��Ƿ�����
	BOOL GetIsLocked();
	void SetIsLocked(const BOOL Value);
	//����¼��Ϣ
	CString GetLastLoginInfo();
	void SetLastLoginInfo(const CString Value); 
	//��¼ʧ����Ϣ
	CString GetLoginFailCause();
	void SetLoginFailCause(const CString pValue); 
	//ҵ���߼��ӿ�
	IBusinessLogic* GetBusinessLogic();
	ITradeDataCore* GetTradeDataCore();

	CMapStringToString* GetNVSession();
	CMapStringToString* GetArSession();

	BOOL Login(CString &sMsg, BOOL bReConnect = FALSE);

	long GetConnectHandle();
	void SetConnectHandle(const long lValue);
	
	BOOL IsConnected();

	CString GetJrName();
	void SetJrName(const CString Value);

	//ITradeMarketList* GetTradeMarketList();

	ITradeConfigObj* GetTradeConfigObj();
	ICCommParamList* GetAccountParamList();

	void AddTradingAsync(ITrading* pTrading);
	void RemoveTradingAsync(ITrading* pTrading);
	EAccountType GetAccountType();
	void SetAccountType(EAccountType nType);
	BOOL AutoLogin();
	BOOL AutoQuoteLogin();
	
	//�ʺ�ID
	int m_iAccountID;

protected:

	BOOL LoginTrade(CString &sMsg);
	BOOL LoginLevel1(CString &sMsg, BOOL bReConnect = FALSE);
	BOOL LoginLevel2(CString &sMsg, BOOL bReConnect = FALSE);
	BOOL LoginFuture(CString &sMsg,  BOOL bReConnect);
private:
	CString m_CellNo;
	CString m_sBranchNo;
	int m_iServerID;
	int m_iOnlineID;
	int m_iOnlineToken;
	int m_iClientAddr;
	BOOL m_bIsLogined;
	CString m_sLoginAccountType;
	CString m_sLoginMarketType;
	CString m_sLoginAccount;
	CString m_sPassword;
	CString m_sClientID;
	CString m_sAccountName;
	CString m_sUserName;
	BOOL m_bIsLocked;
	CString m_sLastLoginInfo;
	CString m_sLoginFailCause;
	IBusinessLogic* m_BusinessLogic;
	CMapStringToString* m_NVSession;
	CMapStringToString* m_ArSession;
	long m_lConnectHandle;
	BOOL m_bConnected;
	OnConnectNotify m_pfnOnConnectNotify;
	CString m_sJrName;
	CString m_sClientIP;
	ITradeConfigObj* m_pTradeConfigObj;
	CArray<ITrading*, ITrading*> m_TradingAsyncList;
	EAccountType m_nAccountType;
	int m_nAutoLoginNum;
	ITradeDataCore* m_TradeDataCore;

	static IChannelManager*  m_pChannelManager;
	static IChannels*        m_pChannels;
	static IDataSourceEx*	 m_pDataSource;		 // ��������ָ��
	ICCommParamList* m_pParamList;
};

typedef CTypedPtrArray<CPtrArray, IAccount*> CAccountList;
typedef CTypedPtrMap<CMapWordToPtr, WORD, CAccountList*> CAccountTypeMap;
typedef CTypedPtrMap<CMapWordToPtr, WORD, IAccount*> CCurrentAccount;

class CAccountMng : public IAccountMng
{
public:
	CAccountMng();
    ~CAccountMng();
	//�ͷ�
	void Release();
	void FreeCache(EAccountType nType);
	void FreeCacheAll();
	int GetCount(EAccountType nType);
	//����������ȡ�˺�
	IAccount * GetAccount(const int iIndex, EAccountType nType);
	//����ID��ȡ�˺�
	IAccount * GetAccountByID(const int iID, EAccountType nType);
	//�½��ʺ�
	IAccount * NewAccount(EAccountType nType);
	//�����˺�
	void Add(IAccount * lpValue);
	//ɾ���˺�
	void DeleteAccount(IAccount * lpValue);
	//��ȡ��ǰ�˺�
	IAccount * GetCurrentAccount(EAccountType nType);
	//���õ�ǰ�˺�
	void SetCurrentAccount(IAccount* lpValue);
	BOOL OnNetConnectNotify(ConnectNotify* pNotify);
	void RegAccountChange(IAccountChange* lpValue);
	void UnRegAccountChange(IAccountChange* lpValue);
	void CallAccountChange(IAccount* lpValue, EAccountChange nChangeType);
	void CallAccountStatus(IAccount* lpValue);
	int GetCurrentAccountCount();
	IAccount * GetCurrentAccountByIndex(int iIndex);
	IAccount * GetAccountByNameAndCellNo(CString sAccountName, CString sCellNo, EAccountType nType);
protected:

private:
	//CAccountList m_AccountList;
	CAccountTypeMap m_AccountTypeMap;
	CCurrentAccount m_CurrentAccountMap;
	int m_iMaxID;
	//IAccount * m_pCurrentAccount;
	IChannelManager* m_pChannelManager;
	OnConnectNotify m_pfnOnConnectNotify;
	CArray<IAccountChange*, IAccountChange*> m_AccountChangeList;
};
