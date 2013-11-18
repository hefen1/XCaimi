#pragma once

#include <afxmt.h>
#include "hsds_comudata.h"
#include "hsstructnew.h"
#include "CombComm/CombManagerInterface.h"
#include "CombComm/CombCache.h"
#include "DataSourceDefineEx.h"

#define Query_Quote_Data_Time_Out     10 * 1000
#define Default_Price_Unit         1000
#define Default_Decimal            2
#define Default_Result_Value     0

enum ValueFlag
{
	VF_NewPrice = 1,               //最新价
	VF_Close,                          //昨收
	VF_Open,                          //开盘价
	VF_NationalDebtRatio        //国债利率,基金净值
};

class CCombCache;
class CComponentCache
{
	friend class CCombCache; 
public:
	CComponentCache()
	{
		m_stkInfo.SetEmpty();
		m_lAmount = 0;
		m_dBuyPKSZ = 0;
		m_dSellPKSZ = 0;
		memset(&m_realTimeData,0,sizeof(m_realTimeData));
	}
	~CComponentCache()
	{

	}
private:
	StockUserInfo               m_stkInfo;          //代码信息
	long                             m_lAmount;        //成份股权重
	ShareRealTimeData_Ext  m_realTimeData; //实时行情缓存数据
	double                         m_dBuyPKSZ;
	double                         m_dSellPKSZ;
};

struct ShareRealTimeChanged
{
	ShareRealTimeChanged()
	{
		memset(&m_ciStockCode,0,sizeof(CodeInfo));
		m_lAmount = 0;
		m_dPreBuyPKSZ = 0;
		m_dlastBuyPKSZ = 0;
		m_dPreSellPKSZ = 0;
		m_dlastSellPKSZ = 0;
		memset(&m_preData,0,sizeof(m_preData));
		memset(&m_lastData,0,sizeof(m_lastData));
	}
	CodeInfo		               m_ciStockCode;		// 股票代码l
	long                            m_lAmount;
	ShareRealTimeData_Ext m_preData;
	ShareRealTimeData_Ext m_lastData;

	double                        m_dPreBuyPKSZ;
	double                        m_dlastBuyPKSZ;

	double                        m_dPreSellPKSZ;
	double                        m_dlastSellPKSZ;	
};

class CChangedQuoteData
{
public:
	friend class CCombCache; 
	CChangedQuoteData();
	~CChangedQuoteData();
	void AddChangedData(ShareRealTimeChanged *pChangedData);
private:
	CPtrArray       m_changedSTRData;
	CPtrArray       m_preSTRData;
};

struct CodeShareRealTime
{
	CodeShareRealTime()
	{
		memset(&m_ciStockCode,0,sizeof(m_ciStockCode));
		memset(&m_shareRealTime,0,sizeof(m_shareRealTime));
	}
	CodeInfo		                 m_ciStockCode;		// 股票代码
	ShareRealTimeData_Ext   m_shareRealTime;	
};

class CCombCacheSet;
class CCombCache
{
public:
	CCombCache(CCombCacheSet*  pOwner);
	~CCombCache(void);
    //设置数据引擎指针
	void   SetDataSourceEx(IDataSourceEx* pDataSourceEx) { m_pDataSourceEx = pDataSourceEx; }
	//初始化组合缓存：初始成份股代码
	BOOL InitCombCache(ICombInfo* pCombInfo);
	//取组合成份股，通过ayStkCode返回
	void   GetAllComponentCode(CArray<StockUserInfo,StockUserInfo> &ayStkCode);
	//更新行情信息
	BOOL	 OnUpdateData(DataHead* pData);
   //处理实时行情信息
	void HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);
	//计算组合指标
	void DoCalculateWork();

	double GetCombSZ();
	double GetCombBuyPKSZ();
	double GetCombSellPKSZ();
protected:
	double CalStockCompentBuyPKSZ(CString strCode,const ShareRealTimeData_Ext &srtData,long nAmount); //计算成份股盘口市值
	double CalStockCompentSellPKSZ(CString strCode,const ShareRealTimeData_Ext &srtData,long nAmount);
