// HSDisk.cpp: implementation of the CHSDisk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "HSDisk.h"
#include "windowsx.h"
#include "KLineDataFile.h"
#include "WinnerApplication.h"
#include "GeneralHelper.h"
#include "TrendFile.h"
#include <afxtempl.h>
#include <io.h>
#include <direct.h>
const double FLOATZERO = 0.00000001;
// 81~86排名
#define	RT_AFTER_PRICE			0x0200		//成交价震幅倒序排名
#define	RT_AFTER_VOLBI			0x0400		//成交量变化(量比)倒序排名
#define	RT_AHEAD_VOLUME			0x0800		//成交量正序排名
#define	RT_AFTER_VOLUME			0x1000		//成交量倒序排名
#define	RT_AFTER_MONEY			0x2000		//资金流向倒序排名

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

extern BOOL IsValidAsk(AskData* pAsk, short nValidSize=0, unsigned short m_nType = 0);


BOOL CreateMultipleDirectory(char * sDirectory)
{   
	HANDLE fFile; //   File   Handle   
	WIN32_FIND_DATA fileinfo; //   File   Information   Structure   
	char sTemp[1024] = {0}; //   Temporary   Cstring   Object   

	//如果目录已存在则返回成功
	fFile =  FindFirstFile(sDirectory, &fileinfo);   
	if(fileinfo.dwFileAttributes   ==   FILE_ATTRIBUTE_DIRECTORY)   
	{   
		FindClose(fFile);   
		return TRUE;   
	}   

	for(size_t i = 0; i < strlen(sDirectory); i++)
	{   
		if(sDirectory[i] == '\\' || sDirectory[i] == '/')
		{
			CreateDirectory(sTemp, NULL);
		}   
		sTemp[i] = sDirectory[i];
		if(i == strlen(sDirectory)-1) //   If   we   reached   the   end   of   the   file   add   the   remaining   string   
		{
			CreateDirectory(sTemp, NULL);
		}
	}   

	//判断目录是否创建成功
	fFile = FindFirstFile(sDirectory, &fileinfo);   
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)   
	{   
		FindClose(fFile);   
		return   TRUE;   
	}   
	else   
	{   
		FindClose(fFile);   
		return   FALSE;   
	}   
}   
BOOL MakeFile(CString strFileName)
{
	CFileFind  findFind; 
	if ( strFileName.IsEmpty())
		return FALSE;
	//文件不存在
	if(!findFind.FindFile(strFileName))
	{
		//创建目录
		char sPath[1024] = {0};
		strcpy(sPath, strFileName);
		for (size_t i = strlen(sPath) - 1; i >= 0; i--)
		{
			if (sPath[i] == '\\' || sPath[i] == '/')
			{
				sPath[i] = '\0';
				break;
			}
		}
		if (strlen(sPath) > 0 && strlen(sPath) != strlen(strFileName))
			if (!CreateMultipleDirectory(sPath))
				return FALSE;

		CFile file;
		file.Open(strFileName,CFile::modeCreate);
		file.Close();
	}
	return TRUE;
}
void CreatePath(CString strBasePath)
{
	if( _access(strBasePath,0) == 0 )
		return ;

	_mkdir( strBasePath );

}

CTime LongToDate(unsigned long lDate)
{
	try
	{
		lDate %= 100000000;
		if( lDate <= 0 )
		{
			lDate = 0 - lDate;
			return 0;
		}


		int nYear = int(lDate/10000);
		int nMonth, nDay, nHour, nMinute;
		if(nYear < 1970)
		{
			CTime curTime = CTime::GetCurrentTime();

			nMonth  = nYear/100;
			nDay    = nYear%100;
			nHour   = (int)((lDate%10000)/100);
			nMinute = (int)(lDate%100);
			nYear = curTime.GetYear();
		}
		else if(nYear < 2038)
		{
			nMonth  = (int)((lDate%10000)/100);
			nDay    = (int)(lDate%100);
			nHour   = 0;
			nMinute = 0;
			//if(nYear > 2038)  // ??
			//	nYear = 2038;
		}
		else if(nYear >= 2038)
		{
			nYear = 2038 - 1;
			nMonth  = nYear/100;
			nDay    = nYear%100;
			nHour   = (int)((lDate%10000)/100);
			nMinute = (int)(lDate%100);
		}

		if(nDay < 1 || nDay > 31)
			nDay = 1;
		if(nMonth < 1 || nMonth > 12)
			nMonth = 1;
		if(nYear == 1970 && nMonth == 1 && nDay < 8 && nHour == 0)//这个时间CTime会报错
			nHour = 8;
		return CTime(nYear,nMonth,nDay,nHour,nMinute,0);
	}
	catch(...){}
	return 0;
}

