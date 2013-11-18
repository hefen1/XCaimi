// HSDownloadData.cpp : 实现文件
//

#include "stdafx.h"
#include "HSDownloadData.h"
#include "NetEngineInterface.h"
#include "WinnerApplication.h"
#include "hsds_comudata.h"
#include "Tools.h"
#include "GeneralHelper.h"
// CHSDownloadData 对话框

IMPLEMENT_DYNAMIC(CHSDownloadData, CDialog)

CTimeSpan GetTimeStep(short nPeriod)
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

CTime GetNowTime(short nPeriod)
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
CTime GetTimeFromIntTime(unsigned long lDate,short nPeriod)
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


unsigned long  GetIntTimeFromTime(const CTime &t,short nPeriod)
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
unsigned long GetLimitDate(CodeInfo *pCode,short nPeriod,int nDays)
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

long GetWorkDays(long lBeginDate,long lEndDate,BOOL bCountHeadTail=TRUE)
{
	long lDays = 0;
	if (lBeginDate <= 0 || lEndDate <= 0)
		return -1;

	CTimeSpan tOneDay(1,0,0,0);
	CTime tBeginDate = LongToDate(lBeginDate);
	CTime tEndDate = LongToDate(lEndDate);

	if (tBeginDate > tEndDate)
		return -1;

	if (!bCountHeadTail)
	{
		tBeginDate += tOneDay;
		tEndDate -= tOneDay;
	}

	if (tBeginDate > tEndDate)
		return - 1;

	while( tBeginDate <= tEndDate)
	{
		long lWeekend = tBeginDate.GetDayOfWeek();
		tBeginDate += tOneDay;
		if (lWeekend == 1 || lWeekend == 7)
		{
			continue;
		}
		lDays++;		
	};

	return lDays;
}
int GetRealMinites(int nValue,int nSubValue)
{
	int nMinite = 0;

	if (nValue > 100)
	{
		nMinite += (nValue/100) * 60 ;
	}

	if (nSubValue%100 == 0)
	{
		if (nValue == 1)
			return nValue;
		else
			nMinite += (nValue%100) - 40 > 0 ? (nValue%100) - 40 : 0;
	}
	else
	{
		nMinite += nValue%100 > 60 ?  (nValue%100) - 40 : (nValue%100);
	}

	return nMinite;
}
//获取在时间lBeginDate到lEndDate之间正常的K线根数
long GetDayDataCount(CodeInfo *pCode,long lBeginDate,long lEndDate,int nPeriod)
{
	if (lBeginDate > lEndDate)
	{
		return 0;
	}

	short nCountPerMin = 60,nIndex = 1;
	short nOneDayNums = 0;
	short nGZIndex = MakeGZIndex(pCode->m_cCodeType);
	short nMainType  = MakeMarket(pCode->m_cCodeType);
	int nMorningBegin,nMorningEnd;
	int nAfterBegin,nAfterEnd;
	if (nMainType == STOCK_MARKET)
	{
		nMorningBegin = 930;
		nMorningEnd = 1130;
		nAfterBegin = 1300;
		nAfterEnd = 1500;
		nOneDayNums = 240;
	}
	if (nMainType == FUTURES_MARKET)
	{
		if (nGZIndex)
		{
			nMorningBegin = 915;
			nMorningEnd = 1130;
			nAfterBegin = 1300;
			nAfterEnd = 1515;
			nOneDayNums = 270;
		}
		else
		{
			nMorningBegin = 900;
			nMorningEnd = 1130;
			nAfterBegin = 1330;
			nAfterEnd = 1500;
			nOneDayNums = 240;
		}
	}

	if (nMainType == HK_MARKET)
	{
		nMorningBegin = 1000;
		nMorningEnd = 1230;
		nAfterBegin = 1430;
		nAfterEnd = 1600;
		nOneDayNums = 240;
	}
	if (nPeriod == PERIOD_TYPE_MINUTE5)
	{
		nIndex = 5;
		nCountPerMin = 12;
	}
	switch(nPeriod)
	{
	case PERIOD_TYPE_DAY:
		return GetWorkDays(lBeginDate,lEndDate);
		break;
	case PERIOD_TYPE_MINUTE1:
	case PERIOD_TYPE_MINUTE5:
		{
			int nCount =0,nGapTime = -1,nRealMinte = 0;
			int nHeadMinite = lBeginDate%10000;
			int nTailMinite = lEndDate%10000;	
			CTime Headtm = GetTimeFromIntTime(lBeginDate,nPeriod);
			CTime Tailtm = GetTimeFromIntTime(lEndDate,nPeriod);
			int nHeadTime = Headtm.GetYear() * 10000 + Headtm.GetMonth() * 100 + Headtm.GetDay();
			int nTailTime = Tailtm.GetYear() * 10000 + Tailtm.GetMonth() * 100 + Tailtm.GetDay();
			int nWorkDays = GetWorkDays(nHeadTime,nTailTime);
			if (nWorkDays < 1)
			{
				return -1;
			}

			if (nTailMinite > nAfterEnd)
			{
				nTailMinite = nAfterEnd;
			}
			if (nHeadMinite < nMorningBegin)
			{
				nHeadMinite = nMorningBegin;
			}

			if (nWorkDays == 1)
			{			
				if (nTailMinite < nHeadMinite)
				{
					return 0;
				}
				nGapTime = nTailMinite - nHeadMinite + 1;
				if (nHeadMinite < nMorningEnd)
				{
					if (nTailMinite < nAfterBegin )
						nGapTime -= nTailMinite - nMorningEnd > 0 ? nTailMinite - nMorningEnd : 0;
					else
						nGapTime -= nAfterBegin - nMorningEnd > 0 ? nAfterBegin - nMorningEnd : 0;
				}
				else
				{
					if (nHeadMinite < nAfterBegin)
						nGapTime -= nAfterBegin - nHeadMinite > 0 ? nAfterBegin - nHeadMinite: 0;
				}

				nRealMinte = GetRealMinites(nGapTime,nTailMinite);
				nCount =  nRealMinte > 0 ? nRealMinte/nIndex : 0;
			}
			else
			{
				nGapTime = nAfterEnd - nHeadMinite + 1;
				if (nHeadMinite < nAfterBegin)
				{
					if (nHeadMinite < nMorningEnd)
						nGapTime -= nAfterBegin - nMorningEnd >0 ? nAfterBegin - nMorningEnd : 0;
					else
						nGapTime -= nAfterBegin - nHeadMinite> 0 ? nAfterBegin - nHeadMinite : 0;					
				}
				nRealMinte = GetRealMinites(nGapTime,nAfterEnd);
				long nTempCount1 = nRealMinte > 0 ? nRealMinte/nIndex : 0;
				nGapTime = nTailMinite - nMorningBegin ;
				if (nTailMinite >= nAfterBegin)
				{
					nGapTime -= nAfterBegin - nMorningEnd > 0 ? nAfterBegin - nMorningEnd : 0;
				}
				else
				{
					if (nTailMinite > nMorningEnd)
					{
						nGapTime -= nTailMinite - nMorningEnd > 0 ? nTailMinite - nMorningEnd :0;
					}
				}
				nRealMinte = GetRealMinites(nGapTime,nTailMinite);
				long nTempCount2 = nRealMinte > 0 ? nRealMinte/nIndex : 0;
				nCount = nTempCount1 + nTempCount2;

				nWorkDays += nCount/nOneDayNums;
				nCount = nCount%nOneDayNums;
				int nGapDays = nWorkDays - 2;
				if (nHeadMinite > nAfterEnd )
				{
					nGapDays --;
				}
				nGapDays = nGapDays >0 ? nGapDays : 0;
				nCount += nGapDays*(nOneDayNums/nIndex);
			}
			return nCount;
		}
		break;
	default:
		break;
	}

	return 0;
}

