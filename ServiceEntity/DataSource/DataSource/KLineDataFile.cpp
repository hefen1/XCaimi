#include "stdafx.h"
#include "klinedatafile.h"
#include "hsds_comudata.h"
#include "GeneralHelper.h"
#include "WinnerApplication.h"
#include <WindowsX.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

extern void CreatePath(CString);
extern BOOL CreateMultipleDirectory(char * sDirectory);
extern BOOL MakeFile(CString strFileName);
static	CMapStringToOb  m_klineFileMap;  //日线数据文件map表
static  CMapStringToOb  m_klineIndexFileMap;  //索引文件map表

char* GetMarketName(int cBourse,char* strRet,int nLen)
{
	memset(strRet,0,nLen);

	switch(MakeMainMarket(cBourse))
	{
	case SH_Bourse:	strncpy(strRet,"国内股票-上证证券",nLen);	break;
	case SZ_Bourse:	strncpy(strRet,"国内股票-深证证券",nLen);	break;

	case (FUTURES_MARKET | DALIAN_BOURSE):		strncpy(strRet,"期货-大连期货",nLen);	break;
	case (FUTURES_MARKET | SHANGHAI_BOURSE): 	strncpy(strRet,"期货-上海期货",nLen);	break;
	case (FUTURES_MARKET | ZHENGZHOU_BOURSE):	strncpy(strRet,"期货-郑州期货",nLen);	break;
	case (FUTURES_MARKET | HUANGJIN_BOURSE):	strncpy(strRet,"期货-黄金期货",nLen);	break;
	case (FUTURES_MARKET | GUZHI_BOURSE):		strncpy(strRet,"期货-股指期货",nLen);	break;

	case (FOREIGN_MARKET | WH_BASE_RATE):		strncpy(strRet,"外汇-基本汇率",nLen);	break;
	case (FOREIGN_MARKET | WH_ACROSS_RATE): 	strncpy(strRet,"外汇-交叉汇率",nLen);	break;
	case (FOREIGN_MARKET | WH_FUTURES_RATE):	strncpy(strRet,"外汇-期汇",nLen);		break;

		//case (WP_MARKET | WP_LME		):		strncpy(strRet,"外盘-LME"	);	break;
	case (WP_MARKET | WP_LME_CLT	):		strncpy(strRet,"外盘-场内铜"	,nLen);	break;
	case (WP_MARKET | WP_LME_CLL	):		strncpy(strRet,"外盘-场内铝"	,nLen);	break;
	case (WP_MARKET | WP_LME_CLM	):		strncpy(strRet,"外盘-场内镍"	,nLen);	break;
	case (WP_MARKET | WP_LME_CLQ	):		strncpy(strRet,"外盘-场内铅"	,nLen);	break;
	case (WP_MARKET | WP_LME_CLX	):		strncpy(strRet,"外盘-场内锌"	,nLen);	break;
	case (WP_MARKET | WP_LME_CWT	):		strncpy(strRet,"外盘-场外铝"	,nLen);	break;
	case (WP_MARKET | WP_LME_CW		):		strncpy(strRet,"外盘-场外"  	,nLen);	break;

	case (WP_MARKET | WP_CBOT		):		strncpy(strRet,"外盘-CBOT"	,nLen);	break;
	case (WP_MARKET | WP_NYMEX	 	):		strncpy(strRet,"外盘-NYMEX"	,nLen);	break;
	case (WP_MARKET | WP_COMEX	 	):		strncpy(strRet,"外盘-COMEX"	,nLen);	break;
	case (WP_MARKET | WP_TOCOM	 	):		strncpy(strRet,"外盘-TOCOM"	,nLen);	break;
	case (WP_MARKET | WP_IPE		):		strncpy(strRet,"外盘-IPE"	,nLen);	break;
	case (WP_MARKET | WP_NYBOT		):		strncpy(strRet,"外盘-NYBOT"	,nLen);	break;
	case (WP_MARKET | WP_NOBLE_METAL):		strncpy(strRet,"外盘-贵金属",nLen);	break;
	case (WP_MARKET | WP_FUTURES_INDEX):	strncpy(strRet,"外盘-期指"	,nLen);	break;
	case (WP_MARKET | WP_SICOM		):		strncpy(strRet,"外盘-SICOM"	,nLen);	break;
	case (WP_MARKET | WP_LIBOR		):		strncpy(strRet,"外盘-LIBOR"	,nLen);	break;

	case (WP_MARKET | WP_INDEX_AZ	):		strncpy(strRet,"国际指数-澳洲"	 ,nLen);	break;
	case (WP_MARKET | WP_INDEX_OZ	):		strncpy(strRet,"国际指数-欧洲"	 ,nLen);	break;
	case (WP_MARKET | WP_INDEX_MZ	):		strncpy(strRet,"国际指数-美洲"	 ,nLen);	break;
	case (WP_MARKET | WP_INDEX_TG	):		strncpy(strRet,"国际指数-泰国"	 ,nLen);	break;
	case (WP_MARKET | WP_INDEX_YL	):		strncpy(strRet,"国际指数-印尼"	 ,nLen);	break;
	case (WP_MARKET | WP_INDEX_RH	):		strncpy(strRet,"国际指数-日韩"	 ,nLen);	break;
	case (WP_MARKET | WP_INDEX_XHP	):		strncpy(strRet,"国际指数-新加坡" ,nLen);	break;
	case (WP_MARKET | WP_INDEX_FLB	):		strncpy(strRet,"国际指数-菲律宾" ,nLen);	break;
	case (WP_MARKET | WP_INDEX_CCN	):		strncpy(strRet,"国际指数-中国大陆",nLen);	break;
	case (WP_MARKET | WP_INDEX_TW	):		strncpy(strRet,"国际指数-中国台湾",nLen);	break;
	case (WP_MARKET | WP_INDEX_MLX	):		strncpy(strRet,"国际指数-马来西亚",nLen);	break;

	case (HK_MARKET | HK_BOURSE		):		strncpy(strRet,"港股-主板市场"	   ,nLen);	break;
	case (HK_MARKET | GE_BOURSE		):		strncpy(strRet,"港股-创业板市场"   ,nLen);	break;
	case (HK_MARKET | INDEX_BOURSE	):		strncpy(strRet,"港股-指数市场"	   ,nLen);	break;

	default:  
		sprintf(strRet,"%hx", cBourse);
		break;
	}

	return strRet;
}

