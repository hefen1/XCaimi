/*******************************************************
  源程序名称:Trading.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易请求类接口定义
  作    者:  shenglq
  开发日期:  20100720
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
	//释放
	void Release();
	void SetAction(const int iAction);
	void SetAccount(IAccount* pAccount);
	//是否成功
	BOOL GetSuccessful();
	//清空数据
	void Clear();
	//NV字段数据读取、设置
	CString GetRequestNVData(const CString sName);
	void SetRequestNVData(const CString sName, const CString sValue);
	//二进制数据根据名称读取、设置
	char* GetBinData(const CString sName, int& iDataLen);
	void SetBinData(const CString scName, char* lpData, int iDataLen);
	CString GetBinStr(const CString sName);
	//AR字段数据的读取，设置
	CString GetRequestARData(const CString sName);
	void SetRequestARData(const CString sName, const CString sValue);
	//同步请求
	BOOL TradingSync(const int iTimeOut = TRADE_TIMEOUT);
	//异步请求
	BOOL TradingAsync(ITradingAsyncCallBack * lpTradingAsyncCallBack, const int iTimeOut = -1);	
	//获取委托结果
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

		CTrading * pThis=(CTrading*)dwUser;//由this指针获得实例的指针

		pThis->OnMMTimer();//调用要回调的成员方法

	}

	virtual IDataList* NewDataList();
	IDataList* m_DataList;
private:
	//打包并返回包信息
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