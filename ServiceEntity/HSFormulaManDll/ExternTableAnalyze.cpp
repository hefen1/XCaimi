/*******************************************************************
* 编写:熊钟彬
* 说明:用于盘后区间分析数据处理
* 备注:
*******************************************************************/

#include "stdafx.h"
#include "ExternTableAnalyze.h"
//#include "..\ylsdrawbase\ylsDrawOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ETA_READOPEN_FLAG    (CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary)

//BEGIN_MESSAGE_MAP(CPhaseSort, CDialog)
//END_MESSAGE_MAP()

CExternTableAnalyze::CExternTableAnalyze()
{

}
CExternTableAnalyze::~CExternTableAnalyze()
{
}
BOOL CExternTableAnalyze::GetDataPosition(CFile *pSrcFile, 
										  long lSeekBeginDate, long *plBeginPos,
										  long lSeekEndDate/* = -1*/, 
										  long *plEndPos /* = NULL*/)
{
	ASSERT(pSrcFile && pSrcFile->m_hFile && lSeekBeginDate > 19700101 && plBeginPos);
	/*if(!(pSrcFile && pSrcFile->m_hFile && lSeekDate > 19700101))
		return FALSE;
	*/
	try
	{
		long lDate = 0;
		long lFileLen = pSrcFile->GetLength();
		long lPosition = 0;

		while(lPosition < lFileLen)
		{
			pSrcFile->Seek(lPosition, CFile::begin );
			pSrcFile->Read(&lDate, sizeof(long));
			if(lDate >= lSeekBeginDate)//寻找左区间
			{
				*plBeginPos = lPosition;
				if(lSeekEndDate == -1)
					return TRUE;

				while(lPosition < lFileLen)//寻找右区间
				{
					pSrcFile->Seek(lPosition, CFile::begin );
					pSrcFile->Read(&lDate, sizeof(long));
					if(lDate < lSeekEndDate)
					{
						lPosition += sizeof(StockDay);
						continue;
					}
					else if(lDate == lSeekEndDate)
					{
						*plEndPos = lPosition;
						return TRUE;
					}
					else if(lDate > lSeekEndDate)
					{
						*plEndPos = lPosition - sizeof(StockDay);
						return TRUE;
					}					
				}
				*plEndPos = lPosition - sizeof(StockDay);//最后一个数据作为末数据
				return TRUE;
			}
			lPosition += sizeof(StockDay);
		}
	}
	catch(...)
	{
		
	}
	return FALSE;
}

int CExternTableAnalyze::GetData(RetDiskData* pData, StockUserInfo* pStock, long lEnd, long lCount/* = NULL*/)
{
	ASSERT(pStock && pData);
	if(pStock == NULL || pData == NULL)
		return 0;

	/*CHSDisk* pDisk = (CHSDisk*)CHSChildFrame::m_lDiskID;
	ASSERT(pDisk);
	if(pDisk == NULL)
		return 0;
	*/

	CString strFile = CHSDisk::GetDayOfPath(& pStock->m_ciStockCode, HISDAY);
	try
	{
		CFile File; 
		if(!File.Open(strFile, ETA_READOPEN_FLAG))
		{
			return -1;
		}
		long lBegin = GetBeginDate(lEnd, lCount);
		long lBeginPos, lEndPos; 
		if(! GetDataPosition(&File, lBegin, &lBeginPos, lEnd, &lEndPos) )
		{
			return -1;
		}
		
		pData->Alloc((lEndPos - lBeginPos + sizeof(StockDay))/sizeof(StockDay));
		if(pData->IsValid())
		{
			File.Seek(lBeginPos - sizeof(StockDay), CFile::begin );
			File.Read(pData->m_pData, sizeof(StockDay));
			//yangdl 2008.03.11 昨收特殊处理价格
	#ifndef Support_XHX_Dll
			if(pStock && MakeNegativeIndex( pStock->m_ciStockCode.m_cCodeType) )
			{
				pStock->m_lPrevClose = Negative(((StockDay*)pData->m_pData)->m_lClosePrice,TRUE);
			}
			else
	#endif
			pStock->m_lPrevClose = ((StockDay*)pData->m_pData)->m_lClosePrice;

			File.Read(pData->m_pData, pData->m_lSize * sizeof(StockDay));
		}
		File.Close();
	}
	catch(...){}
	return pData->m_lSize ;
}

StockDay* CExternTableAnalyze::GetPeriodStockDay(StockDay* pStockDay,short nPeriod)
{
	ASSERT( pStockDay);

	static StockDay tagStockDay;

	memcpy(&tagStockDay, pStockDay, sizeof(StockDay));
	if(nPeriod == 1)
	{
		return &tagStockDay;
	}
	StockDay* pUseStockDay = pStockDay;
	//开
	tagStockDay.m_lOpenPrice  = pUseStockDay->m_lOpenPrice;
	
	//高,低,成交量/额
	long lHigh   = pUseStockDay->m_lMaxPrice;
	long lLow	 = pUseStockDay->m_lMinPrice;
	tagStockDay.m_lMoney = pUseStockDay->m_lMoney;
	tagStockDay.m_lTotal = pUseStockDay->m_lTotal;

	pUseStockDay++;
	for(short i = 1 ;i < nPeriod ; i++, pUseStockDay++)
	{
		if (pUseStockDay->m_lMaxPrice > lHigh )
		{
			lHigh	= pUseStockDay->m_lMaxPrice;
		}
		if (pUseStockDay->m_lMinPrice < lLow)
		{
			lLow	= pUseStockDay->m_lMinPrice;
		}
		tagStockDay.m_lMoney	+= pUseStockDay->m_lMoney;
		tagStockDay.m_lTotal	+= pUseStockDay->m_lTotal;
	}
	tagStockDay.m_lMaxPrice = lHigh;
	tagStockDay.m_lMinPrice	= lLow;
	
	//时间、收
	pUseStockDay--;// 最后一个数据
	tagStockDay.m_lDate  = pUseStockDay->m_lDate;
	tagStockDay.m_lClosePrice = pUseStockDay->m_lClosePrice;

	return &tagStockDay;
}

/********************************************************************************
* 格式化数值
* 函数原型：CPublicData::LongToString
*********************************************************************************/
void CExternTableAnalyze::LongToFloat(long lData, long lClose, long lUnit, short nDecimal, float &fData)
{
	if((lData == 0) || (lUnit <= 0))
	{
		fData = 0;
	}
	else
	{		
		char strFormat[16];
		if(lData < 0)
			lData = -lData;
		char strText[32];
		if(nDecimal < 0)
		{
			sprintf(strFormat, "%%0%dld",-nDecimal);
			sprintf(strText,strFormat, (lData/lUnit));
		}
		else if(nDecimal == 0)
		{
			sprintf(strText, "%ld", (lData/lUnit));
		}                           
		else
		{
			sprintf(strFormat, "%%.%dlf",nDecimal);
			sprintf(strText, strFormat, (double)lData/(double)lUnit);
		}
		fData  = atof(strText);
	}
}