CKLineDataFile::CKLineDataFile(DWORD dwDefaultExpand)/*:m_mapViewFile(dwDefaultExpand),m_mapViewFile(dwDefaultExpand)*/
{
	m_pKlineFile = NULL;
	m_pIndexFile = NULL;
	m_IndexBuffer = NULL;
}

CKLineDataFile::CKLineDataFile()
{
	m_pKlineFile = NULL;
	m_pIndexFile = NULL;
	m_IndexBuffer = NULL;
}
CKLineDataFile::~CKLineDataFile(void)
{
	if ( m_pKlineFile )
	{
		m_pKlineFile->Close();
		delete m_pKlineFile;
	}
	if ( m_pIndexFile )
	{
		m_pIndexFile->Close();
		delete m_pIndexFile;
	}
	if ( m_IndexBuffer && !IsBadReadPtr(m_IndexBuffer,1))
		delete[] m_IndexBuffer;
	m_IndexBuffer = NULL;
}



CTimeSpan CKLineDataFile:: GetTimeStep(short nPeriod)
{
	if(nPeriod == PERIOD_TYPE_DAY)
	{
		CTimeSpan ts(1,0,0,0);
		return ts;
	}
	else if (nPeriod == PERIOD_TYPE_MINUTE1)
	{
		CTimeSpan ts(0,0,1,0);
		return ts;
	}
	else if (nPeriod == PERIOD_TYPE_MINUTE5)
	{
		CTimeSpan ts(0,0,5,0);
		return ts;
	}
	else if (nPeriod == PERIOD_TYPE_MINUTE15)
	{
		CTimeSpan ts(0,0,15,0);
		return ts;
	}
	else if (nPeriod == PERIOD_TYPE_MINUTE30)
	{
		CTimeSpan ts(0,0,30,0);
		return ts;
	}
	else if (nPeriod == PERIOD_TYPE_WEEK)
	{
		CTimeSpan ts(7,0,0,0);
		return ts;
	}
	else if (nPeriod == PERIOD_TYPE_MONTH)
	{
		CTimeSpan ts(30,0,0,0);
		return ts;
	}
	else
	{
		CTimeSpan ts(0,0,0,0);
		return ts;
	}
}

CTime CKLineDataFile::GetNowTime(short nPeriod)
{
	if(nPeriod == PERIOD_TYPE_DAY)
	{
		CTime now = CTime::GetCurrentTime();
		CTime t(now.GetYear(),now.GetMonth(),now.GetDay(),0,0,0);
		return t;
	}
	else if (nPeriod == PERIOD_TYPE_MINUTE1)
	{
		CTime now = CTime::GetCurrentTime();
		CTime t(now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),now.GetMinute(),0);
		return t;
	}
	else if (nPeriod == PERIOD_TYPE_MINUTE5)
	{
		CTime now = CTime::GetCurrentTime();
		int nMinute = now.GetMinute() / 5 * 5;
		CTime t(now.GetYear(),now.GetMonth(),now.GetDay(),now.GetHour(),nMinute,0);
		return t;
	}
	else
	{
		return CTime::GetCurrentTime();
	}
}
CTime CKLineDataFile::GetTimeFromIntTime(unsigned long lDate,short nPeriod)
{
	if (lDate < 19900101)
	{
		CString strErroinfol;
		strErroinfol.Format("%d",lDate);
		strErroinfol += _T("[CKLineDataFile::MakeTimeFromIntTime]遇到错误的时间日期:");
		TRACE(strErroinfol);
		return CTime::GetCurrentTime();
	}

	if(nPeriod == PERIOD_TYPE_DAY)
	{
		int nYear,nMonth,nDay;
		nYear = lDate / 10000;
		nMonth = (lDate - nYear * 10000) / 100;
		nDay = lDate - nYear * 10000 - nMonth * 100;

		CTime t(nYear,nMonth,nDay,0,0,0);
		return t;
	}
	else if (nPeriod == PERIOD_TYPE_MINUTE5 || nPeriod == PERIOD_TYPE_MINUTE1)
	{
		//1907061434
		int nYear,nMonth,nDay,nHour,nMin;
		int nYear0 =  lDate / 100000000;
		nYear =   1990 + nYear0;
		nMonth = (lDate - nYear0 * 100000000) / 1000000;
		nDay =   (lDate - nYear0 * 100000000 - nMonth * 1000000) / 10000;
		nHour =  (lDate - nYear0 * 100000000 - nMonth * 1000000 - nDay * 10000) / 100;
		nMin =    lDate - nYear0 * 100000000 - nMonth * 1000000 - nDay * 10000 - nHour * 100;
		CTime t(nYear,nMonth,nDay,nHour,nMin,0);
		return t;
	}
	else
	{
		return CTime::GetCurrentTime();
	}
}