CDownloadThread* CDownloadThread::m_currentDownloadThread = NULL;
CWnd* CDownloadThread::m_pMain = NULL;


IMPLEMENT_DYNCREATE(CDownloadThread, CWinThread)


CDownloadThread::CDownloadThread()
{
}

CDownloadThread::~CDownloadThread()
{
}

BOOL CDownloadThread::InitInstance()
{
	CHSDownloadData* pCDataDownload = new CHSDownloadData(NULL);
	pCDataDownload->Create(CHSDownloadData::IDD,NULL);
	pCDataDownload->ShowWindow(SW_SHOW);
	m_pMainWnd = pCDataDownload;
	return TRUE;
}

int CDownloadThread::ExitInstance()
{
	m_currentDownloadThread = NULL;
	return CWinThread::ExitInstance();
}


CHSDownloadData::CHSDownloadData(CWnd* pParent /*=NULL*/)
	: CDialog(CHSDownloadData::IDD, pParent)
{
	m_nDataSourceID = 0;
	m_nLimitedDay = 0;
	m_nReloadDegree = 0;
	m_lHandle = 0;
	m_nPos = 0;
	m_pManager = NULL;
	m_pDataSource = NULL;
	m_pTaskItem = NULL;
	m_ayStock.RemoveAll();
}

CHSDownloadData::~CHSDownloadData()
{
}

void CHSDownloadData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_LIST2, m_lcStock);
	DDX_Control(pDX, IDC_TREE_DATATYPE, m_tcDataType);
}


BEGIN_MESSAGE_MAP(CHSDownloadData, CDialog)
END_MESSAGE_MAP()



BOOL CHSDownloadData::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_pManager = (IChannelManager*)CWinnerApplication::GetObject(OBJ_CHANNELMANAGER);
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	if (m_pDataSource && !IsBadReadPtr(m_pDataSource,1))
	{
		m_nDataSourceID = m_pDataSource->HSDataSource_DataSouceInit(this,NULL);
	}
	//创建行情连接
	m_lHandle = m_pManager->CreateConnector(CEV_Connect_HQ,"");
	m_nTaskMask = 0xFFFF;
	m_nCulType = 1;
	InitListView();
	DownLoadData();
	return TRUE;
}

void CHSDownloadData::SetTaskMask(int nType)
{
	
		switch(nType)
		{
		case 1:
			m_nTaskMask = m_nTaskMask & 0xFFFE; //1110 = 8 + 4 + 2 + 0 = 14 E 去掉K线下载
			break;
		case 2:
			m_nTaskMask = m_nTaskMask & 0xFFFD; //1101 = 8 + 4 + 0 + 1 = 13 D 去掉5分钟
			break;
		case 3: 
			m_nTaskMask = m_nTaskMask & 0xFFFB; //1011 = 8 + 0 + 2 + 1 = 11 B 去掉分时
			break;
		}
	
}
BOOL CHSDownloadData::DownLoadData()
{
	CriticalGuard guard(&m_critical);
	long lLen;
	StockUserInfo* pStock = NULL;
	short nPeriod;
	SetTaskMask(m_nCulType);
	m_nCulType ++;
	if ( m_nCulType >= 4)
		m_nCulType = 1;
	GetNextCode(&pStock, nPeriod);
	char* pBuffer = MakeReqPacket(&pStock->m_ciStockCode,nPeriod,lLen);

	if(pBuffer == NULL)
	{
		DownLoadData();
	}
	else
	{
		HSMarketDataType market = pStock->m_ciStockCode.m_cCodeType & 0xF000;
		char cServerType = CEV_Connect_HQ;

		if(market == FUTURES_MARKET)
		{
			//cServerType = CEV_Connect_QH;  //期货
		}

		if (market == HK_MARKET)
		{
			//cServerType = CEV_Connect_GG;  //港股
		}
		if ( m_pDataSource && !IsBadReadPtr(m_pDataSource,1))
			m_pDataSource->HSDataSource_RequestAsyncData(m_nDataSourceID,pBuffer,lLen,-1,e_DataSouceSend_Normal|e_DataSourceSend_HQDataDownLoad,
			                     m_lHandle);
		TRACE2("\r\n send packet code:%s, nPeriod:%d \r\n", pStock->GetName(),nPeriod);
	}
	return TRUE;	
}

BOOL CHSDownloadData::HSDataSourceSink_OnCommNotify( void* pData )
{
	return TRUE;
}

BOOL CHSDownloadData::HSDataSourceSink_OnRecvData( void* pData, int nLen )
{
	if (!pData || IsBadReadPtr(pData,1))
	{
		return FALSE;
	}
	DataHead* pHead = (DataHead*)pData;
	if(pHead->m_nType == RT_TECHDATA_RANGE)
	{
		CriticalGuard guard(&m_critical);
		DealTechDataAns(pHead);	
		DownLoadData();
		TRACE2("\r\n recv packet code:%s, nPeriod:%d\r\n", pHead->m_nPrivateKey.m_pCode.m_cCode, pHead->m_lKey);
	}
	return TRUE;
}

