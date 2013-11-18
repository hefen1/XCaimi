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
	VF_NewPrice = 1,               //���¼�
	VF_Close,                          //����
	VF_Open,                          //���̼�
	VF_NationalDebtRatio        //��ծ����,����ֵ
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
	StockUserInfo               m_stkInfo;          //������Ϣ
	long                             m_lAmount;        //�ɷݹ�Ȩ��
	ShareRealTimeData_Ext  m_realTimeData; //ʵʱ���黺������
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
	CodeInfo		               m_ciStockCode;		// ��Ʊ����l
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
	CodeInfo		                 m_ciStockCode;		// ��Ʊ����
	ShareRealTimeData_Ext   m_shareRealTime;	
};

class CCombCacheSet;
class CCombCache
{
public:
	CCombCache(CCombCacheSet*  pOwner);
	~CCombCache(void);
    //������������ָ��
	void   SetDataSourceEx(IDataSourceEx* pDataSourceEx) { m_pDataSourceEx = pDataSourceEx; }
	//��ʼ����ϻ��棺��ʼ�ɷݹɴ���
	BOOL InitCombCache(ICombInfo* pCombInfo);
	//ȡ��ϳɷݹɣ�ͨ��ayStkCode����
	void   GetAllComponentCode(CArray<StockUserInfo,StockUserInfo> &ayStkCode);
	//����������Ϣ
	BOOL	 OnUpdateData(DataHead* pData);
   //����ʵʱ������Ϣ
	void HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);
	//�������ָ��
	void DoCalculateWork();

	double GetCombSZ();
	double GetCombBuyPKSZ();
	double GetCombSellPKSZ();
protected:
	double CalStockCompentBuyPKSZ(CString strCode,const ShareRealTimeData_Ext &srtData,long nAmount); //����ɷݹ��̿���ֵ
	double CalStockCompentSellPKSZ(CString strCode,const ShareRealTimeData_Ext &srtData,long nAmount);
private:
	CString                  m_strCombName;
	CMapStringToPtr    m_componentCatchMap;   //key: code value: CComponentCache*
	CList<CChangedQuoteData*,CChangedQuoteData*>  m_quoteChangeDataList;
	IDataSourceEx*        m_pDataSourceEx;		    //��������ӿ�ָ��
	CCriticalSection        m_critSection; 
	CCombCacheSet*     m_pOwner;

	int                           m_nAmount;
	double                    m_dCombSZ;//�����ֵ
	double                    m_dCombBuyPKSZ;//����̿���ֵ
	double                    m_dCombSellPKSZ;
};

typedef CTypedPtrArray<CPtrArray,ICombCacheNotifier*>  CNotifierArray;
class CCombCacheSet: public ICombCacheSet,public IDataSourceSink
{
	DECLARE_DYNAMIC( CCombCacheSet )
public:
	CCombCacheSet();
	~CCombCacheSet();

	//IDataSourceSinkʵ��
	virtual	BOOL HSDataSourceSink_OnCommNotify(void* pData);
	virtual	BOOL HSDataSourceSink_OnRecvData(void* pData, int nLen);

	//ICombCacheSetʵ��
	virtual	BOOL InitCacheSet();//��ʼ������Ϣ��Ϣ	
	virtual	void ReflashCombCacheSet();//ˢ�������Ϣ
	virtual	BOOL DoCalculateWork(BOOL bForceCal = FALSE);//�������ָ��
	virtual	void StartCalculateWork(BOOL bStart = TRUE);//����/ֹͣ ���㹤��
	virtual   void RegisterCombCacheNotifier(ICombCacheNotifier *pNotifier);//ע��֪ͨ
	virtual   void UnregisterCombCacheNotifier(ICombCacheNotifier *pNotifier);//ȡ��ע��֪ͨ
public:
	BOOL  GetStockPriceUnitDecimal(CString strCode, short& nPriceUnit, short& nDecimal);
	double GetPrice(CString strCode);//ȡ���¼�
	double GetNetValue(CString strCode);//ȡ��ֵ
	double GetClose(CString strCode);//�����̼�
	double GetOpen(CString strCode);//���̼�
	double GetBPrice(CString strCode,int nFlag);//ȡ���
	double GetSPrice(CString strCode,int nFlag);//ȡ����
	double GetCombPrice(const char* code); //ȡ�����ֵ
	double GetCombBPKSZ(const char* code);//ȡ������̿���ֵ
	double GetCombSPKSZ(const char* code);//ȡ������̿���ֵ
	double GetRealTimeValue(CString strCode,ValueFlag vf);//ȡ����۸�
protected:
	void RequestRealTimeData(const CArray<StockUserInfo> &ayCodeinfo,unsigned short nType,char	cOperator);
	void ConvertRealTimeToRealTimeExt(CodeInfo	 ci,ShareRealTimeData* pCurrentRealTime,ShareRealTimeData_Ext &rtdExt);
	void CalculateCombCache();
	void DoExternalNotify();
	//��ջ���
	void Clear();
	//���󲢶�����������
	BOOL RequestRealTimeData(Server_type svrType = CEV_Connect_ALL);
	//��������(���߳���ִ�в���)
	void OnUpdateData(void* pData, int nLen); 
	void HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);
	void HandleNowDataExt(DataHead* pHead, const CommRealTimeData* pnowData, int nSize);
private:
	CMapStringToOb            m_mapCombCache;  //CombCache Map: key-�������  value-CCombCacheָ��
	CMapStringToOb            m_mapQuoteCache;  //���д�����������ݻ���(ʵʱ����) key-�������� value-
	CMapStringToOb            m_mapStkInfoCache; //���д�����������ݻ���(ʵʱ����) key-�������� value-StockUserInfoָ��
	CNotifierArray                 m_ayNoifiers;
	IDataSourceEx*               m_pDataSourceEx;	 //��������ӿ�ָ��
	long                               m_nDataSourceID;     //��������ID
	CCriticalSection               m_critSection;
	CEvent                            m_event;
	BOOL                             m_bStopCalculate;
	CWinThread*                   m_pCalThread;
};

UINT DoCalculateWorkThreadProc( LPVOID pParam );
BOOL IsStockStop(const ShareRealTimeData_Ext &srtData);
void LogStockRealTimeInfo(CString strCode,const ShareRealTimeData_Ext &srtData);

