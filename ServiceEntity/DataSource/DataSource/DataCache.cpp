
#include "StdAfx.h"
#include "FormulaManPublic.h"
#include "DataCache.h"
#include "DataSourceImpl.h"
#include "InfoData.h"
#include "GeneralHelper.h"
#include "hsds_comudata.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

extern void QuickSortAscend2(struct KQuickSort* pQSData);
void AnalyzeUnionString(const CString strSource,       // 源字符串
						CString& strInfoPath,	 // 配置路径
						CString& strRefSection,   // 配置段名
						CString& strRefEntry,     // 配置值名
						CString& strRefFileName   // 配置文件名
						)
{
	CStringArray ayRet;
	int nCount = CGeneralHelper::AnalyseStr(strSource,";",ayRet);
	if( nCount >= 1 )
		strInfoPath    = ayRet.GetAt(0);
	if( nCount >= 2 )
		strRefSection  = ayRet.GetAt(1);
	if( nCount >= 3 )
		strRefEntry    = ayRet.GetAt(2);
	if( nCount >= 4 )
		strRefFileName = ayRet.GetAt(3);
	else
		strRefFileName = INFO_PATH_INI_FILE;

	strInfoPath.Trim();
	strRefSection.Trim();
	strRefEntry.Trim();
	strRefFileName.Trim();
}

CString GetNameByMarketMain(int nMarket)
{
	CString strText;
	const short nMainMarket = MakeMarket(nMarket);
	CString strSection = "特色资讯";
	//YANGDL ADD 2007-01-30 F10路径
	if (strSection.Compare("巨潮资讯") == 0)
	{
		if(nMainMarket == HK_MARKET)
		{
			strText = "港股";
		}
		else if(nMainMarket == STOCK_MARKET)
		{
			if(MakeMainMarket(nMarket) == SH_Bourse)
			{
				if(MakeSubMarket(nMarket) == KIND_BOND)
					strText = "上证债券";
				else if(MakeSubMarket(nMarket) == KIND_QuanZhen)
					strText = "上证权证";
				else if(MakeSubMarket(nMarket) == KIND_FUND)
					strText = "上证基金";
				else
					strText = "上证";

			}
			else if(MakeMainMarket(nMarket) == SZ_Bourse)
			{
				if(MakeSubMarket(nMarket) == KIND_BOND)
					strText = "深证债券";
				else if(MakeSubMarket(nMarket) == KIND_QuanZhen)
					strText = "深证权证";
				else if(MakeSubMarket(nMarket) == KIND_FUND)
					strText = "上证基金";
				else
					strText = "深证";
			}
			else if (MakeMainMarket(nMarket) == (STOCK_MARKET|SH_BOURSE|SZ_BOURSE))		//add by lxqi 20090721	添加板块路径
			{
				strText = "板块";
			}
			else
			{
				strText = "股票";
			}
		}
		else if(nMainMarket == FUTURES_MARKET)
		{
			strText =  "期货";
		}
		else if(nMainMarket == FOREIGN_MARKET)
		{
			strText = "外汇";
		}
		else if(nMainMarket == WP_MARKET)
		{
			strText = "外盘";
		}
		else
		{
			strText = "未知";
		}
	}
	else
	{
		if(nMainMarket == HK_MARKET)
		{
			strText = "港股";
		}
		else if(nMainMarket == STOCK_MARKET)
		{
			if(MakeMainMarket(nMarket) == SH_Bourse)
			{
				strText = "上证";
			}
			else if(MakeMainMarket(nMarket) == SZ_Bourse)
			{
				strText = "深证";
			}
			else if (MakeMainMarket(nMarket) == (STOCK_MARKET|SH_BOURSE|SZ_BOURSE))		//add by lxqi 20090721	添加板块路径
			{
				strText = "板块";
			}
			else
			{
				strText = "股票";
			}
		}
		else if(nMainMarket == FUTURES_MARKET)
		{
			strText =  "期货";
		}
		else if(nMainMarket == FOREIGN_MARKET)
		{
			strText = "外汇";
		}
		else if(nMainMarket == WP_MARKET)
		{
			strText = "外盘";
		}
		else
		{
			strText = "未知";
		}
	}
	return strText;
}

