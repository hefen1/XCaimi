/*******************************************************
  源程序名称:TradeAccountMngIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  账号管理器接口
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "..\NetEngineInterface.h"
#include "TradeConfigObjIntf.h"
//#include "TradeDataCoreIntf.h"
//#include "BusinessLogicIntf.h"

struct IBusinessLogic;
struct ITrading;
struct ITradeDataCore;

//                  交易       L1        L2      资讯      中心
enum EAccountType {atTrade, atLevel1, atLevel2, atInfo, atCenter,atFuture};

//					  改变	   增加   删除
enum EAccountChange {acChange, acAdd, acDel};

struct IAccount
{
	//释放
	virtual void Release() = 0;
	virtual void FreeCache() = 0;
	//账号ID在客户端的一个标识
	virtual int GetID() = 0;
	//连接接口
	//virtual IAsyncTCPClient * GetCommunication(void) = 0;
	//virtual void SetCommunication(const IAsyncTCPClient * lpValue) = 0; 
	//客户端配置的营业部号
	virtual CString GetCellNo() = 0;
	virtual void SetCellNo(const CString Value) = 0;
	//柜台营业部号
	virtual CString GetBranchNo() = 0;
	virtual void SetBranchNo(const CString Value) = 0;

	//接入转发ID
	virtual int GetServerID() = 0;
	virtual void SetServerID(const int Value) = 0;
	//在线客户号
	virtual int GetOnlineID() = 0;
	virtual void SetOnlineID(const int Value) = 0;
	//在线令牌
	virtual int GetOnlineToken() = 0;
	virtual void SetOnlineToken(const int Value) = 0;
	//接入返回的外网IP
	virtual int GetClientAddr() = 0;
	virtual void SetClientAddr(const int Value) = 0;
	//账号是否登录
	virtual BOOL GetIsLogined() = 0;
	virtual void SetIsLogined(const BOOL Value) = 0;
	//交易登录账号类型
	virtual CString GetLoginAccountType() = 0;
	virtual void SetLoginAccountType(const CString Value) = 0; 

	//如果是股东帐号登录需要市场类型
	virtual CString GetLoginMarketType() = 0;
	virtual void SetLoginMarketType(const CString Value) = 0; 

	//登录账号
	virtual CString GetLoginAccount() = 0;
	virtual void SetLoginAccount(const CString Value) = 0; 
	//交易密码
	virtual CString GetPassword() = 0;
	virtual void SetPassword(const CString Value) = 0;
	//客户号
	virtual CString GetClientID() = 0;
	virtual void SetClientID(const CString Value) = 0;
	//完整的资金账号
	virtual CString GetAccountName() = 0;
	virtual void SetAccountName(const CString Value) = 0;
	//客户姓名
	virtual CString GetUserName() = 0;
	virtual void SetUserName(const CString Value) = 0; 
	//账号是否锁定
	virtual BOOL GetIsLocked() = 0;
	virtual void SetIsLocked(const BOOL Value) = 0;
	//最后登录信息
	virtual CString GetLastLoginInfo() = 0;
	virtual void SetLastLoginInfo(const CString Value) = 0; 
	//登录失败信息
	virtual CString GetLoginFailCause() = 0;
	virtual void SetLoginFailCause(const CString Value) = 0; 
	//业务逻辑接口
	virtual IBusinessLogic * GetBusinessLogic() = 0;
	//交易数据中心
	virtual ITradeDataCore * GetTradeDataCore() = 0;

	virtual CMapStringToString* GetNVSession() = 0;
	virtual CMapStringToString* GetArSession() = 0;
	//登陆
	virtual BOOL Login(CString &sMsg, BOOL bReConnect = FALSE) = 0;
	//获取连接句柄
	virtual long GetConnectHandle() = 0;
	//设置连接句柄
	virtual void SetConnectHandle(const long lValue) = 0;
	//获取连接成功标识
	virtual BOOL IsConnected() = 0;

	virtual CString GetJrName() = 0;
	virtual void SetJrName(const CString Value) = 0;

	//virtual ITradeMarketList* GetTradeMarketList() = 0;

	virtual void AddTradingAsync(ITrading* pTrading) = 0;
	virtual void RemoveTradingAsync(ITrading* pTrading) = 0;

	//账号类型
	virtual EAccountType GetAccountType() = 0;
	//自动登陆
	virtual BOOL AutoLogin() = 0;
	virtual BOOL AutoQuoteLogin() = 0;
	virtual ITradeConfigObj* GetTradeConfigObj() = 0;
	//获取帐号参数列表
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
	//释放
	virtual void Release() = 0;
	virtual void FreeCache(EAccountType nType) = 0;
	virtual void FreeCacheAll() = 0;

	virtual int GetCount(EAccountType nType) = 0;
	//根据索引获取账号
	virtual IAccount * GetAccount(const int iIndex, EAccountType nType) = 0;
	//根据ID获取账号
	virtual IAccount * GetAccountByID(const int iID, EAccountType nType) = 0;
	//新建帐号
	virtual IAccount * NewAccount(EAccountType nType) = 0;
	//增加账号
	virtual void Add(IAccount * lpValue) = 0;	
	//删除账号
	virtual void DeleteAccount(IAccount * lpValue) = 0;
	//获取当前账号
	virtual IAccount * GetCurrentAccount(EAccountType nType) = 0;
	//设置当前账号
	virtual void SetCurrentAccount(IAccount * lpValue) = 0;
	virtual BOOL OnNetConnectNotify(ConnectNotify* pNotify) = 0;
	//注册账号改变事件
	virtual void RegAccountChange(IAccountChange* lpValue) = 0;
	//注销账号改变事件
	virtual void UnRegAccountChange(IAccountChange* lpValue) = 0;
	virtual void CallAccountStatus(IAccount* lpValue) = 0;
	//统计当前帐号数量
	virtual int GetCurrentAccountCount() = 0;
	//根据索引获取当前帐号
	virtual IAccount * GetCurrentAccountByIndex(int iIndex) = 0;
	//根据登陆返回的账号（交易为资金账号）和客户端配置的营业部
	virtual IAccount * GetAccountByNameAndCellNo(CString sAccountName, CString sCellNo, EAccountType nType) = 0;
};
