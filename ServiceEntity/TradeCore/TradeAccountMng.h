/*******************************************************
  源程序名称:TradeAccountMng.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  账号管理器类定义
  作    者:  shenglq
  开发日期:  20100720
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
	//获取帐号ID
	int GetID();
	//客户端配置的营业部号
	CString GetCellNo();
	void SetCellNo(const CString Value);
	//柜台营业部号
	CString GetBranchNo();
	void SetBranchNo(const CString Value);
	//接入转发ID
    int GetServerID();
	void SetServerID(const int Value);
	//在线客户号
	int GetOnlineID();
	void SetOnlineID(const int Value);
	//在线令牌
	int GetOnlineToken();
	void SetOnlineToken(const int Value);
	//接入返回的外网IP
	int GetClientAddr();
	void SetClientAddr(const int Value);
	//账号是否登录
	BOOL GetIsLogined();
	void SetIsLogined(const BOOL Value);
	//登录账号类型
	CString GetLoginAccountType();
	void SetLoginAccountType(const CString Value); 

	//如果是股东帐号登录需要市场类型
	CString GetLoginMarketType();
	void SetLoginMarketType(const CString Value); 

	//登录账号
	CString GetLoginAccount();
	void SetLoginAccount(const CString Value); 
	//交易密码
	CString GetPassword();
	void SetPassword(const CString Value);
	//客户号
	CString GetClientID();
	void SetClientID(const CString Value);
	//完整的资金账号
	CString GetAccountName();
	void SetAccountName(const CString Value);
	//客户姓名
	CString GetUserName();
	void SetUserName(const CString lpValue); 
	//账号是否锁定
	BOOL GetIsLocked();
	void SetIsLocked(const BOOL Value);
	//最后登录信息
	CString GetLastLoginInfo();
	void SetLastLoginInfo(const CString Value); 
	//登录失败信息
	CString GetLoginFailCause();
	void SetLoginFailCause(const CString pValue); 
	//业务逻辑接口
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
	
	//帐号ID
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
	static IDataSourceEx*	 m_pDataSource;		 // 数据引擎指针
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
	//释放
	void Release();
	void FreeCache(EAccountType nType);
	void FreeCacheAll();
	int GetCount(EAccountType nType);
	//根据索引获取账号
	IAccount * GetAccount(const int iIndex, EAccountType nType);
	//根据ID获取账号
	IAccount * GetAccountByID(const int iID, EAccountType nType);
	//新建帐号
	IAccount * NewAccount(EAccountType nType);
	//增加账号
	void Add(IAccount * lpValue);
	//删除账号
	void DeleteAccount(IAccount * lpValue);
	//获取当前账号
	IAccount * GetCurrentAccount(EAccountType nType);
	//设置当前账号
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