unsigned long  CKLineDataFile::GetIntTimeFromTime(const CTime &t,short nPeriod)
{
	switch(nPeriod)
	{
	case PERIOD_TYPE_DAY:
		return t.GetDay() + t.GetMonth() * 100 + t.GetYear() * 10000;
	case PERIOD_TYPE_MINUTE5:
	case PERIOD_TYPE_MINUTE1:
		long lDate = t.GetMinute() 
			+ t.GetHour() * 100
			+ t.GetDay()   * 10000
			+ t.GetMonth() * 1000000 
			+ (t.GetYear() - 1990)* 100000000;
		return lDate;
	}
	return 0;
}
unsigned long CKLineDataFile::GetLimitDate(CodeInfo *pCode,short nPeriod,int nDays)
{
	static int min1_download_limit_day = 0;
	static int min5_download_limit_day = 0;

	if(min1_download_limit_day == 0 || min5_download_limit_day == 0) 
	{
		CString strConfigFile = CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) + "sysconfig.ini";
		min1_download_limit_day = ::GetPrivateProfileInt("data_download","min1_download_limit_day",33000,strConfigFile);
		min5_download_limit_day = ::GetPrivateProfileInt("data_download","min15_download_limit_day",33000,strConfigFile);

	}

	short nGZIndex = MakeGZIndex(pCode->m_cCodeType);
	short nMainType  = MakeMarket(pCode->m_cCodeType);
	int nBeing1min = 931;
	int nBegin5min = 935;
	if (nMainType != STOCK_MARKET)
	{
		if (nGZIndex)
		{
			nBeing1min = 916;
			nBegin5min = 920;
		}
		else
		{
			nBeing1min = 901;
			nBegin5min = 905;
		}
	}

	CTime tm20(2000,1,5,0,0,0);
	CTime t = GetNowTime(nPeriod);
	CTimeSpan tmSpan = t - tm20;
	long lCurTime = t.GetYear()*10000 + t.GetMonth()*100 + t.GetDay();
	long nLimitedTime = 0;
	switch(nPeriod)
	{
	case PERIOD_TYPE_DAY:
		{
			if (nDays == 0)
				nLimitedTime =  19900101;
			else
			{
				CTimeSpan tSpanDay(nDays,0,0,0);
				nLimitedTime = GetIntTimeFromTime(t - tSpanDay,nPeriod);
			}
			break;
		}	
	case PERIOD_TYPE_MINUTE5:
		{
			int nLimit = 10;
			if (nDays != 0)
				nLimit = nDays < min5_download_limit_day ? nDays : min5_download_limit_day;
			else
				nLimit = tmSpan.GetDays();

			CTimeSpan tSpan5(nLimit,0,0,0);
			CTime nLimitTm = t - tSpan5;
			nLimitedTime = nBegin5min+ nLimitTm.GetDay()* 10000 + nLimitTm.GetMonth() * 1000000 + (nLimitTm.GetYear() - 1990)* 100000000;
			//nLimitedTime =  GetIntTimeFromTime(t - tSpan5,nPeriod);
			break;
		}
	case PERIOD_TYPE_MINUTE1:
		{
			int nLimit = 10;
			if (nDays != 0)
				nLimit = nDays < min1_download_limit_day ? nDays : min1_download_limit_day;
			else
				nLimit = tmSpan.GetDays();

			CTimeSpan tSpan1(nLimit,0,0,0);
			CTime nLimitTm = t - tSpan1;
			nLimitedTime = nBeing1min + nLimitTm.GetDay()* 10000 + nLimitTm.GetMonth() * 1000000 + (nLimitTm.GetYear() - 1990)* 100000000;
			break;
		}
	}
	return nLimitedTime;
}