char* CHSDownloadData::MakeReqPacket( CodeInfo* pCode, short nPeriod , long &lReqLen)
{
	if ( pCode == NULL || IsBadReadPtr(pCode,1))
		return NULL;
	BOOL bChangeRange = FALSE;
	BOOL bDownLoaded = FALSE;
	CString str;
	str.Format("%d-%s",pCode->m_cCodeType,pCode->GetCode());
/*	TaskItem* m_pTaskItem = NULL;*/
	if( m_pTaskItem && !IsBadReadPtr(m_pTaskItem,1) && m_pTaskItem->m_TaskName.Compare(str) )
	{
		m_pTaskItem->SetDayTaskFinished();
		m_pTaskItem->SetMinute1TaskFinished();
		m_pTaskItem->SetMinute5TaskFinished();
		return NULL;
	}

	m_DownLoadCount = 0;
	int nAfterBegin = 1300,nAfterEnd = 1500;
	short nGZIndex = MakeGZIndex(pCode->m_cCodeType);
	short nQihuo = MakeMarket(pCode->m_cCodeType);
	if (nQihuo == FUTURES_MARKET)
	{
		if (nGZIndex)
		{
			nAfterEnd = 1515;
		}
		else
		{
			nAfterBegin = 1330;
		}
	}
	//取本地缓存中已有的数据
	GetLocalData(pCode,nPeriod,m_pTaskItem);

	CArray<DateRange,DateRange> ayDate;
	CTime t1 = GetNowTime(nPeriod);
	CTimeSpan tSpan = GetTimeStep(nPeriod);
	unsigned long nLimitTime = GetLimitDate(pCode,nPeriod,m_nLimitedDay);//修正2012时间问题
	unsigned long nEarliestTime = GetIntTimeFromTime(t1,nPeriod);//修正2012时间问题
	unsigned long nTodayTime = nEarliestTime;//修正2012时间问题

	long lCurDate = 10;	
	
	CTime tmNow = CTime::GetCurrentTime();
	lCurDate = tmNow.GetYear() * 10000 + tmNow.GetMonth() * 100 + tmNow.GetDay();
	
	bDownLoaded = IsDownLoaded(lCurDate,pCode,nPeriod);

	try
	{

		BOOL bExistLastData = FALSE;
		BOOL bFirstDayRange = TRUE;
		CString strCode = pCode->GetCode();
		StockDay* pStockDay = (StockDay*)m_pTaskItem->m_localData;
		for(int i =  m_pTaskItem->m_localDataCount - 1; i >= 0; i--)
		{
			if (bDownLoaded)
			{
				break;
			}

			if(pStockDay[i].m_lDate <= 0)
				continue;

			if(pStockDay[i].m_lDate < nEarliestTime)
				nEarliestTime = pStockDay[i].m_lDate;

			if (pStockDay[i].m_lDate == 20000000) //修正错误的日期时间
			{
				pStockDay[i].m_lDate = 19991231;
			}

			///////////////若本地一存在的数据日期在限制时间内 则挑过数据的缺失检查/////////////////////////////
			if (i == m_pTaskItem->m_localDataCount - 1)
			{
				CTime tmNow = t1;
				CTime tmLastData = GetTimeFromIntTime(pStockDay[i].m_lDate,nPeriod);
				int nNowYear = tmNow.GetYear();
				int nNowMonth = tmNow.GetMonth();
				int nNowDay = tmNow.GetDay();
				if ( nNowYear == tmLastData.GetYear() && nNowMonth == tmLastData.GetMonth() && tmLastData.GetDay() == nNowDay)
				{
					CTime tmlimit = GetTimeFromIntTime(nLimitTime,nPeriod);
					CTime tmFirstDate = GetTimeFromIntTime(pStockDay[0].m_lDate,nPeriod);
					if (m_nLimitedDay == 0)
					{
						if ( PERIOD_TYPE_DAY != nPeriod)
						{
							int nMaxDays = 0;
							if (nGZIndex)
								nMaxDays = 36 - 1;  //36 = (10000/240 * 240)/270
							else
								nMaxDays = 41 - 1; //41= 10000/240

							long lLastDate = tmLastData.GetYear() * 10000 + tmLastData.GetMonth() * 100 + tmLastData.GetDay();
						
							if (lLastDate != lCurDate)
							{
								nMaxDays --;
							}

							CTimeSpan tmSpan(1,0,0,0);
							int nCount = 0;
							while(nCount < nMaxDays)
							{
								tmLastData -= tmSpan;
								if (tmLastData.GetDayOfWeek() != 1&& tmLastData.GetDayOfWeek() != 7)
								{
									nCount ++;
								}
							}				
						}
						if (PERIOD_TYPE_DAY == nPeriod)
						{
							if (tmFirstDate.GetYear() <= tmlimit.GetYear() && tmFirstDate.GetMonth() <= tmlimit.GetMonth() 
								&& tmFirstDate.GetDay() <= tmlimit.GetDay())
							{
								bExistLastData = TRUE;
							}
						}
						else if (tmFirstDate.GetYear() <= tmLastData.GetYear() && tmFirstDate.GetMonth() <= tmLastData.GetMonth() 
							&& tmFirstDate.GetDay() <= tmLastData.GetDay())
						{
							bExistLastData = TRUE;
						}
					}
					else
					{
						if (tmFirstDate.GetYear() <= tmlimit.GetYear() && tmFirstDate.GetMonth() <= tmlimit.GetMonth() 
							&& tmFirstDate.GetDay() <= tmlimit.GetDay())
						{
							bExistLastData = TRUE;
						}
					}
					if (bExistLastData)
					{
						break;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////

			int nCount = 0;	
			long lEndDate = 0;
			long lBeginDate = 0;

			CTime t2 = GetTimeFromIntTime(pStockDay[i].m_lDate,nPeriod);
			CTime t = t2;
			if (i != m_pTaskItem->m_localDataCount - 1)
			{
				t = t + tSpan;
			}
			while(t < t1)
			{
				if(t.GetDayOfWeek() != 1 &&  t.GetDayOfWeek() != 7 && t < t1) //1 = Sunday, 2 = Monday, ..., 7 = Saturday
				{
					if(lBeginDate == 0)
						lBeginDate = GetIntTimeFromTime(t,nPeriod);

					if (!bFirstDayRange)
					{
						lEndDate = GetIntTimeFromTime(t1 - tSpan,nPeriod);
					}
					else
					{
						while(t1.GetDayOfWeek()==1 || t1.GetDayOfWeek() == 7)
						{
							CTimeSpan OneDay(1,0,0,0);
							t1 -= OneDay;
						}
						lEndDate = GetIntTimeFromTime(t1,nPeriod);
					}
					nCount ++;
					break;
				}
				t = t + tSpan;
			};

			t1 = t2;
			bFirstDayRange = FALSE;

			if(nCount > 0 && lBeginDate >= nLimitTime )
			{
				if( lEndDate < nLimitTime)
					lEndDate = nLimitTime;

				CTime tmBegin = GetTimeFromIntTime(lBeginDate,nPeriod);
				CTime tmEnd = GetTimeFromIntTime(lEndDate,nPeriod);
				if (tmBegin.GetYear() == tmEnd.GetYear() && tmBegin.GetMonth() == tmEnd.GetMonth() && tmBegin.GetDay() == tmEnd.GetDay())
				{
					if (tmBegin.GetHour()*100 + tmBegin.GetMinute() > 1130 && tmEnd.GetHour()*100 + tmEnd.GetMinute() < nAfterBegin)
					{
						continue;
					}
				}

				if (lBeginDate > lEndDate)
				{
					continue ;
				}

				DateRange dr;
				dr.m_nBeginDate = lBeginDate;
				dr.m_nEndDate = lEndDate;
				ayDate.Add(dr);
			}

			if (nCount == 0 && pStockDay[i].m_lDate <= nLimitTime)
			{
				break;
			}
		}

		//下载分钟线数据时按9840的倍数根往前下载，若没有最近41天的数据，则下载最近82天数据，依次类推，最多倒退41*18天
		int nLimitedDays = 0,nPermitCount = 9840;
		if (ayDate.GetCount() < 1)
		{
			if(nEarliestTime > nLimitTime && !bExistLastData) 			//加入最后一段时间
			{
				DateRange dr;
				dr.m_nBeginDate = nLimitTime;
				dr.m_nEndDate = nEarliestTime;
				ayDate.Add(dr);
			}
		}
		else
		{
			int nMaxDays = 0;
			DateRange LastDateRage = ayDate.GetAt(ayDate.GetUpperBound());
			long nPreDate = GetPreDataDate(pCode,LastDateRage.m_nBeginDate,nPeriod);
			if (nGZIndex)
				nMaxDays = 36 - 1;  //36 = (10000/240 * 240)/270
			else
				nMaxDays = 41 - 1;  //41= 10000/240

			long nEarlyLimitedDate = nLimitTime;
			if (PERIOD_TYPE_DAY != nPeriod)
			{
				nEarlyLimitedDate = GetPreDate(pStockDay[m_pTaskItem->m_localDataCount -1].m_lDate,nMaxDays,nPeriod);
				long nCheckDataCount = GetDayDataCount(pCode,nEarlyLimitedDate,nTodayTime,nPeriod);
				if (nCheckDataCount > nPermitCount)
				{
					int nSurplusCount = nCheckDataCount - nPermitCount;
					nEarlyLimitedDate = GetChangedBeginDate(pCode,nEarlyLimitedDate,nSurplusCount,nPeriod);
				}
			}
			if (nPreDate >= nEarlyLimitedDate)
			{
				DateRange dr;
				dr.m_nBeginDate = nEarlyLimitedDate > nLimitTime ? nEarlyLimitedDate : nLimitTime;
				dr.m_nEndDate = nPreDate;
				ayDate.Add(dr);
			}
		}

		CArray<DateRange,DateRange> ayDateChange;
		int nDateCount = ayDate.GetCount();
		nPermitCount = nPermitCount*(m_nReloadDegree + 1);
		for (int i = nDateCount - 1; i>= 0; i--)
		{
			DateRange dateRange = ayDate.GetAt(i);
			long nRangeCount = GetDayDataCount(pCode,dateRange.m_nBeginDate,dateRange.m_nEndDate,nPeriod);
			nLimitedDays += nRangeCount;
			if (nLimitedDays < nPermitCount)
			{
				ayDateChange.Add(dateRange);
			}
			else
			{
				int nSpillCount = nLimitedDays - nPermitCount;
				dateRange.m_nBeginDate = GetChangedBeginDate(pCode,dateRange.m_nBeginDate,nSpillCount,nPeriod);
				ayDateChange.Add(dateRange);
				bChangeRange = TRUE;
				break;
			}
		}
		if (bChangeRange)
		{
			nDateCount = ayDateChange.GetCount();
		}

		if (bDownLoaded)
		{
			bChangeRange = TRUE;
			ayDateChange.RemoveAll();
			long nOutRangeDateB = 20101001; //国庆放假无交易行情
			long nOutRangeDateE = 20101001;
			if (PERIOD_TYPE_DAY != nPeriod)
			{
				nOutRangeDateB = 2010010930;
				nOutRangeDateE = 2010010930;
			}		
			DateRange dr;
			dr.m_nBeginDate = nOutRangeDateB;
			dr.m_nEndDate = nOutRangeDateE;
			nDateCount = ayDateChange.GetCount();
		}

		short nCount = sizeof(ReqDayData) / sizeof(CodeInfo);
		if( sizeof(ReqDayData) % sizeof(CodeInfo) )
			nCount++;
		int lLen = sizeof(AskData) + sizeof(CodeInfo) * (nCount - 1) + nDateCount * sizeof(DateRange);
		lReqLen = lLen;
		AskData* ask = (AskData*)(new char[lLen]);
		memset(ask,0,lLen);

		ask->m_nType  = RT_TECHDATA_RANGE;
		ask->m_nSize  = nCount;
		ask->m_lKey = nPeriod;
		memcpy(&ask->m_nPrivateKey, pCode, sizeof(CodeInfo));

		ReqRangeDayData* pRequest = (ReqRangeDayData*)ask->m_pCode;
		pRequest->m_lBeginPosition = 0;
		pRequest->m_nDay = 0;
		pRequest->m_cPeriod = nPeriod;
		memcpy(&pRequest->m_ciCode, pCode, sizeof(CodeInfo));
		pRequest->m_nDataRangCount = nDateCount;
		DateRange *pRane = pRequest->m_dataRange;

		CString strRequest,strRang;
		for(int nIndex = 0; nIndex < nDateCount; nIndex++)
		{
			if (bChangeRange)
				pRane[nIndex] = ayDateChange.GetAt(nIndex);
			else
				pRane[nIndex] = ayDate.GetAt(nIndex);

			m_DownLoadCount += GetDayDataCount(pCode,pRane[nIndex].m_nBeginDate,pRane[nIndex].m_nEndDate,nPeriod);
			strRang.Format("%d-%d;",pRane[nIndex].m_nBeginDate,pRane[nIndex].m_nEndDate);
			strRequest += strRang;
		}
		return (char*)ask;

	}
	catch (...)
	{
	}
	return NULL;
}

BOOL CHSDownloadData::GetNextCode( StockUserInfo **pStock,short &nPeriod )
{
	nPeriod = 0;
	*pStock = NULL;

	do
	{
		if(m_nPos >= m_lcStock.GetItemCount())
		{
			OnCancel();
			return FALSE;
		}

		StockUserInfo* pStockInfo = (StockUserInfo*)m_lcStock.GetItemData(m_nPos);
		*pStock = pStockInfo;
		TRACE1("\r\n pos is %d", m_nPos);
		CString str;
		str.Format("%d-%s",pStockInfo->m_ciStockCode.m_cCodeType,pStockInfo->m_ciStockCode.GetCode());
		if (m_pTaskItem == NULL || IsBadReadPtr(m_pTaskItem,1))
		{			
			m_pTaskItem = new TaskItem;
			m_pTaskItem->m_TaskName.Format("%s", str);
			m_pTaskItem->m_lBeignTime = GetTickCount();
			m_pTaskItem->m_nTaskMask = m_nTaskMask;
			
		}
		else if ( m_pTaskItem->m_TaskName.Compare(str))
		{
			m_pTaskItem->m_TaskName.Format("%s", str);
			m_pTaskItem->m_lBeignTime = GetTickCount();
			m_pTaskItem->m_nTaskMask = m_nTaskMask;
		}

		if(m_pTaskItem->IsFinishedDAYTask() && m_pTaskItem->IsFinishedMinute5Task() && m_pTaskItem->IsFinishedMinute1Task())
		{
			nPeriod = 0;
			m_nPos++;
			m_progress.SetPos(m_nPos);
			continue;
		}

		CString strText;
		strText.Format("正在下载:%s",pStockInfo->GetName());
		if(!m_pTaskItem->IsFinishedDAYTask())
		{
			strText +=_T("--日线数据");
			nPeriod = PERIOD_TYPE_DAY;
		
		}
		else if(!m_pTaskItem->IsFinishedMinute5Task())
		{
			strText +=_T("--5分钟线数据");
			nPeriod = PERIOD_TYPE_MINUTE5;
		
		}
		else if(!m_pTaskItem->IsFinishedMinute1Task())
		{
			strText +=_T("--1分钟线数据");
			nPeriod = PERIOD_TYPE_MINUTE1;
			
		}

	}while(nPeriod == 0);

	if(*pStock != NULL && nPeriod != 0 )
		return TRUE;
	else
		return FALSE;
}

void CHSDownloadData::DealTechDataAns( DataHead *pHead )
{
	//for test
// 
// 	AnsDayDataEx* pDayData = (AnsDayDataEx*)pHead;
// 	StockDay* pKData = pDayData->m_sdData;
// 	int nSize  = pDayData->m_nSize;
// 	CodeInfo *pCode  = &pDayData->m_dhHead.m_nPrivateKey.m_pCode;
// 
// 	CString strCode;
// 	strCode.Format("%d-%s",pCode->m_cCodeType,pCode->GetCode());
// 	if(m_pTaskItem == NULL || IsBadReadPtr(m_pTaskItem,1) || ( m_pTaskItem && m_pTaskItem->m_TaskName.Compare(strCode)))
// 	{
// 		return;
// 	}
// 	if (m_pTaskItem  && !m_pTaskItem->m_TaskName.Compare(strCode))  
// 	{
// 		switch (pHead->m_lKey)
// 		{
// 		case PERIOD_TYPE_DAY:
// 			m_pTaskItem->SetDayTaskFinished();
// 			m_pTaskItem->m_DayCount = nSize <= 9840 ? nSize : 9840;
// 			break;
// 		case PERIOD_TYPE_MINUTE5:
// 			m_pTaskItem->SetMinute5TaskFinished();
// 			m_pTaskItem->m_Min5Count = nSize <= 9840 ? nSize : 9840;
// 			break;
// 		case PERIOD_TYPE_MINUTE1:
// 			m_pTaskItem->SetMinute1TaskFinished();
// 			m_pTaskItem->m_Min1Count = nSize <= 9840 ? nSize : 9840;
// 			break;
// 		}
// 	}
// 	unsigned long lLastDate = 0;
// 	BOOL bSetZero = FALSE;
// 	int nTotalSize = nSize + m_pTaskItem->m_localDataCount;
// 	if(nSize > 0)
// 	{	
// 		StockDay* pLocalData = NULL;
// 		if(m_pTaskItem->m_localDataCount > 0)
// 		{
// 			pLocalData = (StockDay*)m_pTaskItem->m_localData;
// 		}
// 
// 		HGLOBAL StockDayNew = GlobalAllocPtr(GHND, sizeof(StockDay) * nTotalSize);
// 		memset(StockDayNew, 0, sizeof(StockDay)*nTotalSize);
// 		StockDay* pNewData = (StockDay*)StockDayNew;
// 
// 		CString strCode = pCode->GetCode();
// 		//新数据
// 		for(int i = 0; i < nSize; i++ )
// 		{
// 			memcpy(&pNewData[i], &pKData[i], sizeof(StockDay));	
// 			pNewData[i].m_lMoney = pKData[i].m_lMoney;
// 			pNewData[i].m_lTotal = pKData[i].m_lTotal;		
// 		}
// 
// 		//旧数据
// 		if(m_pTaskItem->m_localDataCount > 0)
// 		{
// 			for(int j = 0; j < nTotalSize - nSize;j++ )
// 				memcpy(&pNewData[j + nSize], &pLocalData[j], sizeof(StockDay));
// 		}
// 
// 
// 		// 排序
// 		KQuickSort qsData;
// 		qsData.m_pData      = pNewData;
// 		qsData.m_nDataLen   = sizeof(StockDay);
// 		qsData.m_nKeyOffset = 0;
// 		qsData.m_nKeyLen    = QS_UNLONG;
// 		qsData.m_nLow       = 0;
// 		qsData.m_nHigh      = (short)( nTotalSize- 1); //将nSize替换成nTotalSize保证数据排序范围包含所有数据以对全部数据正确排序
// 
// 		QuickSortAscend2(&qsData);	
// 
// 		//计算真实个数
// 		StockDay *ppTemp = pNewData;
// 		unsigned long lPreDay = 0; //修正2012时间问题
// 		int nPeriod = pHead->m_lKey;
// 		int nDayCount = 0;
// 		for(int i = 0; i < nTotalSize; i++, ppTemp++ )
// 		{
// 			if( lPreDay == ppTemp->m_lDate)
// 			{
// 				continue;			
// 			}
// 			nDayCount++;
// 			lPreDay = ppTemp->m_lDate;
// 		}
// 
// 		StockDay * pTotalDay = pNewData;
// 		if( nDayCount > 0 )
// 		{
// 			HGLOBAL stockday = GlobalAllocPtr(GHND, sizeof(StockDay) * nDayCount);
// 			StockDay * pStock = (StockDay*)stockday;
// 			if( pStock == NULL )
// 				return;
// 			ppTemp = pStock;
// 			for(int i = 0; i < nTotalSize; i++)
// 			{
// 				//过滤非法数据
// 				//---支持基差、价差等负数价格
// 				if ( pTotalDay[i].m_lDate	   <= 0 || pTotalDay[i].m_lOpenPrice  <= INVALIDE_VALUE || pTotalDay[i].m_lMaxPrice   <= INVALIDE_VALUE ||
// 					pTotalDay[i].m_lMinPrice   <= INVALIDE_VALUE || pTotalDay[i].m_lClosePrice <= INVALIDE_VALUE )
// 					continue;
// 
// 				//过虑重复
// 				if(i > 0)
// 				{
// 					if(pTotalDay[i-1].m_lDate == pTotalDay[i].m_lDate)
// 						continue;
// 				}
// 
// 				memcpy(ppTemp,&pTotalDay[i],sizeof(StockDay));
// 				ppTemp++;
// 				lLastDate = pTotalDay[i].m_lDate;
// 			}
// 			//写文件	
// 			if( m_pDataSource)
// 				m_pDataSource->HSDataSource_WriteDayData(pCode, pHead->m_lKey,pStock,nDayCount);
// 			
// 			GlobalFree(stockday);
// 		}
// 
// 		GlobalFree(StockDayNew);
// 	}
// 	else
// 	{
// 		switch (pHead->m_lKey)
// 		{
// 		case PERIOD_TYPE_DAY:
// 			if (m_pTaskItem->m_DayCount == 0 && m_pTaskItem->m_localDataCount > 0)
// 				bSetZero = TRUE;
// 			break;
// 		case PERIOD_TYPE_MINUTE5:
// 			if (m_pTaskItem->m_Min5Count == 0 && m_pTaskItem->m_localDataCount > 0)
// 				bSetZero = TRUE;
// 			break;
// 		case PERIOD_TYPE_MINUTE1:
// 			if (m_pTaskItem->m_Min1Count == 0 && m_pTaskItem->m_localDataCount > 0)
// 				bSetZero = TRUE;
// 			break;
// 		}
// 	}
// 	if (bSetZero)
// 	{
// 		switch (pHead->m_lKey)
// 		{
// 		case PERIOD_TYPE_DAY:
// 			m_pTaskItem->m_DayCount = 0;
// 			break;
// 		case PERIOD_TYPE_MINUTE5:
// 			m_pTaskItem->m_Min5Count = 0;
// 			break;
// 		case PERIOD_TYPE_MINUTE1:
// 			m_pTaskItem->m_Min1Count = 0;
// 			break;
// 		}
// 	}
// 	BOOL b;
// 	UpdateStockListView(pCode,m_pTaskItem,pHead->m_lKey,bSetZero,b);	
}

long CHSDownloadData::GetPreDataDate( CodeInfo *pCode,long nCurrentDate,int nPeriod )
{
	CTimeSpan oneDay(1,0,0,0);
	CTimeSpan tSpan = GetTimeStep(nPeriod);
	CTime tm =  GetTimeFromIntTime(nCurrentDate,nPeriod);

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

	switch(nPeriod)
	{
	case PERIOD_TYPE_DAY:
		tm -= tSpan;
		while(tm.GetDayOfWeek() == 1 || tm.GetDayOfWeek() ==7)
		{
			tm -= tSpan;
		}
		return GetIntTimeFromTime(tm,nPeriod);

	case PERIOD_TYPE_MINUTE1:
	case PERIOD_TYPE_MINUTE5:
		{
			int nMinute = nCurrentDate%10000;
			if (nPeriod == PERIOD_TYPE_MINUTE1)
			{
				if (nMinute != nBeing1min)
				{
					tm -= tSpan;
					return GetIntTimeFromTime(tm,nPeriod);
				}
			}
			if (nPeriod == PERIOD_TYPE_MINUTE5)
			{
				if (nMinute != nBegin5min)
				{
					tm -= tSpan;
					return GetIntTimeFromTime(tm,nPeriod);
				}
			}

			tm -= oneDay;
			while(tm.GetDayOfWeek() == 1 || tm.GetDayOfWeek() == 7)
			{
				tm -= oneDay;
			}
			long lPreDate = (tm.GetYear() - 1990)* 100000000 + tm.GetMonth() * 1000000 + tm.GetDay() * 10000;
			if (nGZIndex)
			{
				lPreDate += 1515;
			}
			else
			{
				lPreDate += 1500;
			}

			return lPreDate;
		}
	default:
		return nCurrentDate;
	}
}

long CHSDownloadData::GetChangedBeginDate( CodeInfo *pCode,long lBeginDate,int nSpillDays,int nPeriod )
{
	if (nSpillDays <= 0)
	{
		return lBeginDate;
	}

	short nOneDayNums  = 240;
	short nGZIndex = MakeGZIndex(pCode->m_cCodeType);
	short nMainType  = MakeMarket(pCode->m_cCodeType);

	if (nGZIndex)
		nOneDayNums = 270;
	else
		nOneDayNums = 240;

	int nCount = 0,nGapDays =0, nYushu =0;
	CTimeSpan oneDay(1,0,0,0);
	CTime tmbg = GetTimeFromIntTime(lBeginDate,nPeriod);
	long lBeginMinite = lBeginDate%10000;
	switch(nPeriod)
	{
	case PERIOD_TYPE_DAY:
		while(nCount < nSpillDays)
		{
			tmbg += oneDay;
			int nWeekEnd = tmbg.GetDayOfWeek();
			if (nWeekEnd != 1 && nWeekEnd != 7)
			{
				nCount++;
			}
		}
		return GetIntTimeFromTime(tmbg,nPeriod);
	case PERIOD_TYPE_MINUTE1:
	case PERIOD_TYPE_MINUTE5:
		{
			int nIndex = 1,nQiHuoMorningBegin = 900,nQZQihuoMorningBegin = 915,nStockMorningBegin = 930;
			if (nPeriod == PERIOD_TYPE_MINUTE5)
			{
				nIndex = 5;
				nQiHuoMorningBegin = 905;
				nQZQihuoMorningBegin = 920;
				nStockMorningBegin = 935;
			}
			nGapDays = nSpillDays/(nOneDayNums/nIndex);
			nYushu = nSpillDays%(nOneDayNums/nIndex);
			while(nCount < nGapDays)
			{
				tmbg += oneDay;
				int nWeekEnd = tmbg.GetDayOfWeek();
				if (nWeekEnd != 1 && nWeekEnd != 7)
				{
					nCount++;
				}
			}

			CTime tmMoriningEnd;
			CTime tmNewTime;
			long lTmp = 0;
			CTimeSpan  tmTempSpan;
			CTimeSpan min1(0,(nYushu*nIndex)/60,(nYushu*nIndex)%60,0);
			CTime tmTemp = tmbg;
			tmTemp += min1;
			long lNewMini = GetIntTimeFromTime(tmTemp,nPeriod);
			tmNewTime = tmTemp;
			if (lBeginMinite < 1130)
			{	
				long ltailMini = lNewMini%10000;
				if (ltailMini <1130 )
				{
					return  lNewMini;
				}
				else
				{
					if (nMainType == STOCK_MARKET)
					{
						lTmp = (lNewMini/10000) * 10000 + 1300;
					}
					else
					{
						if (!nGZIndex)
							lTmp = (lNewMini/10000) * 10000 + 1330;
						else
							lTmp = (lNewMini/10000) * 10000 + 1300;
					}
					tmNewTime = GetTimeFromIntTime(lTmp,nPeriod);
					tmMoriningEnd = GetTimeFromIntTime((lNewMini/10000) * 10000 + 1130,nPeriod);
					tmTempSpan = tmTemp - tmMoriningEnd;
					tmNewTime += tmTempSpan;
					lNewMini = GetIntTimeFromTime(tmNewTime,nPeriod);
					goto minite1_5;
				}
			}
			else
			{
				goto minite1_5;
			}

minite1_5:
			if (nGZIndex)
			{
				if (lNewMini%10000 <= 1515)
				{
					return lNewMini;
				}
				else
				{
					lTmp = (lNewMini/10000) * 10000 + 1515;
					tmTemp = GetTimeFromIntTime(lTmp,nPeriod);
					tmTempSpan = tmNewTime - tmTemp;

					lNewMini = (lNewMini/10000) * 10000 + nQZQihuoMorningBegin;
					tmTemp =  GetTimeFromIntTime(lNewMini,nPeriod);
					tmTemp += oneDay;
					tmTemp += tmTempSpan;
					lNewMini = GetIntTimeFromTime(tmTemp,nPeriod);
					if (lNewMini%10000 <= 1130)
					{
						return lNewMini;
					}
					tmMoriningEnd = GetTimeFromIntTime((lNewMini/10000) * 10000 + 1130,nPeriod);
					tmTempSpan = tmTemp - tmMoriningEnd;
					lTmp = (lNewMini/10000) * 10000 + 1300;
					tmTemp = GetTimeFromIntTime(lTmp,nPeriod);
					tmTemp += tmTempSpan;
					return  GetIntTimeFromTime(tmTemp,nPeriod);
				}
			}
			else
			{
				if (lNewMini%10000 <= 1500)
				{
					return lNewMini;
				}
				else
				{
					lTmp = (lNewMini/10000) * 10000 + 1500;
					tmTemp = GetTimeFromIntTime(lTmp,nPeriod);
					tmTempSpan = tmNewTime - tmTemp;

					if (nMainType == STOCK_MARKET)						
						lNewMini = (lNewMini/10000) * 10000 + nStockMorningBegin;			
					else
						lNewMini = (lNewMini/10000) * 10000 + nQiHuoMorningBegin;

					tmTemp =  GetTimeFromIntTime(lNewMini,nPeriod);
					tmTemp += oneDay;
					tmTemp += tmTempSpan;
					lNewMini = GetIntTimeFromTime(tmTemp,nPeriod);
					if (lNewMini%10000 <= 1130)
					{
						return lNewMini;
					}
					tmMoriningEnd = GetTimeFromIntTime((lNewMini/10000) * 10000 + 1130,nPeriod);
					tmTempSpan = tmTemp - tmMoriningEnd;
					if (nMainType == STOCK_MARKET)	
						lTmp = (lNewMini/10000) * 10000 + 1300;
					else
						lTmp = (lNewMini/10000) * 10000 + 1330;

					tmTemp = GetTimeFromIntTime(lTmp,nPeriod);
					tmTemp += tmTempSpan;
					return  GetIntTimeFromTime(tmTemp,nPeriod);
				}
			}
		}
		break;
	default:
		return lBeginDate;
	}
}

long CHSDownloadData::GetPreDate( long lCurrentDate,int nDays,int nPeriodType )
{
	int nCount = 0;
	CTimeSpan tmOneDay(1,0,0,0);
	CTime tm = GetTimeFromIntTime(lCurrentDate,nPeriodType);
	while(nCount < nDays )
	{
		tm -= tmOneDay;
		if (tm.GetDayOfWeek() != 1 && tm.GetDayOfWeek() != 7 )
		{
			nCount ++;
		}
	}
	return GetIntTimeFromTime(tm,nPeriodType);
}

BOOL CHSDownloadData::IsDownLoaded( long lDate,CodeInfo *pCode,short nPeriod )
{
	long lLastTimeDownLoad = 20100101;//ReadWriteDownLoadDate(pCode,nPeriod,TRUE);
	if (lLastTimeDownLoad != lDate)
	{
		return FALSE;
	}
	else
	{
		CString strTime = "";// 服务器时间
		strTime = strTime.Right(6);
		char *pBuffer = strTime.GetBuffer();
		int nTime = atoi(pBuffer);
		strTime.ReleaseBuffer();
		int lTradeEndTime = 150000;
		short nGZIndex = MakeGZIndex(pCode->m_cCodeType);
		if (nGZIndex)
		{
			lTradeEndTime = 151500;
		}
		if (nTime <= lTradeEndTime)
			return TRUE;
		else
			return FALSE;
	}
}

void CHSDownloadData::InitListView()
{
	if ( m_lcStock.GetSafeHwnd() )
	{
		m_lcStock.InsertColumn(0,_T("证券代码"),LVCFMT_LEFT,75);
		m_lcStock.InsertColumn(1,_T("下载数据"),LVCFMT_LEFT,315);
		m_lcStock.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP); 
	}
	if ( m_pDataSource )
	{
		CArray<CodeInfo> arInfo;
// 		CodeInfo code;
// 		arInfo.Add(code);
		m_pDataSource->HSDataSourceEx_GetStocksUserInfo(arInfo, m_ayStock);
	}
	for (int i=0;i<m_ayStock.GetCount(); i++)
	{
		StockUserInfo* pStock = &m_ayStock.GetAt(i);
		if ( pStock )
		{
			m_lcStock.InsertItem(i,"");
			m_lcStock.SetItemText(i,0,pStock->GetName());
			m_lcStock.SetItemData(i,(LPARAM)pStock);
		}
	}
}

void CHSDownloadData::GetLocalData( CodeInfo *pCode,short nPeriod,TaskItem* pTask )
{
	pTask->Free();
	char* pBuf = NULL;
	int nDataCount = 0;
	AskData ask;
	ask.m_nType = RT_TECHDATA_EX;
	memcpy((void*)&ask.m_nPrivateKey.m_pCode, (void*)pCode, sizeof(CodeInfo));
	void* pData;
	if (m_pDataSource)
		pData = m_pDataSource->HSDataSource_GetDayData(&ask);
	if ( pData && !IsBadReadPtr(pData,1))
	{
		RetDiskData* pDisk = (RetDiskData*)pData;
		pTask->m_localData = (char*)pDisk->m_pData;
		pTask->m_localDataCount = pDisk->m_lSize;
	}
	
}


void CHSDownloadData::UpdateStockListView(CodeInfo *pCode,TaskItem *pTask,int nPeriod,BOOL bAllExist,BOOL &bWriteDownLoadDate)
{
	if (pCode == NULL)
	{
		return ;
	}
	bWriteDownLoadDate = TRUE;
	short nMainMarket = MakeMarket(pCode->m_cCodeType);
	short nSubMarket = MakeSubMarket(pCode->m_cCodeType);
	CString strText,strFormat,strType;
	strText.Empty();
	strFormat.Empty();
	strType.Empty();
	strText = m_lcStock.GetItemText(m_nPos,1);
	int nLimit = 16, nIndex = 2;
	BOOL bLimit = FALSE;
	if (nMainMarket == STOCK_MARKET && (nSubMarket == KIND_BOND || nSubMarket == KIND_FUND ||nSubMarket == KIND_QuanZhen || SC_Others == nSubMarket))
	{
		bLimit = TRUE;
		nLimit = 8;
	}

	switch(nPeriod)
	{
	case PERIOD_TYPE_DAY:
		if (bAllExist && pTask->m_localDataCount > 0)
		{
			m_nReloadDegree = 0;
			strType = "日线已更新   ";
			strText += strType;
		}
		else if(pTask->m_DayCount > 0)
		{
			m_nReloadDegree = 0;
			//			strFormat.Format("更新日线:%d   ",pTask->m_DayCount);
			strFormat.Format("日线已更新  ");
			strText += strFormat;
		}
		else if (pTask->m_localDataCount == 0 && pTask->m_DayCount == 0)
		{
			strFormat.Format("无日线数据  ");
			strText += strFormat;
		}
		break;
	case PERIOD_TYPE_MINUTE5:
		if (bAllExist && pTask->m_localDataCount > 0)
		{
			m_nReloadDegree = 0;
			strType = "5分钟已更新  ";
			strText += strType;
		}
		else if(pTask->m_Min5Count > 0)
		{
			m_nReloadDegree = 0;
			strFormat.Format("5分钟已更新  ");
			strText += strFormat;
		}
		else if (m_DownLoadCount > 9800 && pTask->m_localDataCount == 0 && pTask->m_Min5Count == 0)
		{
			long lNowDate = 0,nMaxDays = 41;
			short nGZIndex = MakeGZIndex(pCode->m_cCodeType);
			if (nGZIndex)
			{
				nMaxDays = 36;
			}
			CTime now = CTime::GetCurrentTime();
// 			const BourseInfo *pInfo = CStockManager::GetBourseInfo(pCode->m_cCodeType);
// 			if (pInfo)
// 				lNowDate = pInfo->m_lDate;
// 			else
				lNowDate = now.GetYear() * 10000 + now.GetMonth() * 100 + now.GetDay();

		//	long lLastDownLoadTime = ReadWriteDownLoadDate(pCode,nPeriod,TRUE);
// 			int nGapDays = GetWorkDays(lLastDownLoadTime,lNowDate);
// 			if (nGapDays < nMaxDays)
// 			{
// 				m_nReloadDegree = nLimit + 1;
// 			}

			if (m_nReloadDegree >= nLimit)
			{
				if (bLimit)
					strText += "5分钟无最近1年数据 ";
				else
					strText += "5分钟无最近2年数据 ";

				m_nReloadDegree = 0;
				break;
			}

			if (m_nReloadDegree == 0)
				m_nReloadDegree ++;
			else
				m_nReloadDegree = m_nReloadDegree*nIndex;

			bWriteDownLoadDate = FALSE;
			pTask->UnFinishedMinte5Task();
		}
		else
		{
			strFormat.Format("5分钟无数据可更新  ");
			strText += strFormat;
		}
		break;
	case PERIOD_TYPE_MINUTE1:
		if (bAllExist && pTask->m_localDataCount > 0 )
		{
			m_nReloadDegree = 0;
			strType = "1分钟已更新  ";
			strText += strType;
		}
		else if(pTask->m_Min1Count > 0)
		{
			m_nReloadDegree = 0;
			strFormat.Format("1分钟已更新  ");
			strText += strFormat;
		}
		else if (m_DownLoadCount > 9800 && pTask->m_localDataCount == 0 && pTask->m_Min1Count == 0)
		{
			long lNowDate = 0,nMaxDays = 41;
			short nGZIndex = MakeGZIndex(pCode->m_cCodeType);
			if (nGZIndex)
			{
				nMaxDays = 36;
			}
			CTime now = CTime::GetCurrentTime();
		//	const BourseInfo *pInfo = CStockManager::GetBourseInfo(pCode->m_cCodeType);
// 			if (pInfo)
// 				lNowDate = pInfo->m_lDate;
// 			else
				lNowDate = now.GetYear() * 10000 + now.GetMonth() * 100 + now.GetDay();

// 			long lLastDownLoadTime = ReadWriteDownLoadDate(pCode,nPeriod,TRUE);
// 			int nGapDays = CHSTools::GetWorkDays(lLastDownLoadTime,lNowDate);
// 			if (nGapDays < nMaxDays)
// 			{
// 				m_nReloadDegree = nLimit + 1;
// 			}

			if (m_nReloadDegree >= nLimit)
			{
				if (bLimit)
					strText += "1分钟无最近1年数据 ";
				else
					strText += "1分钟无最近2年数据 ";

				m_nReloadDegree = 0;
				break;
			}

			if (m_nReloadDegree == 0)
				m_nReloadDegree ++;
			else
				m_nReloadDegree = m_nReloadDegree*nIndex;

			bWriteDownLoadDate = FALSE;
			pTask->UnFinishedMinite1Task();
		}
		else
		{
			strFormat.Format("1分钟无数据可更新  ");
			strText += strFormat;
		}
		break;
	}
	m_lcStock.SetItemText(m_nPos,1,strText);
}