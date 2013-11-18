/*******************************************************
  源程序名称:TradingIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易请求类接口定义
  作    者:  shenglq
  开发日期:  20100720
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
	//释放
	virtual void Release() = 0;
	virtual void SetAction(const int iAction) = 0;
	virtual void SetAccount(IAccount* pAccount) = 0;
	//是否成功
	virtual BOOL GetSuccessful() = 0;
	//清空数据
	virtual void Clear() = 0;
	//NV字段数据读取、设置
	virtual CString GetRequestNVData(const CString sName) = 0;
	virtual void SetRequestNVData(const CString sName, const CString sValue) = 0;
	//二进制数据根据名称读取、设置
	virtual char* GetBinData(const CString sName, int& iDataLen) = 0;
	virtual void SetBinData(const CString scName, char* lpData, int iDataLen) = 0;
	//AR字段数据的读取，设置
	virtual CString GetRequestARData(const CString sName) = 0;
	virtual void SetRequestARData(const CString sName, const CString sValue) = 0;
	//同步请求
	virtual BOOL TradingSync(const int iTimeOut = TRADE_TIMEOUT) = 0;
	//异步请求
	virtual	BOOL TradingAsync(ITradingAsyncCallBack * lpTradingAsyncCallBack, const int iTimeOut = -1) = 0;	
	//获取委托结果
	virtual IDataList * GetDataList() = 0;

	virtual void SetConnectHandle(long nConnectHandle) = 0;
	virtual CString GetLastError() = 0;
	virtual BOOL HaveError(CString &sMsg) = 0;
	virtual BOOL HaveData() = 0;
	virtual void FreeDataList() = 0;

	virtual void CallBack() = 0;
};