void CKLineDataFile::GetFileHeadOption(KLineDataFileHead *pHead,short nPeriod)
{
	CString strConfigFile = "";//::GetPath(//注释 by xudan ::Setting) + "sysconfig.ini"; //注释by xudan
	pHead->m_nVersion = KLINEDATAFILE_VERSION_1;
	switch(nPeriod)
	{
	case PERIOD_TYPE_DAY:
		{	
			pHead->m_nIndexCount = ::GetPrivateProfileInt("data_file","day_index_count",3000,strConfigFile);
			pHead->m_nIndexCapability = ::GetPrivateProfileInt("data_file","day_index_size",20,strConfigFile);
			pHead->m_nPageCapability = ::GetPrivateProfileInt("data_file","day_page_size",500,strConfigFile);
			break;
		}
	case PERIOD_TYPE_MINUTE5:
		{
			pHead->m_nIndexCount = ::GetPrivateProfileInt("data_file","min5_index_count",3000,strConfigFile);
			pHead->m_nIndexCapability = ::GetPrivateProfileInt("data_file","min5_index_size",10,strConfigFile);
			pHead->m_nPageCapability = ::GetPrivateProfileInt("data_file","min5_page_size",1000,strConfigFile);
			break;
		}
	case PERIOD_TYPE_MINUTE1:
		{
			pHead->m_nIndexCount = ::GetPrivateProfileInt("data_file","min1_index_count",3000,strConfigFile);
			pHead->m_nIndexCapability = ::GetPrivateProfileInt("data_file","min1_index_size",10,strConfigFile);
			pHead->m_nPageCapability = ::GetPrivateProfileInt("data_file","min1_page_size",1000,strConfigFile);
			break;
		}
	default:
		{
			pHead->m_nVersion = -1;
		}
	}

}


CKLineDataFile* CKLineDataFile::GetIndexFile( CString strKey ,short nPeriod)
{
	//加锁
	static  CCriticalSection mapCritical;
	CriticalGuard gurard(&mapCritical);
	CString strConfigFile = "";
	int nDefaultExpand = ::GetPrivateProfileInt("data_file","default_expand",1024 * 1024 * 8,strConfigFile);
	CKLineDataFile* pKLineDataFile=NULL;
	

	if ( !m_klineIndexFileMap.Lookup(strKey, (CObject*&)pKLineDataFile))
	{
		//取文件头信息
		pKLineDataFile = new CKLineDataFile();
		memset(&pKLineDataFile->m_head,0,sizeof(pKLineDataFile->m_head));
		GetFileHeadOption(&pKLineDataFile->m_head,nPeriod);

		//映射索引文件
		pKLineDataFile->OpenIndexData(strKey);

		m_klineIndexFileMap.SetAt(strKey,(CObject*)pKLineDataFile);
	}
	return pKLineDataFile;
}

DWORD CKLineDataFile::GetIndexData(CodeInfo* pCode,int nPeriod,CString filename,KLineDataFileHead& head,  CArray<PageInfo,PageInfo>& ayPageInfo)
{	
		
	CKLineDataFile* pKLineDataFile = GetIndexFile(filename,nPeriod);
	if ( pKLineDataFile == NULL || (pKLineDataFile && pKLineDataFile->m_pIndexFile == NULL))
		return 0;
	CriticalGuard guard(&pKLineDataFile->m_critical);

	//取索引 索引大小 1个codeinfo+n*pageinfo
	int nIndexSize = sizeof(CodeInfo) + pKLineDataFile->m_head.m_nIndexCapability * sizeof(PageInfo);
	DWORD dwOffset = 0;

	if (pKLineDataFile->m_IndexBuffer)
		delete[] pKLineDataFile->m_IndexBuffer;

	pKLineDataFile->m_pIndexFile->SeekToBegin();
	KLineDataFileHead tagHead;
	memset(&tagHead,0,sizeof(tagHead));
	// 读取通用文件头
	pKLineDataFile->m_pIndexFile->SeekToBegin();
	pKLineDataFile->m_pIndexFile->Read(&tagHead, sizeof(KLineDataFileHead));
	if (tagHead.m_nVersion >0 || tagHead.m_nIndexCount >0)
		memcpy(&pKLineDataFile->m_head, &tagHead, sizeof(KLineDataFileHead));

	memcpy(&head, &pKLineDataFile->m_head, sizeof(KLineDataFileHead));
	//读索引
	unsigned int nSize = nIndexSize*pKLineDataFile->m_head.m_nIndexCount;
	pKLineDataFile->m_IndexBuffer = new char[nSize];
	memset(pKLineDataFile->m_IndexBuffer,0,nSize);
	pKLineDataFile->m_pIndexFile->Seek(sizeof(KLineDataFileHead), CFile::begin);
	pKLineDataFile->m_pIndexFile->Read(pKLineDataFile->m_IndexBuffer,nSize);
	char* pTemp = pKLineDataFile->m_IndexBuffer;
	
	for(int i = 0; i < pKLineDataFile->m_head.m_nIndexCount; i++)
	{
		dwOffset = i * nIndexSize;
	
		IndexInfo*pIndexInfo = (IndexInfo*)pTemp;
		if (pIndexInfo == NULL )
		{
			pTemp += nIndexSize;
			continue;
		}

		if(IsSameCode(pCode,&(pIndexInfo->m_CodeInfo)) || pIndexInfo->IsEmptyIndex())
		{	
			break;
		}
		pTemp += nIndexSize;
	}
	
	dwOffset +=  sizeof(KLineDataFileHead);
	//取页信息
	IndexInfo *pIndexInfo = (IndexInfo*)pTemp;
	if ( pIndexInfo && !IsBadReadPtr(pIndexInfo,1))
	{
		for(int i=0;i< pKLineDataFile->m_head.m_nIndexCapability;i++)
		{
			ayPageInfo.Add(pIndexInfo->m_pages[i]);
		}	
	}		
	return dwOffset;
}