private:
	CString                  m_strCombName;
	CMapStringToPtr    m_componentCatchMap;   //key: code value: CComponentCache*
	CList<CChangedQuoteData*,CChangedQuoteData*>  m_quoteChangeDataList;
	IDataSourceEx*        m_pDataSourceEx;		    //数据引擎接口指针
	CCriticalSection        m_critSection; 
	CCombCacheSet*     m_pOwner;

	int                           m_nAmount;
	double                    m_dCombSZ;//组合市值
	double                    m_dCombBuyPKSZ;//组合盘口市值
	double                    m_dCombSellPKSZ;
};

typedef CTypedPtrArray<CPtrArray,ICombCacheNotifier*>  CNotifierArray;
class CCombCacheSet: public ICombCacheSet,public IDataSourceSink
{
	DECLARE_DYNAMIC( CCombCacheSet )
public:
	CCombCacheSet();
	~CCombCacheSet();

	//IDataSourceSink实现
	virtual	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	virtual	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);

	//ICombCacheSet实现
	virtual	BOOL InitCacheSet();//初始缓存信息信息	
	virtual	void ReflashCombCacheSet();//刷新组合信息
	virtual	BOOL DoCalculateWork(BOOL bForceCal = FALSE);//计算相关指标
	virtual	void StartCalculateWork(BOOL bStart = TRUE);//开启/停止 计算工作
	virtual   void RegisterCombCacheNotifier(ICombCacheNotifier *pNotifier);//注册通知
	virtual   void UnregisterCombCacheNotifier(ICombCacheNotifier *pNotifier);//取消注册通知
public:
	BOOL  GetStockPriceUnitDecimal(CString strCode, short& nPriceUnit, short& nDecimal);
	double GetPrice(CString strCode);//取最新价
	double GetNetValue(CString strCode);//取净值
	double GetClose(CString strCode);//最收盘价
	double GetOpen(CString strCode);//开盘价
	double GetBPrice(CString strCode,int nFlag);//取买价
	double GetSPrice(CString strCode,int nFlag);//取卖价
	double GetCombPrice(const char* code); //取组合市值
	double GetCombBPKSZ(const char* code);//取组合买盘口市值
	double GetCombSPKSZ(const char* code);//取组合买盘口市值
	double GetRealTimeValue(CString strCode,ValueFlag vf);//取行情价格
protected:
	void RequestRealTimeData(const CArray<StockUserInfo> &ayCodeinfo,unsigned short nType,char	cOperator);
	void ConvertRealTimeToRealTimeExt(CodeInfo	 ci,ShareRealTimeData* pCurrentRealTime,ShareRealTimeData_Ext &rtdExt);
	void CalculateCombCache();
	void DoExternalNotify();
	//清空缓存
	void Clear();
	//请求并订阅行情数据
	BOOL RequestRealTimeData(Server_type svrType = CEV_Connect_ALL);
	//更新行情(主线程内执行操作)
	void OnUpdateData(void* pData, int nLen); 
	void HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);
	void HandleNowDataExt(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);
private:
	CMapStringToOb            m_mapCombCache;  //CombCache Map: key-组合名称  value-CCombCache指针
	CMapStringToOb            m_mapQuoteCache;  //所有代码的行情数据缓存(实时行情) key-代码名称 value-
	CMapStringToOb            m_mapStkInfoCache; //所有代码的行情数据缓存(实时行情) key-代码名称 value-StockUserInfo指针
	CNotifierArray                 m_ayNoifiers;
	IDataSourceEx*               m_pDataSourceEx;	 //数据引擎接口指针
	long                               m_nDataSourceID;     //数据引擎ID
	CCriticalSection               m_critSection;
	CEvent                            m_event;
	BOOL                             m_bStopCalculate;
	CWinThread*                   m_pCalThread;
};

UINT DoCalculateWorkThreadProc( LPVOID pParam );
BOOL IsStockStop(const ShareRealTimeData_Ext &srtData);
void LogStockRealTimeInfo(CString strCode,const ShareRealTimeData_Ext &srtData);