void CHSDisk::Init()
{
	InitPath();	
}
void CHSDisk::InitPath()
{
	CString strDayBasePath =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::DATA_PATH);
	CreatePath(strDayBasePath);
}
int  CHSDisk::CaclExRight(StockDay* pDay, int nLen, CodeInfo* pCodeInfo, int nExRight/* = 0*/)
{
	if( pDay == NULL || pCodeInfo == NULL || nExRight == 0 )
		return -1;

	if( nExRight == 1 )
	{
		nExRight = HS_HxDataSource_Left_ChuQuan;
	}
	
	//need to modify
	CArray<ChuQuanData*,ChuQuanData*>* payExRight = NULL;//GetChuQuanAyData(pCodeInfo);
	if( payExRight == NULL )
		return -1;
	CArray<CodeInfo> ayCode;
	ayCode.Add(*pCodeInfo);
	CArray<StockUserInfo> ayStockInfo;
	long nPriceUnit = ayStockInfo.GetAt(0).m_nPriceUnit;
	for( int i = 0; i < nLen; i++ )
	{
		CalcChuQuan(pDay[i].m_lDate,AnalisysForDay, pDay[i], *payExRight, nExRight, nPriceUnit);  //--20100629 修改除权算法兼容新参数----
	}
	return nLen;
}
int CHSDisk::CalcChuQuan(unsigned long lPreDate,int nDayType,StockDay& pDay,
				CArray<ChuQuanData*,ChuQuanData*>& ayChuQuanData,
				long lMask,long nPriceUnit)
{
	
	ChuQuanData* pData;
	BOOL bCheck = FALSE;
	int i;

	//yangdl 2007.08.20 分钟线也除权

	unsigned long lDate = 0 ;
	if ( pDay.m_lDate > 99999999 ) //分钟线
	{
		lDate = pDay.m_lDate / 10000;
		long lYear = lDate / 10000 - 10 + 2000 ;
		lDate = lYear * 10000 + lDate % 10000;
	}
	
	//----20100629 litao增加当除权日是本周期的第一天时的判断,可能会因为节假日造成第一天判断不对,如果
	//----要彻底修改多日线除权发生的本周期最高价除权计算不对的情况,需要先计算日复权数据再生成周复权数据,
	//----而不是目前的根据周除权数据来计算,这个在重构项目中要注意修改掉
	BOOL bFirstDayPower = FALSE;  //本周期第一天是否是除权日
	BOOL bMoreDays = (nDayType >= AnalisysForWeek && nDayType <= AnalisysForMoreDay)?TRUE:FALSE;
	if (bMoreDays == TRUE)
	{
		if (pDay.m_lOpenPrice > FLOATZERO)
		{
			bFirstDayPower = (pDay.m_lPrevClose/pDay.m_lOpenPrice > 1.051)?TRUE:FALSE;
		}
	}
	//-------------------------------------------------------------------------------------------------

	// 除权
	if ( lMask & HS_HxDataSource_Left_ChuQuan )	 // 向前除权
	{
		for(i = 0; i < ayChuQuanData.GetSize(); i++)
		{
			pData = ayChuQuanData.GetAt(i);
			if( pData != NULL )
			{
				//--------20100629 litao 修改继续通过除权的日期来判断除权日是否本周期的第一天--------------------------------
				if (bMoreDays == TRUE)
				{
					if (nDayType == AnalisysForWeek)
					{
						long lSpanDays = 0;
						CTime tBgnDate = LongToDate(lPreDate);
						CTime tEndDate = LongToDate(pData->m_lDate);
						CTimeSpan tsSpan = tEndDate - tBgnDate;
						lSpanDays = tsSpan.GetDays();
						if (lSpanDays == 3)   //当周除权日期和上周只差两天则判断为本周第一天
							bFirstDayPower = TRUE;
					}
				}
				//-----------------------------------------------------------------------------------------------------------
				if( (pData->m_cMask == ChuQuanData::Calc) &&
					( (pDay.m_lDate < pData->m_lDate) 
					|| ( ( lDate > 0 ) && (lDate < pData->m_lDate) )
					) )
				{
					// 前复权:复权后价格＝[(复权前价格-现金红利)＋配(新)股价格×流通股份变动比例]÷(1＋流通股份变动比例) 
					pDay.m_lOpenPrice  = ((pDay.m_lOpenPrice/(double)nPriceUnit  + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lMaxPrice   = ((pDay.m_lMaxPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lMinPrice   = ((pDay.m_lMinPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lClosePrice = ((pDay.m_lClosePrice/(double)nPriceUnit + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lTotal	   = (long)(pDay.m_lTotal*pData->m_fMulit);
					pDay.m_lMoney	   = (long)(pDay.m_lMoney*pData->m_fMulit);
				}
				//-修改当除权在本周期某一天发生时对于最高最低需要按公式计算-------------------------
				else if((bMoreDays == TRUE) && (bFirstDayPower == FALSE)
					&&(pData->m_cMask == ChuQuanData::Calc) 
					&& (pDay.m_lDate >= pData->m_lDate) && (lPreDate < pData->m_lDate))
				{
					pDay.m_lOpenPrice  = ((pDay.m_lOpenPrice/(double)nPriceUnit  + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lMaxPrice   = ((pDay.m_lMaxPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
				}
			}
		}
	}
	else if( lMask & HS_HxDataSource_Back_ChuQuan ) // 向后除权
	{
		for(i = 0; i < ayChuQuanData.GetSize(); i++)
		{
			pData = ayChuQuanData.GetAt(i);
			if(pData != NULL)
			{
				//--------20100629 litao 修改继续通过除权的日期来判断除权日是否本周期的第一天--------------------------------
				if (bMoreDays == TRUE)
				{
					if (nDayType == AnalisysForWeek)
					{
						long lSpanDays = 0;
						CTime tBgnDate = LongToDate(lPreDate);
						CTime tEndDate = LongToDate(pData->m_lDate);
						CTimeSpan tsSpan = tEndDate - tBgnDate;
						lSpanDays = tsSpan.GetDays();
						if (lSpanDays == 3)   //当周除权日期和上周只差两天则判断为本周第一天
							bFirstDayPower = TRUE;
					}
				}
				//-----------------------------------------------------------------------------------------------------------
				//yangdl 2007.08.17 后除权修改
				if( (pData->m_cMask == ChuQuanData::Calc) &&
					( ( ( pDay.m_lDate >= pData->m_lDate ) && ( lDate == 0 ) ) 
					||  ( lDate >= pData->m_lDate ) 
					) )
					/*if( (pData->m_cMask == ChuQuanData::Calc) &&
					(pDay.m_lDate >= pData->m_lDate) )*/
				{
					/*pDay.m_lOpenPrice  = ((pDay.m_lOpenPrice/(double)nPriceUnit  + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lMaxPrice   = ((pDay.m_lMaxPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lMinPrice   = ((pDay.m_lMinPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lClosePrice = ((pDay.m_lClosePrice/(double)nPriceUnit + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
					pDay.m_lTotal	   = (long)(pDay.m_lTotal*pData->m_fMulit);
					pDay.m_lMoney	   = (long)(pDay.m_lMoney*pData->m_fMulit);*/

					// 后复权:复权后价格＝复权前价格×(1＋流通股份变动比例)-配(新)股价格×流通股份变动比例＋现金红利 
					//---litao 20100628 修改当除权在本周期某一天发生时对于最高最低需要按公式计算-------------------------
					if((bMoreDays == TRUE) && (bFirstDayPower == FALSE) 
						&& (pDay.m_lDate >= pData->m_lDate) && (lPreDate < pData->m_lDate))
					{
						pDay.m_lOpenPrice  = pDay.m_lOpenPrice;
						pDay.m_lMaxPrice   = pDay.m_lMaxPrice;
					}
					else
					{
						pDay.m_lOpenPrice  = ((pDay.m_lOpenPrice/(double)nPriceUnit  * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;
						pDay.m_lMaxPrice   = ((pDay.m_lMaxPrice/(double)nPriceUnit   * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;
					}
					//----------------------------------------------------------------------------------------------------

					
					pDay.m_lMinPrice   = ((pDay.m_lMinPrice/(double)nPriceUnit   * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;
					pDay.m_lClosePrice = ((pDay.m_lClosePrice/(double)nPriceUnit * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;

					pDay.m_lTotal	   = (long)(pDay.m_lTotal*pData->m_fMulit);
					pDay.m_lMoney	   = (long)(pDay.m_lMoney*pData->m_fMulit);

				}

			}			
		}

	}
	return 1;
}


CHSDisk::CHSDisk()
{

}

CHSDisk::CHSDisk(HWND hWnd,UINT nNotifyMessage)
{

}

CHSDisk::~CHSDisk()
{
	POSITION pos = m_mapTrendfile.GetStartPosition();
	WORD key;
	CTrendFile* pFile;
	while(pos)
	{
		m_mapTrendfile.GetNextAssoc(pos,key,(void*&)pFile);
		if (pFile && !IsBadReadPtr(pFile,1))
		{
			delete pFile;
		}
	}
	CString strKey;
	pos = m_mapHisTrendfile.GetStartPosition();
	while(pos)
	{
		m_mapHisTrendfile.GetNextAssoc(pos,strKey,(void*&)pFile);
		if (pFile && !IsBadReadPtr(pFile,1))
		{
			delete pFile;
		}
	}
	pos = m_mapTickFile.GetStartPosition();
	while(pos)
	{
		m_mapTickFile.GetNextAssoc(pos,strKey,(void*&)pFile);
		if (pFile && !IsBadReadPtr(pFile,1))
		{
			delete pFile;
		}
	}
	m_mapTrendfile.RemoveAll();
	m_mapHisTrendfile.RemoveAll();
	m_mapTickFile.RemoveAll();
	CKLineDataFile::RemoveMap();
}

void* CHSDisk::GetDayData(void* pAsk,int nMask /*= 0*/, int nExRight/* = 0*/)
{
	if(  pAsk == NULL )
	{
		return NULL;
	}

	AskData* pCurAsk = (AskData*)pAsk;
	if( !IsValidAsk(pCurAsk, sizeof(ReqDayData)) )
	{
		return NULL;
	}
	ReqDayData* pDayAsk = (ReqDayData*)pCurAsk->m_pCode;
	if( nMask == 2 && pDayAsk->m_nSize > 0 ) // 本地请求一次
	{
		return NULL;
	}

	char* pTempBuf = NULL;
	int nDataCount = pDayAsk->m_nDay;
	CKLineDataFile::ReadKLineDataEx(&pDayAsk->m_ciCode,pDayAsk->m_cPeriod,&pTempBuf,nDataCount);
	StockCompDayDataEx* pStockDay = (StockCompDayDataEx*)pTempBuf;

	if(pStockDay != NULL && nDataCount > 0)
	{
		short nOldPeriodNum = pDayAsk->m_nPeriodNum;
		if( nMask == 0 || nMask == 2 )  // 主窗口, 此处不进行周期转换
		{
			pDayAsk->m_nPeriodNum = 1;
		}

		//数据抽取
		RetDiskData *pRetData = MakePeriodData(pStockDay, nDataCount, pDayAsk, nMask, nExRight);		
		pDayAsk->m_nPeriodNum = nOldPeriodNum;
		delete[] pStockDay;

		if( pRetData == NULL || !pRetData->IsValid() )
		{
			return NULL;
		}

		return pRetData;
	}

	return NULL;
}

void* CHSDisk::GetDayData(void* pAsk,long lRetSize,int nMask /*= 0*/, int nExRight /*= 0*/)
{
	AskData* pCurAsk = (AskData*)pAsk;
	if(pCurAsk == NULL)
		return NULL;

	//
	short nCount = sizeof(ReqDayData) / sizeof(CodeInfo);
	if( sizeof(ReqDayData) % sizeof(CodeInfo) )
	{
		nCount++;
	}
	int lLen = sizeof(AskData) + sizeof(CodeInfo) * (nCount - 1);

	char* pbuffer;
	pbuffer = new char[lLen];
	AskData* ask = (AskData*)pbuffer;
	memset(ask,0,lLen);

	ask->m_nIndex = 0;
	ask->m_nType  = RT_TECHDATA_EX;
	ask->m_nSize  = nCount;
	ReqDayData* pReqDayData = (ReqDayData*)ask->m_pCode;
	memcpy(&pReqDayData->m_ciCode,pCurAsk->m_pCode,sizeof(pReqDayData->m_ciCode));
	pReqDayData->m_nPeriodNum = 1;
	pReqDayData->m_cPeriod = PERIOD_TYPE_DAY;

	return GetDayData(ask, 1);

}

RetDiskData* CHSDisk::MakePeriodData(StockCompDayDataEx* pDataStartPos, UINT nCount, 
							ReqDayData* pAskData,
							int nMask /*= 0*/, int nExRight/* = 0*/)
{
	if( !pDataStartPos || !pAskData )
		return NULL;

	static RetDiskData tagRetData0;
	static RetDiskData tagRetData1;

	RetDiskData* pTagCurRetData = NULL;
	if( nMask == 0 || nMask == 2 )	  // 主窗口要的数据
	{
		pTagCurRetData = &tagRetData0;
	}
	else if( nMask == 1 ) // 选股、排序、计算要的数据
	{
		pTagCurRetData = &tagRetData1;
	}
	else
	{
		return NULL;
	}

	// 
	ReqDayData* pAsk = pAskData;

	if( nExRight &&  pAsk->m_cPeriod == PERIOD_TYPE_DAY )
	{
		// 如果为１，则默认为向前除权

		CaclExRight((StockDay*)pDataStartPos, nCount, &pAsk->m_ciCode, nExRight);
	}

	// 基数直接返回
	if( pAsk->m_nPeriodNum <= 1 ) // 分析周数小等于1,则为基数周期数据
	{
		pTagCurRetData->Free();
		pTagCurRetData->Alloc(nCount);
		memcpy(pTagCurRetData->m_pData, pDataStartPos, sizeof(StockCompDayDataEx) * nCount);		
		return pTagCurRetData;
	}

	//
	pTagCurRetData->Alloc( nCount );
	if( !pTagCurRetData->IsValid() )
	{
		return NULL;
	}

	StockDay* pCurData  = (StockDay*)pTagCurRetData->m_pData;
	StockDay* pStockDay = (StockDay*)pDataStartPos;

	StockDay* pUseStockDay = NULL;

	//static UINT m_pDataEnd	= pStockDay + nCount - 1;//计算最后一个数据的位置

	UINT nCursor = 0;

	int nCurSize = 0;

	while(nCursor < nCount )
	{
	}

	pTagCurRetData->m_lSize = nCurSize; // 实际长度
	return pTagCurRetData;
}

BOOL CHSDisk::WriteDayDataNew(CodeInfo* info,int period,StockCompDayDataEx* pNewData,long nSize )
{
	CKLineDataFile::WriteKLineDataEx(info,period,(char*)pNewData,
		nSize * sizeof(StockCompDayDataEx));
	return TRUE;
}

void* CHSDisk::GetTraceData( void* pAsk ,long& nSize,long lDate)
{
	if(  pAsk == NULL )
	{
		return NULL;
	}

	AskData* pCurAsk = (AskData*)pAsk;
	if( !IsValidAsk(pCurAsk) )
	{
		return NULL;
	}
	CodeInfo* pCode  = pCurAsk->m_pCode;
	if ( pCode )
	{		
		CTrendFile* trenFile = NULL;
		int nType = MakeMainMarket(pCode->m_cCodeType);
		if ( !m_mapTrendfile.Lookup( nType, (void*&)trenFile ))
		{
			trenFile = new CTrendFile;
			trenFile->Open(pCode, CTrendFile::NewTrace) ;
			m_mapTrendfile.SetAt(nType, (void*&)trenFile);
		}
		if ( lDate <= 0 )
		{
			CString strTime;
			CTime t=CTime::GetCurrentTime();
			strTime.Format("%04d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay());
			lDate = atoi(strTime.GetBuffer());
		}
		if ( trenFile->InitIndex(pCode->m_cCodeType) )
			return trenFile->Read(lDate,nSize, pCode);
		
	}
	return NULL;

}
//历史分时
void* CHSDisk::GetHisTraceData( void* pAsk ,long& nSize)
{
	if(  pAsk == NULL )
	{
		return NULL;
	}

	AskData* pCurAsk = (AskData*)pAsk;
	if( !IsValidAsk(pCurAsk) )
	{
		return NULL;
	}
	CodeInfo* pCode  = pCurAsk->m_pCode;
	if (pCode)
	{
		CTrendFile* pFile = NULL;
		if ( !m_mapHisTrendfile.Lookup(pCode->GetKey(),(void*&)pFile ) )
		{
			pFile = new CTrendFile;
			pFile->Open(pCode, CTrendFile::HisTrace) ;
			m_mapHisTrendfile.SetAt(pCode->GetKey(), (void*&)pFile);
		}
		ReqHisTrend* pHisTrend = (ReqHisTrend*)pCurAsk->m_pCode;  
		long lDate = pHisTrend->m_lDate;
		pFile->InitIndex();
		return pFile->Read(lDate,nSize);
		
	}
	return NULL;
}
void*  CHSDisk::GetTickData(void* pAsk,long& nSize,long lDate)
{
	if(  pAsk == NULL )
	{
		return NULL;
	}

	AskData* pCurAsk = (AskData*)pAsk;
	if( !IsValidAsk(pCurAsk) )
	{
		return NULL;
	}
	CodeInfo* pCode  = pCurAsk->m_pCode;
	if (pCode)
	{
		CTrendFile* pFile = NULL;
		if ( !m_mapTickFile.Lookup(pCode->GetKey(),(void*&)pFile ) )
		{
			pFile = new CTrendFile;
			pFile->Open(pCode, CTrendFile::Tick) ;
			m_mapTickFile.SetAt(pCode->GetKey(), (void*&)pFile);
		}
		pFile->InitIndex();
		if ( lDate <= 0 )
		{
			CString strTime;
			CTime t=CTime::GetCurrentTime();
			strTime.Format("%04d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay());
			lDate = atoi(strTime.GetBuffer());
		}
		return pFile->Read(lDate,nSize,pCode);				
	}
	return NULL;
}

BOOL CHSDisk::WriteHisTraceData(CodeInfo* pCode, long lDate, void* pData, long lSize )
{
	if(  pData == NULL )
	{
		return FALSE;
	}
	if (pCode)
	{	
		CTrendFile* pFile = NULL;
		if ( !m_mapHisTrendfile.Lookup(pCode->GetKey(),(void*&)pFile ) )
		{
			pFile = new CTrendFile;
			pFile->Open(pCode, CTrendFile::HisTrace) ;
			m_mapHisTrendfile.SetAt(pCode->GetKey(), (void*&)pFile);
		}
	
		pFile->InitIndex();
		if ( lDate <= 0 )
		{
			CString strTime;
			CTime t=CTime::GetCurrentTime();
			strTime.Format("%04d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay());
			lDate = atoi(strTime.GetBuffer());
		}
		return pFile->Write((char*)pData,lSize,lDate,pCode);
		
	}	
	return FALSE;
}
BOOL CHSDisk::WriteTickData(CodeInfo* pCode, void* pData, long lSize,long lDate)
{
	if(  pData == NULL )
	{
		return FALSE;
	}
	if (pCode)
	{
		CTrendFile* pFile = NULL;
		if ( !m_mapTickFile.Lookup(pCode->GetKey(),(void*&)pFile ) )
		{
			pFile = new CTrendFile;
			pFile->Open(pCode, CTrendFile::Tick) ;
			m_mapTickFile.SetAt(pCode->GetKey(), (void*&)pFile);
		}
	
		if ( lDate <= 0 )
		{
			CString strTime;
			CTime t=CTime::GetCurrentTime();
			strTime.Format("%04d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay());
			lDate = atoi(strTime.GetBuffer());
		}
		pFile->InitIndex();

		return pFile->Write((char*)pData,lSize,lDate,pCode);
		
	}	
	return FALSE;
}
BOOL CHSDisk::WriteNewTraceData( CodeInfo* pCode, void* pData, long lSize ,long lDate)
{
	if(  pData == NULL )
	{
		return FALSE;
	}
	if (pCode)
	{
		CTrendFile* trenFile = NULL;
		int nType = MakeMainMarket(pCode->m_cCodeType);
		if ( !m_mapTrendfile.Lookup( nType, (void*&)trenFile ))
		{
			trenFile = new CTrendFile;
			trenFile->Open(pCode, CTrendFile::NewTrace) ;
			m_mapTrendfile.SetAt(nType, (void*&)trenFile);
		}
		trenFile->InitIndex(pCode->m_cCodeType);
		if ( lDate <= 0 )
		{
			CString strTime;
			CTime t=CTime::GetCurrentTime();
			strTime.Format("%04d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay());
			lDate = atoi(strTime.GetBuffer());
		}
		return trenFile->Write((char*)pData,lSize,lDate,pCode);
	}	
	return FALSE;
}	