BOOL IsValidAsk(AskData* pAsk, short nValidSize=0, unsigned short m_nType = 0)
{
	if(pAsk == NULL)
	{
		return FALSE;
	}
	BOOL bIsValid = (m_nType != 0) ? (pAsk->m_nType == m_nType) : TRUE;
	/* 请求证券总数 */
	if(pAsk->m_nSize > 0 ) 
	{
		bIsValid = bIsValid && ((pAsk->m_nSize * sizeof(CodeInfo)) >= (unsigned short)nValidSize);
	}
	else if(pAsk->m_nSize < 0 )
	{
		bIsValid = bIsValid && (abs(pAsk->m_nSize) >= nValidSize);
	}
	else
	{
	}
	return bIsValid;
}
CDataCache::CDataCache(void)
{
	m_nCurlate = 0;
	m_nSvrTine = 0;
}

CDataCache::~CDataCache(void)
{
}

void* CDataCache::MakeDayData( AskData*& pCurAsk ,long& lRetSize,BOOL& bSend)
{
	bSend = TRUE;
	lRetSize = 0;
	if( !IsValidAsk(pCurAsk) )
	{
		return NULL;
	}

	AskData* pAsk = (AskData*)pCurAsk;
	ReqDayData* pDayAskData = (ReqDayData*)pAsk->m_pCode;
	if( pDayAskData->m_nDay <= 0 )
	{
		return NULL;
	}
	BOOL bMinute1 = FALSE;
	if ( pAsk->m_lKey == PERIOD_TYPE_MINUTE1)
		bMinute1 = TRUE;
	int nReqCount = pDayAskData->m_nDay;
	short nPeriod = pDayAskData->m_nPeriodNum; // 请求周期
	int nRequestCount = pDayAskData->m_nDay;  //请求天数
	
	/* 读取数据, 并按照周期转换 */
	//第一次请求时 直接读取所有文件数据送给k线
	RetDiskData* pRetData = (RetDiskData*)(m_disk.GetDayData(pAsk,2));
	if( pRetData == NULL )
	{
		return NULL;
	}

	long nSize = pRetData->m_lSize; //返回日线数据长度 
	long nBeginDay = 0;
	
	CodeInfo* pCode = pAsk->m_pCode;
	long bufSize = nSize*sizeof(StockCompDayDataEx) + sizeof(AnsDayDataEx) - sizeof(StockCompDayDataEx);
	char*  phBuff = new  char[bufSize];
	lRetSize = bufSize;
	if(phBuff == NULL)
	{
		return FALSE;
	}

	AnsDayDataEx* pData = (AnsDayDataEx*)phBuff;
	MakeHeadData(&pData->m_dhHead, pAsk);
	pData->m_nSize  = nSize;
	/*StockCompDayDataEx* pTemp = pData->m_sdData;*/
	//memcpy(pTemp, pRetData->m_pData,nSize*sizeof(StockCompDayDataEx));
	//按日期排序
 	StockCompDayDataEx* pComp = (StockCompDayDataEx*)pRetData->m_pData;
  	KQuickSort qsData;
  	qsData.m_pData      = pComp;
  	qsData.m_nDataLen   = sizeof(StockCompDayDataEx);
  	qsData.m_nKeyOffset = 0;
  	qsData.m_nKeyLen    = QS_LONG;
  	qsData.m_nLow       = 0;
  	qsData.m_nHigh      = nSize - 1;
  	QuickSortAscend2(&qsData);	
  
   	StockCompDayDataEx* pStockDay = pComp;
   	CArray<StockCompDayDataEx*,StockCompDayDataEx*> ayStock;
   	int nCount = nSize ;
   	long lPreDay = 0;
   	for(int i = 0; i < nCount; i++ )
   	{
   		if ( 0 >= pStockDay[i].m_lDate || pStockDay[i].m_lDate == lPreDay   //重复，错误数据过滤	
   			|| 	pStockDay[i].m_lOpenPrice == 0
   			||  pStockDay[i].m_lClosePrice == 0
   			||  pStockDay[i].m_lTotal == 0)
   		{
   			continue;
   		}
   		else
   			ayStock.Add(pStockDay+i);
   		lPreDay = pStockDay[i].m_lDate;
   	}
  	pData->m_nSize = ayStock.GetCount();
  	StockCompDayDataEx* pTemp = pData->m_sdData;
  
	int nOffSet = 0;
	//分时画指标线时 需要请求1分钟线，此处请求多少就返回多少(解决公式计算报错)
	if ( ayStock.GetCount() > nReqCount && bMinute1)
	{
		nOffSet = ayStock.GetCount() - nReqCount;
		pData->m_nSize = nReqCount;
	}
  	for (int i=nOffSet; i<ayStock.GetCount(); i++)
  	{							
  		memcpy((void*)pTemp, (void*)ayStock.GetAt(i), sizeof(StockCompDayDataEx));
  		pTemp++;
  	}
	return phBuff;
}