int CKLineDataFile::GetDefaultDownloadDay()
{
	CString strConfigFile = ""; ////注释 by xudan ::GetPath(//注释 by xudan ::Setting) + "sysconfig.ini"; //注释by xudan
	int default_download_days = 0;
	default_download_days = ::GetPrivateProfileInt("data_download","default_download_days",10,strConfigFile);
	return default_download_days;
}

CKLineDataFile* CKLineDataFile::GetKLineDataFile(CString strKey,BOOL bReleaseFile)
{
	static  CCriticalSection mapCritical;

	CKLineDataFile* pKLineDataFile=NULL;
	if (bReleaseFile)
	{
		if (m_klineFileMap.Lookup(strKey,(CObject*&)pKLineDataFile))
		{
			m_klineFileMap.RemoveKey(strKey);
			delete pKLineDataFile;
			pKLineDataFile = NULL;	
		}
		return NULL;
	}

	//加锁
	CriticalGuard gurard(&mapCritical);
	
	if(!m_klineFileMap.Lookup(strKey,(CObject*&)pKLineDataFile) )
	{
	
		CString strConfigFile = "";//by xudan ::Setting) + "sysconfig.ini";
		int nDefaultExpand = ::GetPrivateProfileInt("data_file","default_expand",1024 * 1024 * 8,strConfigFile);
		pKLineDataFile = new CKLineDataFile();

		//打开文件
		pKLineDataFile->Open(strKey);
		m_klineFileMap.SetAt(strKey,(CObject*)pKLineDataFile);	

	}
	return pKLineDataFile;
}


BOOL CKLineDataFile::IsSameCode(const CodeInfo* pSrc,const CodeInfo* pDes)
{
	if( pSrc == NULL || pDes == NULL )
		return 0;

	CString str1 = ((CodeInfo*)pSrc)->GetCode();
	CString str2 = ((CodeInfo*)pDes)->GetCode();
	return ( pSrc->m_cCodeType == pDes->m_cCodeType && !str1.CompareNoCase(str2) );
}
void CKLineDataFile::ReleaseMapFile(CodeInfo *pCode,short nPeriod)
{
// 	CKLineDataFile* pKLineDataFile = GetKLineDataFile(pCode,nPeriod);
// 	if (pKLineDataFile)
// 	{
// 		pKLineDataFile->UnMapFile();
// 		GetKLineDataFile(pCode,nPeriod,TRUE);
// 	}
}
void CKLineDataFile::UnMapFile()
{
	
}
void CKLineDataFile::WriteKLineDataEx(CodeInfo *pCode,short nPeriod,char* buf,long nBufLen)
{
	WriteKLineData(pCode,nPeriod,buf,nBufLen);
	//ReleaseMapFile(pCode,nPeriod);
}
void CKLineDataFile::WriteKLineData(CodeInfo *pCode,short nPeriod,char* buf,long nBufLen)
{
	//去掉过期
	int nCount = nBufLen / sizeof(StockCompDayDataEx);
	if(nCount>9840)
	{
		StockCompDayDataEx *temp = (StockCompDayDataEx*)buf;
		temp = temp + (nCount - 9840);
		buf = (char*)temp;
		nCount = 9840;
	}

	unsigned long nLimitDay = CKLineDataFile::GetLimitDate(pCode,nPeriod,0);
	
	CArray<StockCompDayDataEx*,StockCompDayDataEx*> ayDay;
	StockCompDayDataEx *pStock = (StockCompDayDataEx*)buf;
	for(int i=0; i < nCount; i++)
	{
		if(pStock[i].m_lDate < nLimitDay)
			continue;
		ayDay.Add(&pStock[i]);
	}

	int nRealCount = ayDay.GetCount();
	if(nRealCount <= 0)
		return;

	//新数据缓存
	char* newBuf = (char*)GlobalAllocPtr(GHND, sizeof(StockCompDayDataEx) * nRealCount);
	long newBufLen = nRealCount * sizeof(StockCompDayDataEx);
	StockCompDayDataEx * pTemp = (StockCompDayDataEx*)newBuf;
	for(int i=0; i <nRealCount; i++,pTemp++)
	{
		memcpy(pTemp,ayDay.GetAt(i),sizeof(StockCompDayDataEx));
	}

	//取文件路径
	CString strIndexPath, strDataPath;
	GetFilePath(pCode, nPeriod,strIndexPath,strDataPath);

	//获取索引文件信息

	CArray<PageInfo,PageInfo> ayInfo;
	KLineDataFileHead head;
	DWORD dwIndexOffset = GetIndexData(pCode, nPeriod, strIndexPath, head,ayInfo);
	CString strKey = strDataPath;	
	//打开数据文件
	CKLineDataFile* pKLineDataFile;
	pKLineDataFile = GetKLineDataFile(strDataPath);
	
	if(pKLineDataFile == NULL)
	{
		return;
	}
	memcpy(&pKLineDataFile->m_head, &head,sizeof(KLineDataFileHead));
	//加锁
	CriticalGuard guard(&pKLineDataFile->m_critical);
	try
	{
		//把数据写入数据页
		int nPageSize = pKLineDataFile->GetOnePageSize();
		char* dataBuf = newBuf;
		long  nDataLen = newBufLen; 
		int nUperbound = ayInfo.GetUpperBound();
		for(int i=0; i <= nUperbound;i++)
		{
			PageInfo pageInfo = ayInfo[i]; 
			pageInfo.m_code = *pCode;
			pKLineDataFile->WriteOnePage(&pageInfo,dataBuf,nDataLen);
			ayInfo.SetAt(i,pageInfo);//更新pageinfo信息,WriteOnePage会更改它的值
			if( (nPageSize - sizeof(PageInfo)) * (i+1) >= newBufLen )
			{   
				//清空后续页:可能现有数据比原来数据少
				for(int j=i+1; j < ayInfo.GetCount(); j++ )
				{
					PageInfo pageInfo2 = ayInfo[j]; 
					pKLineDataFile->ClearOnePage(&pageInfo2);
					ayInfo.SetAt(j,pageInfo2);//更新pageinfo信息,ClearOnePage会更改它的值
				}
				break;
			}
			else
			{
				nDataLen = nDataLen - (nPageSize - sizeof(PageInfo)); //数据长度 - 已写掉的数据长度
				dataBuf = dataBuf + ( nPageSize - sizeof(PageInfo)); //数据缓存指针向前偏移已写掉的长度
			}
		}
		//写索引信息	
		CKLineDataFile* pIndexFile;
		if (m_klineIndexFileMap.Lookup(strIndexPath,(CObject*&)pIndexFile) && pIndexFile)
		{
			memcpy(&pIndexFile->m_head, &pKLineDataFile->m_head,sizeof(KLineDataFileHead));
			pIndexFile->WriteIndexInfo(dwIndexOffset,pCode,&ayInfo);
		}

	}
	catch(...)
	{
	}

	GlobalFree((HGLOBAL)newBuf);
}