void* CDataCache::MakeHisTraceData( AskData* pCurAsk ,long& lRetSize)
{
	long nSize=0;
	void* pData = m_disk.GetHisTraceData(pCurAsk,nSize);
	if (pData == NULL)
		return NULL;
	long bufSize = nSize + sizeof(AnsHisTrend) - sizeof(StockCompHistoryData);
	char* buffer = new char[bufSize];
	 lRetSize = bufSize;
	if (pData && !IsBadReadPtr(pData,1))
	{
		AnsHisTrend* pHisTrend = (AnsHisTrend*)buffer;
		MakeHeadData(&pHisTrend->m_dhHead, pCurAsk);
		StockCompHistoryData* pTemp = pHisTrend->m_shTend.m_shData;
		memcpy(pTemp,pData, nSize);
	}
	delete[] pData;
	return (void*)buffer;
}

void* CDataCache::MakeTickData(AskData* pCurAsk,long& lRetSize)
{
	long nSize;
	void*pData = m_disk.GetTickData(pCurAsk,nSize,m_nCurlate);
	if (pData == NULL)
		return NULL;
	long bufSize = nSize + sizeof(AnsStockTick) - sizeof(StockTick);
	char* buffer = new char[bufSize];
	 lRetSize = bufSize;
	if (pData && !IsBadReadPtr(pData,1))
	{
		AnsStockTick* pTick = (AnsStockTick*)buffer;
		pTick->m_nSize = nSize/sizeof(StockTick);
		MakeHeadData(&pTick->m_dhHead, pCurAsk);
		StockTick* pTemp = pTick->m_traData;
		memcpy(pTemp,pData, nSize);
	}
	delete[] pData;
	return (void*)buffer;
}

void* CDataCache::MakeTraceNewData( AskData* pCurAsk,long& lRetSize )
{   	
	 long lSize;
	 void* pData = m_disk.GetTraceData(pCurAsk,lSize,m_nCurlate);
	 if (pData == NULL || !IsValidAsk(pCurAsk))
		return NULL;
	 long bufSize = lSize + sizeof(AnsTrendData_Ext) - sizeof(PriceVolItem_Ext);
	 char* buffer = new char[bufSize];
	 lRetSize = bufSize;
	 if (pData && !IsBadReadPtr(pData,1))
	 {
		 AnsTrendData_Ext* pTrend = (AnsTrendData_Ext*)buffer;
		 pTrend->m_nHisLen = lSize/sizeof(PriceVolItem_Ext);
		 MakeHeadData(&pTrend->m_dhHead, pCurAsk);
		 PriceVolItem_Ext* pTemp = pTrend->m_pHisData;
		 memcpy(pTemp,pData, lSize);
	 }
	 delete[] pData;
	 return (void*)buffer;
}

void CDataCache::WriteDayData( CodeInfo* info, int period,StockCompDayDataEx* pNewData,long nSize )
{
	m_disk.WriteDayDataNew(info,period,pNewData,nSize);
}
//历史分时
void CDataCache::WriteHisTraceData(void* pData, CodeInfo* pCode)
{
	if (pData == NULL)
		return;
	PacketInfo* packet = (PacketInfo*)pData;
	DataHead* pHead = (DataHead*)packet->m_pszData;
	AnsHisTrend* pAnsHisTrend = (AnsHisTrend*)pHead;
	long l_Size =  sizeof(StockCompHistoryData) * (pAnsHisTrend->m_shTend.m_shHead.m_nSize );
	m_disk.WriteHisTraceData(pCode,pAnsHisTrend->m_shTend.m_shHead.m_lDate,(char*)pAnsHisTrend->m_shTend.m_shData,
		l_Size );
}
//分时走势
void CDataCache::WriteTraceNewData(void* pData, CodeInfo* pCode)
{
	if (pData == NULL)
		return;
	PacketInfo* packet = (PacketInfo*)pData;
	DataHead* pHead = (DataHead*)packet->m_pszData;
	AnsTrendData_Ext* pAnsHisTrend = (AnsTrendData_Ext*)pHead;
	long l_Size = sizeof(PriceVolItem_Ext) * pAnsHisTrend->m_nHisLen;
	m_disk.WriteNewTraceData(pCode,(char*)pAnsHisTrend->m_pHisData,l_Size,m_nCurlate);
}
//tick
void CDataCache::WriteTickData( void* pData ,CodeInfo* pCode)
{
	PacketInfo* packet = (PacketInfo*)pData;
	DataHead* pHead = (DataHead*)packet->m_pszData;
	AnsStockTick* pAnsTick = (AnsStockTick*)pHead;
	m_disk.WriteTickData(pCode,pAnsTick->m_traData,pAnsTick->m_nSize*sizeof(StockTick),m_nCurlate);
}

void CDataCache::MakeHeadData( DataHead* pHead, AskData* pAsk)
{
	memset(pHead,0,sizeof(DataHead));
	if(pAsk != NULL)
	{
		pHead->m_nType  = pAsk->m_nType;
		pHead->m_nIndex = pAsk->m_nIndex;

		pHead->m_lKey		  = pAsk->m_lKey;
		pHead->m_nPrivateKey = pAsk->m_nPrivateKey;
	}
}

void* CDataCache::MakeInfoData( AskData* pAsk ,long& lRetSize)
{
	lRetSize = 0;
	CInfoData info;
	void* pReadData =  info.Read(pAsk,lRetSize);
	if ( pReadData == NULL )
		return NULL;
	long lSize = lRetSize + (long)(sizeof(AnsTextData) - sizeof(char));
	unsigned char* phBuff = new unsigned char[lSize];
	AnsTextData* pData = (AnsTextData*)phBuff;
	MakeHeadData(&pData->m_dhHead, pAsk);
	ReqTextData* pTextDataReq = (ReqTextData*)pAsk->m_pCode;
	strncpy(pData->m_sMarkData.m_cTitle,pTextDataReq->m_sMarkData.m_cTitle,
		sizeof(pData->m_sMarkData.m_cTitle));

	strncpy(pData->m_sMarkData.m_szInfoCfg,pTextDataReq->m_sMarkData.m_szInfoCfg,
		sizeof(pData->m_sMarkData.m_szInfoCfg));
	pData->m_nSize = lRetSize;
	char* pTemp = pData->m_cData;
	memcpy(pTemp, pReadData, lRetSize);
	lRetSize = lSize;
	delete[] pReadData;
	return pData;
}

void CDataCache::WriteInfoData( void* pData ,CodeInfo* pCode)
{
	if ( pData == NULL )
		return;
	CInfoData info;
	info.Write(pData,pCode);
}



void* CDataCache::ReadFile( AskData* pAsk ,long& lRetSize)
{
	if ( m_nCurlate > m_nSvrTine )
		return NULL;
	if( !IsValidAsk(pAsk) )
	{
		return NULL;
	}
	switch (pAsk->m_nType)
	{
	case RT_TREND_EXT:          //分时走势
		{
			return MakeTraceNewData(pAsk, lRetSize);
		}
	case RT_HISTREND:           //历史分时
		{
			//return MakeHisTraceData(pAsk,lRetSize);
			break;
		}
	case RT_STOCKTICK:              //tick
		{
			return MakeTickData(pAsk,lRetSize);
		}
	case RT_TEXTDATAWITHINDEX_NEGATIVE:
	case RT_TEXTDATAWITHINDEX_PLUS:
	case RT_BYINDEXRETDATA:    //资讯
		{
			return MakeInfoData(pAsk, lRetSize);
		}
	}
	return NULL;
}


void CDataCache::WriteFile( void* pData , CodeInfo* pCode)
{
	if ( m_nCurlate > m_nSvrTine )
		return;
	if (pData == NULL || IsBadReadPtr(pData,1) || pCode == NULL || pCode->GetCode().IsEmpty())
		return;
	PacketInfo* packet = (PacketInfo*)pData;
	DataHead* pHead = (DataHead*)packet->m_pszData;
	switch ( pHead->m_nType )
	{
	case RT_TEXTDATAWITHINDEX_NEGATIVE:
	case RT_TEXTDATAWITHINDEX_PLUS:
	case RT_BYINDEXRETDATA:    //资讯
		{
			WriteInfoData(  pData ,pCode);
			break;
		}
	case RT_STOCKTICK:                //tick
		{
			WriteTickData(pData,pCode);
			break;
		}
	case RT_TREND_EXT:              //分时
		{
			WriteTraceNewData(pData,pCode);
			break;
		}
	case RT_HISTREND:					//历史分时
		{
			//WriteHisTraceData(pData,pCode);
			break;
		}
	default:
		{
			break;
		}
	}
	
}

void CDataCache::SetDate( long lCurDate, long lSvrTime )
{
	m_nCurlate = lCurDate;
	m_nSvrTine = lSvrTime;
}

BOOL CDataCache::DelTraceFile()
{
	CString strParent =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::DATA_PATH);
	return CTrendFile::DelFile(strParent);
}