BOOL CKLineDataFile::ReadKLineDataEx(CodeInfo *pCode,short nPeriod,char** buf,int &nDataCount)
{
	BOOL bRead = ReadKLineData(pCode,nPeriod,buf,nDataCount);
	//ReleaseMapFile(pCode,nPeriod);
	return bRead;
}

//读K线文件
BOOL CKLineDataFile::ReadKLineData(CodeInfo *pCode,short nPeriod,char** buf,int &nDataCount)
{

	//取文件路径
	int nGetCount = nDataCount;
	CString strIndexPath, strDataPath;
	GetFilePath(pCode, nPeriod,strIndexPath,strDataPath);

	//获取索引文件信息
	CArray<PageInfo,PageInfo> codePages;
	
	KLineDataFileHead head;
	DWORD dwIndexOffset = GetIndexData(pCode, nPeriod, strIndexPath, head,codePages);

	//打开数据文件
	CString strKey = strDataPath;	
	CKLineDataFile* pKLineDataFile;
	pKLineDataFile = GetKLineDataFile(strDataPath);

	if(pKLineDataFile == NULL)
	{
		return FALSE;
	}
	memcpy(&pKLineDataFile->m_head, &head,sizeof(KLineDataFileHead));
	//加锁
	CriticalGuard guard(&pKLineDataFile->m_critical);
	try
	{
		//计算个数
		nDataCount = 0;
		int nUperbound = codePages.GetUpperBound();
		for(int i=0; i <= nUperbound;i++ )
		{
			nDataCount += codePages[i].m_count;
		}

		if(nDataCount <= 0)
		{
			nDataCount = 0;
			*buf = 0;
			return FALSE;
		}
		*buf = (char*)new StockCompDayDataEx[nDataCount];
		memset(*buf,0,nDataCount*sizeof(StockCompDayDataEx));
		char *pBuf = *buf;

		//取数据
		int nPos = 0;
		for(int i=0; i <= nUperbound;i++ )
		{
			if( codePages[i].m_count > 0 )
			{
				DWORD dwLength =  sizeof(StockCompDayDataEx) * codePages[i].m_count;
				pKLineDataFile->ReadOnePage(&codePages[i],(LPVOID)pBuf,dwLength);
				pBuf += dwLength;

			}
		}

	}

	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CKLineDataFile::ClearOnePage(PageInfo* pPage)
{
	if(pPage->m_nPageNO != 0)
	{
		int   nPageSize = GetOnePageSize();
		//页偏移位置
		DWORD dwOffset = (pPage->m_nPageNO - 1) * nPageSize; 
		//清空页
		char* tempBuffer = new char[nPageSize];
		memset(tempBuffer,0,nPageSize);
		m_pKlineFile->Seek(dwOffset,CFile::begin);
		m_pKlineFile->Write(tempBuffer,nPageSize);
		delete []tempBuffer;
	}
	memset(pPage,0,sizeof(PageInfo));
	return TRUE;
}

BOOL CKLineDataFile::WriteOnePage(PageInfo* pPage,LPVOID pBuf,DWORD dwLength)
{
	int   nPageSize = GetOnePageSize();
	int   mOnePageMaxLen = nPageSize - sizeof(PageInfo); 
	DWORD dwWriteLen =  mOnePageMaxLen < dwLength ? mOnePageMaxLen : dwLength;
	//页中写的日线个数
	pPage->m_count = dwWriteLen / sizeof(StockCompDayDataEx);

	//页偏移位置
	unsigned int nPageNO  = pPage->m_nPageNO;
	nPageNO = nPageNO==0?1:nPageNO;
	DWORD dwOffset =  (nPageNO-1) * nPageSize; 	
	char* pBuffer = new char[sizeof(PageInfo)];
	memset(pBuffer,0,sizeof(PageInfo));
	m_pKlineFile->Seek(dwOffset,CFile::begin);
	m_pKlineFile->Read(pBuffer,sizeof(PageInfo));
	PageInfo* pHead = (PageInfo*)pBuffer;
	
	if ( 0 == pPage->m_nPageNO )  //新的页，在文件尾写
	{
		pPage->m_nPageNO = m_head.m_nPageNO + 1;
		dwOffset = nPageSize * m_head.m_nPageNO; 
		m_pKlineFile->Seek(dwOffset,CFile::begin);
		m_pKlineFile->Write(pPage,sizeof(PageInfo));//写入页信息
		dwOffset += sizeof(PageInfo);
		m_pKlineFile->Seek(dwOffset,CFile::begin);
		m_pKlineFile->Write(pBuf,dwWriteLen);
		m_head.m_nPageNO += 1;
	}
	else
	{
		if(IsSameCode(&pHead->m_code,&pPage->m_code))
		{
			m_pKlineFile->Seek(dwOffset,CFile::begin);
			m_pKlineFile->Write(pPage,sizeof(PageInfo));//向数据页中写入新的数据
			m_pKlineFile->Seek(dwOffset+sizeof(PageInfo),CFile::begin);
			m_pKlineFile->Write(pBuf,dwWriteLen);

			//清空页后面内容 
			long lSize = nPageSize - sizeof(PageInfo) - dwWriteLen;
			char* tempBuffer = new char[lSize];
			memset(tempBuffer,0,lSize);
			m_pKlineFile->Seek(dwOffset+sizeof(PageInfo)+dwWriteLen,CFile::begin);
			m_pKlineFile->Write(tempBuffer,lSize);
			delete []tempBuffer;
		}
	}
	delete[] pBuffer;
	return TRUE;
}

//读文件:从文件读dwLength到pBuf把指的空间,pBuf要事件
BOOL CKLineDataFile::ReadOnePage(const PageInfo* pPage,LPVOID pBuf,DWORD dwLength)
{
	if(pPage->m_nPageNO <= 0)
		return FALSE;

	int   nPageSize = GetOnePageSize();
	//第N页真正的数据偏移: 数据页起始偏移 + N -1页块 + 页信息
	DWORD dwOffset =(pPage->m_nPageNO -1) * nPageSize + sizeof(PageInfo); 
	char* pBuffer = new char[dwLength];
	memset(pBuffer,0,dwLength);
	m_pKlineFile->Seek(dwOffset,CFile::begin);
	m_pKlineFile->Read(pBuffer,dwLength);
	if ( pBuffer && !IsBadReadPtr(pBuffer,1))
		memcpy(pBuf,pBuffer,dwLength);
	delete[] pBuffer;
	return TRUE;
}

BOOL CKLineDataFile::WriteIndexInfo(DWORD dwIndexOffset,CodeInfo *pCode,CArray<PageInfo,PageInfo> *codePages)
{	
	CriticalGuard guard(&m_critical);
	//写code	
	if ( m_pIndexFile == NULL || IsBadReadPtr(m_pIndexFile,1))
		return FALSE;
	m_pIndexFile->SeekToBegin();
	m_pIndexFile->Write(&m_head, sizeof(m_head));
	m_pIndexFile->Seek(dwIndexOffset,CFile::begin);
	m_pIndexFile->Write(pCode,sizeof(CodeInfo));
	//写页信息
	int nUperbound = codePages->GetUpperBound();
	for(int i = 0; i < nUperbound; i++)
	{
		DWORD dwOffset = dwIndexOffset + sizeof(CodeInfo) + sizeof(PageInfo) * i;
		m_pIndexFile->Seek(dwOffset,CFile::begin);
		m_pIndexFile->Write(&codePages->GetAt(i),sizeof(PageInfo));
	}
	return TRUE;
}

BOOL CKLineDataFile::Open(CString strFileName)
{
	CriticalGuard guard(&m_critical);
	try
	{
		if ( MakeFile(strFileName) )
		{
			if (m_pKlineFile == NULL)
			{
				m_pKlineFile   = new CFile;
			}
			if(	!m_pKlineFile->Open(strFileName,  ( CFile::modeCreate |CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate|CFile::typeBinary)))
			{
				delete m_pKlineFile;
				m_pKlineFile = NULL;		
				return FALSE;
			}
		}	
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL CKLineDataFile::InitFile()
{
	return TRUE;//m_mapViewFile.Write(0,&m_head,sizeof(m_head));
}

//取缓存文件锁
CCriticalSection* CKLineDataFile::GetCriticalSection()
{
	return NULL;
}

DWORD CKLineDataFile::GetIndexOffset(CodeInfo *pCode)
{
	//取索引信息
	int nIndexSize = GetOneIndexSize();
	IndexInfo *pIndexInfo = NULL;

	for(int i = 0; i < m_head.m_nIndexCount; i++)
	{
		DWORD dwOffset = sizeof(m_head) + i * nIndexSize;
		pIndexInfo = NULL;//(IndexInfo*)m_mapViewFile.Read(dwOffset);
		if (pIndexInfo == NULL)
			continue;

		if(IsSameCode(pCode,&(pIndexInfo->m_CodeInfo)) || pIndexInfo->IsEmptyIndex())
		{	
			return dwOffset;
		}
	}
	return 0;
}

BOOL CKLineDataFile::GetCodePages(DWORD dwIndexOffset,CArray<PageInfo,PageInfo> *codePages)
{
	
	IndexInfo *pIndexInfo = NULL;//(IndexInfo*)m_mapViewFile.Read(dwIndexOffset);
	for(int i=0;i< m_head.m_nIndexCapability;i++)
	{
		codePages->Add(pIndexInfo->m_pages[i]);
	}
	
	return TRUE;
}


int CKLineDataFile::GetOneIndexSize()
{
	return sizeof(CodeInfo) + m_head.m_nIndexCapability * sizeof(PageInfo);
}

int CKLineDataFile::GetOnePageSize()
{
	//sizeof(页信息) + N * sizeof(日线结构）
	return sizeof(PageInfo)  + m_head.m_nPageCapability * sizeof(StockCompDayDataEx);
}

BOOL CKLineDataFile::OpenIndexData( CString filename )
{
	//return m_mapViewFile.Open(filename,KLINEDATAFILE_EXPAND);
	CriticalGuard guard(&m_critical);

	try
	{
		if ( MakeFile(filename) )
		{
			if (m_pIndexFile == NULL)
			{
				m_pIndexFile   = new CFile;
			}
			if(	!m_pIndexFile->Open(filename,  ( CFile::modeCreate |CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate|CFile::typeBinary)))
			{
				delete m_pIndexFile;
				m_pIndexFile = NULL;
				return FALSE;
			}
		}	
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CKLineDataFile::GetFilePath(CodeInfo* pCode, int nPeroid, CString& indexPath, CString& dataPath )
{
	CString strFileName,strIndexFile;
	switch(nPeroid)
	{
	case PERIOD_TYPE_DAY: 
		{
			strIndexFile = "index_day.dat";
			strFileName = "day.dat";
			break;
		}	

	case PERIOD_TYPE_MINUTE1:
		{
			strIndexFile = "index_min1.dat";
			strFileName = "min1.dat";
			break;
		}

	case PERIOD_TYPE_MINUTE5:
		{
			strIndexFile = "index_min5.dat";
			strFileName = "min5.dat";
			break;
		}

	default:
		strFileName = "unknow.dat";
	}

	if(strFileName == "unknow.dat")
		return FALSE;

	//取文件路径
	CString strDayBasePath =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::DATA_PATH);
	CString strIndexPath , strDataPath;
	char strRet[_MAX_PATH];
	memset(strRet,0,sizeof(strRet));
	GetMarketName(pCode->m_cCodeType,strRet,sizeof(strRet));
	CString strMarketPath;
	strMarketPath.Format("%s%s",strDayBasePath, strRet );
	CreatePath(strMarketPath);
	indexPath.Format("%s%s\\%s", strDayBasePath, strRet, strIndexFile);
	dataPath.Format("%s%s\\%s", strDayBasePath, strRet, strFileName);
	return TRUE;
}

 void CKLineDataFile::RemoveMap()
{
	POSITION pos = m_klineFileMap.GetStartPosition();
	CString strKey;
	CKLineDataFile* pKlineFile;
	while(pos)
	{
		m_klineFileMap.GetNextAssoc(pos,strKey, (CObject*&)pKlineFile);
		if (pKlineFile && !IsBadReadPtr(pKlineFile,1))
		{
			delete  pKlineFile;
		}
	}
	pos = m_klineIndexFileMap.GetStartPosition();
	pKlineFile = NULL;
	while(pos)
	{
		m_klineIndexFileMap.GetNextAssoc(pos,strKey, (CObject*&)pKlineFile);
		if (pKlineFile && !IsBadReadPtr(pKlineFile,1))
		{
			delete  pKlineFile;
		}
	}
	m_klineFileMap.RemoveAll();
	m_klineIndexFileMap.RemoveAll();
}
