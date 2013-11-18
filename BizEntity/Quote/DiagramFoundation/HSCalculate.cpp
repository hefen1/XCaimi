#include "StdAfx.h"
#include "HSCalculate.h"
#include "..\..\..\HelperLib\GeneralHelper.h"
#include "DiagramComm.h"
#include "..\..\..\Common\hsds_comudata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CHSCalculate::CHSCalculate(void)
{
}

CHSCalculate::~CHSCalculate(void)
{
}

double CHSCalculate::GetBuySellCount(const StockUserInfo* pStockInfo, long lValue, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = (double)lValue / (double)pStockInfo->m_nHandNum;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetWeiBi(const HSStockRealTime* pRealTime, char strText[64] )
{// ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	double dValue = 0.0;
	if (pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long BuyCountTotal  =  pRealTime->m_lBuyCount1 + pRealTime->m_lBuyCount2 + 
		pRealTime->m_lBuyCount3 + pRealTime->m_lBuyCount4 + pRealTime->m_lBuyCount5;
	long SellCountTotal =  pRealTime->m_lSellCount1 + pRealTime->m_lSellCount2 + 
		pRealTime->m_lSellCount3 + pRealTime->m_lSellCount4 + pRealTime->m_lSellCount5;
	long A = BuyCountTotal - SellCountTotal;
	long B = BuyCountTotal + SellCountTotal; 

	short nMark = LTOA_MASK_SHOW_PERSENT | LTOA_MASK_HORLINE_IFZERO;
	if((B != 0) && (A != 0))
		dValue = 10000.0 * (double)A / (double)B;
	CGeneralHelper::LongToString(dValue, 0, (long)100, (short)2, strText, nMark);	

	return dValue;
}

double CHSCalculate::GetWeiBi(const LevelRealTime* pLevel, char strText[64])
{// ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	double dValue = 0.0;
	if (pLevel == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long BuyCount  =  pLevel->m_lBuyCount1 + pLevel->m_lBuyCount2 + 
		pLevel->m_lBuyCount3 + pLevel->m_lBuyCount4 + pLevel->m_lBuyCount5 +
		pLevel->m_lBuyCount6 + pLevel->m_lBuyCount7 + pLevel->m_lBuyCount8 +
		pLevel->m_lBuyCount9 + pLevel->m_lBuyCount10;

	long SellCount =  pLevel->m_lSellCount1 + pLevel->m_lSellCount2 + 
		pLevel->m_lSellCount3 + pLevel->m_lSellCount4 + pLevel->m_lSellCount5 +
		pLevel->m_lSellCount6 + pLevel->m_lSellCount7 + pLevel->m_lSellCount8 +
		pLevel->m_lSellCount9 + pLevel->m_lSellCount10;

	long A = BuyCount - SellCount;
	long B = BuyCount + SellCount;
	short nMark = LTOA_MASK_SHOW_PERSENT | LTOA_MASK_HORLINE_IFZERO;
	if((B != 0) && (A != 0))
	{
		dValue = 10000.0 * (double)A/(double)B;
		dValue = dValue>0?dValue+0.5:dValue-0.5;
	} 
	CGeneralHelper::LongToString(dValue, 0, 100, 2, strText, nMark);		

	return dValue;
}

double CHSCalculate::GetWeiBi(const HSIndexRealTime* pRealTime, char strText[64])
{// ί�ȣ�(ί��������ί������)/(ί��������ί������)��100��
	double dValue = 0.0;
	if (pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long A =  pRealTime->m_lBuyCount - pRealTime->m_lSellCount;
	long B =  pRealTime->m_lBuyCount + pRealTime->m_lSellCount;

	short nMark = LTOA_MASK_SHOW_PERSENT | LTOA_MASK_HORLINE_IFZERO;
	if((B != 0) && (A != 0))
		dValue = 10000.0 * (double)A / (double)B;
	CGeneralHelper::LongToString(dValue, 0, (long)100, (short)2, strText, nMark);

	return dValue;
}

double CHSCalculate::GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	short nMark = bStop?LTOA_MASK_HORLINE_IFZERO:LTOA_MASK_FIXEDLEN_IFZERO;
	dValue = pRealTime->m_lNewPrice==0?0:pRealTime->m_lNewPrice - pStockInfo->m_lPrevClose;
	CGeneralHelper::LongToString(dValue , 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, nMark);

	return dValue;
}

double CHSCalculate::GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	short nMark = bStop?LTOA_MASK_HORLINE_IFZERO:LTOA_MASK_FIXEDLEN_IFZERO;
	dValue = pLevel->m_lNewPrice==0?0:pLevel->m_lNewPrice - pStockInfo->m_lPrevClose;
	CGeneralHelper::LongToString(dValue , 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, nMark);

	return dValue;
}

double CHSCalculate::GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pIndex == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	short nMark = bStop?LTOA_MASK_HORLINE_IFZERO:LTOA_MASK_FIXEDLEN_IFZERO;
	dValue = pIndex->m_lNewPrice==0?0:pIndex->m_lNewPrice - pStockInfo->m_lPrevClose;
	CGeneralHelper::LongToString(dValue , 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, nMark);

	return dValue;
}

double CHSCalculate::GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	short nMark = bStop?LTOA_MASK_HORLINE_IFZERO:LTOA_MASK_FIXEDLEN_IFZERO;
	nMark |= LTOA_MASK_SHOW_PERSENT;
	if (pStockInfo->m_lPrevClose == 0)
	{
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, 0, strText, nMark);	
	}
	else
	{
		dValue = pRealTime->m_lNewPrice == 0?0: (double)(pRealTime->m_lNewPrice - pStockInfo->m_lPrevClose) / (double)pStockInfo->m_lPrevClose * 10000;
		CGeneralHelper::LongToString(dValue, 0, 100, pStockInfo->m_nDecimal, strText, nMark);
	}

	return dValue;
}

double CHSCalculate::GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	short nMark = bStop?LTOA_MASK_HORLINE_IFZERO:LTOA_MASK_FIXEDLEN_IFZERO;
	nMark |= LTOA_MASK_SHOW_PERSENT;
	if (pStockInfo->m_lPrevClose == 0)
	{
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, 0, strText, nMark);	
	}
	else
	{
		dValue = pLevel->m_lNewPrice == 0?0: (double)(pLevel->m_lNewPrice - pStockInfo->m_lPrevClose) / (double)pStockInfo->m_lPrevClose * 10000;
		CGeneralHelper::LongToString(dValue, 0, 100, pStockInfo->m_nDecimal, strText, nMark);
	}

	return dValue;
}

double CHSCalculate::GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pIndex == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	short nMark = bStop?LTOA_MASK_HORLINE_IFZERO:LTOA_MASK_FIXEDLEN_IFZERO;
	nMark |= LTOA_MASK_SHOW_PERSENT;
	if (pStockInfo->m_lPrevClose == 0)
	{
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, 0, strText, nMark);	
	}
	else
	{
		dValue = pIndex->m_lNewPrice == 0?0: (double)(pIndex->m_lNewPrice - pStockInfo->m_lPrevClose) / (double)pStockInfo->m_lPrevClose * 10000;
		CGeneralHelper::LongToString(dValue, 0, 100, pStockInfo->m_nDecimal, strText, nMark);
	}

	return dValue;
}

double CHSCalculate::GetZongshou(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}
	
	if (MakeSubMarket(pStockInfo->m_ciStockCode.m_cCodeType) == KIND_QuanZhen)
		dValue = pRealTime->m_lTotal;
	else
		dValue = (double)pRealTime->m_lTotal / (double)pStockInfo->m_nHandNum;

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetZongshou(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if (MakeSubMarket(pStockInfo->m_ciStockCode.m_cCodeType) == KIND_QuanZhen)
		dValue = pLevel->m_lTotal;
	else
		dValue = (double)pLevel->m_lTotal / (double)pStockInfo->m_nHandNum;

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetZongshou(const HSIndexRealTime* pRealTime, int nHand, short nMask, char strText[64])
{
	double dValue = 0.0;
	if (pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText, _T(" -"));
		return dValue;
	}

	dValue = (double)pRealTime->m_lTotal / (double)nHand;
	
	int nDecimal = 2;
	if (nMask == LTOA_MASK_HORLINE_IFZERO)
		nDecimal = 0;
	CGeneralHelper::LongToString(dValue, 0, 1, nDecimal, strText, nMask);

	return dValue;
}

double CHSCalculate::GetXianshou(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pOther == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if (MakeSubMarket(pStockInfo->m_ciStockCode.m_cCodeType) == KIND_QuanZhen)
		dValue = pOther->m_lCurrent;
	else
		dValue = (double)pOther->m_lCurrent / (double)pStockInfo->m_nHandNum;

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetXianshou(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevelOther == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if (MakeSubMarket(pStockInfo->m_ciStockCode.m_cCodeType) == KIND_QuanZhen)
		dValue = pLevelOther->m_lCurrent;
	else
		dValue = (double)pLevelOther->m_lCurrent / (double)pStockInfo->m_nHandNum;

	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetShiying(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL || pFinanceData == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	double nQuarter = 4; 
	if( pFinanceData)
	{
		HSDouble dRet = pFinanceData->GetDataByID(COLUMN_FINANCE_PER_INCOME);
		if ( dRet.m_dData != hx_DefMaxValues && dRet.m_dData != 0 )
		{
			dValue = dRet;
		}
		else
		{
			dValue = 0;
		}
		dValue *= pStockInfo->m_nPriceUnit;

		nQuarter = pFinanceData->GetDataByID(0); 
	}
	if (abs((int)nQuarter - 1) < 0.0001)
		nQuarter = 0.25;
	else if (abs((int)nQuarter - 2) < 0.0001)
		nQuarter = 0.5;
	else if (abs((int)nQuarter - 3) < 0.0001)
		nQuarter = 0.75;
	else
		nQuarter = 1;

	if( dValue > 0 && pRealTime->m_lNewPrice != 0 )
	{
		CGeneralHelper::LongToString(pRealTime->m_lNewPrice/dValue*pStockInfo->m_nPriceUnit*nQuarter, 0, pStockInfo->m_nPriceUnit, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	}
	else
	{
		CGeneralHelper::LongToString((long)0,0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	}

	return dValue;
}

double CHSCalculate::GetShiying(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL || pFinanceData == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	double nQuarter = 4; 
	if( pFinanceData)
	{
		HSDouble dRet = pFinanceData->GetDataByID(COLUMN_FINANCE_PER_INCOME);
		if ( dRet.m_dData != hx_DefMaxValues && dRet.m_dData != 0 )
		{
			dValue = dRet;
		}
		else
		{
			dValue = 0;
		}
		dValue *= pStockInfo->m_nPriceUnit;

		nQuarter = pFinanceData->GetDataByID(0); 
	}
	if (abs((int)nQuarter - 1) < 0.0001)
		nQuarter = 0.25;
	else if (abs((int)nQuarter - 2) < 0.0001)
		nQuarter = 0.5;
	else if (abs((int)nQuarter - 3) < 0.0001)
		nQuarter = 0.75;
	else
		nQuarter = 1;

	if( dValue > 0 && pLevel->m_lNewPrice != 0 )
	{
		CGeneralHelper::LongToString(pLevel->m_lNewPrice/dValue*pStockInfo->m_nPriceUnit*nQuarter, 0, pStockInfo->m_nPriceUnit, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	}
	else
	{
		CGeneralHelper::LongToString((long)0,0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO);
	}

	return dValue;
}

double CHSCalculate::Getliutong(CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pFinanceData == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if (pFinanceData)
	{
		HSDouble dRet = pFinanceData->GetDataByID(COLUMN_CAPITALIZATION_PASS_A);
		dValue = dRet.m_dData;
		dValue *= 10000;
	}
	CGeneralHelper::LongToString(dValue, 0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetShizhi( const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pRealTime == NULL || pFinanceData == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if (pFinanceData)
	{
		HSDouble dRet = pFinanceData->GetDataByID(COLUMN_CAPITALIZATION_PASS_A);
		dValue = dRet.m_dData * pRealTime->m_lNewPrice;
	}
	CGeneralHelper::LongToString(dValue*10, 0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);
	return dValue;
}

double CHSCalculate::GetShizhi(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pLevel == NULL || pFinanceData == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if (pFinanceData)
	{
		HSDouble dRet = pFinanceData->GetDataByID(COLUMN_CAPITALIZATION_PASS_A);
		dValue = dRet.m_dData * pLevel->m_lNewPrice;
	}
	CGeneralHelper::LongToString(dValue*10, 0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);
	return dValue;
}

double CHSCalculate::GetWeicha( const HSStockRealTime* pRealTime, int nHand, char strText[64] )
{
	double dValue = 0.0;
	if (pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long BuyCountTotal  =  pRealTime->m_lBuyCount1 + pRealTime->m_lBuyCount2 + 
		pRealTime->m_lBuyCount3 + pRealTime->m_lBuyCount4 + pRealTime->m_lBuyCount5;
	long SellCountTotal =  pRealTime->m_lSellCount1 + pRealTime->m_lSellCount2 + 
		pRealTime->m_lSellCount3 + pRealTime->m_lSellCount4 + pRealTime->m_lSellCount5;
	long A = BuyCountTotal - SellCountTotal;
	dValue = (double)A / (double)nHand;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetWeicha(const LevelRealTime* pLevel, int nHand, char strText[64])
{
	double dValue = 0.0;
	if (pLevel == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long BuyCount  =  pLevel->m_lBuyCount1 + pLevel->m_lBuyCount2 + 
		pLevel->m_lBuyCount3 + pLevel->m_lBuyCount4 + pLevel->m_lBuyCount5 +
		pLevel->m_lBuyCount6 + pLevel->m_lBuyCount7 + pLevel->m_lBuyCount8 +
		pLevel->m_lBuyCount9 + pLevel->m_lBuyCount10;

	long SellCount =  pLevel->m_lSellCount1 + pLevel->m_lSellCount2 + 
		pLevel->m_lSellCount3 + pLevel->m_lSellCount4 + pLevel->m_lSellCount5 +
		pLevel->m_lSellCount6 + pLevel->m_lSellCount7 + pLevel->m_lSellCount8 +
		pLevel->m_lSellCount9 + pLevel->m_lSellCount10;

	long A = BuyCount - SellCount;
	dValue = (double)A / (double)nHand;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetWeicha(const HSIndexRealTime* pRealTime, int nHand, char strText[64])
{
	double dValue = 0.0;
	if (pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long A =  pRealTime->m_lBuyCount - pRealTime->m_lSellCount;
	dValue = (double)A / (double)nHand;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetJunjia(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64] )
{
	double dValue = 0.0;
	if (pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	HSMarketDataType cCodeType = pStockInfo->m_ciStockCode.m_cCodeType;				
	if(pRealTime->m_lTotal != 0)
	{
		dValue = ((double)pRealTime->m_fAvgPrice * pStockInfo->m_nPriceUnit / ((double)pRealTime->m_lTotal * 100 / pStockInfo->m_nHandNum));
		if( (MakeMarket(cCodeType) == STOCK_MARKET) && 
			(MakeSubMarket(cCodeType) == KIND_BOND) )
		{//�����г���������ʵ������
			if (MakeMainMarket(cCodeType) == SH_Bourse)	
			{
				dValue *= 1;
			} 
			else
			{
				dValue *= 10;
			}
		}
		else if (MakeSubMarket(cCodeType) == KIND_QuanZhen)
		{// �����Ȩ֤
			dValue /= 100;
		}
	}
	CGeneralHelper::LongToString(dValue, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetJunjia(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64])
{
	double dValue = 0.0;
	if (pLevel == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	HSMarketDataType cCodeType = pStockInfo->m_ciStockCode.m_cCodeType;				
	if(pLevel->m_lTotal != 0)
	{
		dValue = ((double)pLevel->m_fAvgPrice * pStockInfo->m_nPriceUnit / ((double)pLevel->m_lTotal  * 100 / pStockInfo->m_nHandNum));
		if( (MakeMarket(cCodeType) == STOCK_MARKET) && 
			(MakeSubMarket(cCodeType) == KIND_BOND) )
		{//�����г���������ʵ������
			if (MakeMainMarket(cCodeType) == SH_Bourse)	
			{
				dValue *= 1;
			} 
			else
			{
				dValue *= 10;
			}
		}
		else if (MakeSubMarket(cCodeType) == KIND_QuanZhen)
		{// �����Ȩ֤
			dValue /= 100;
		}
	}
	CGeneralHelper::LongToString(dValue, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetLiangbi( const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, const StockOtherData* pOther, IDataSourceEx* pDataSource, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL || pOther == NULL || pDataSource == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = pStockInfo->m_l5DayVol;
	if (dValue > 0)
	{
		CArray<StockType> ayStockType;
		WORD wMarket = pStockInfo->m_ciStockCode.m_cCodeType;
		long nCount = pDataSource->HSDataSourceEx_GetSystemMarketInfo(&wMarket, 1, ayStockType);
		if (nCount == 1)
		{
			short nTotalTime = ayStockType.GetAt(0).m_nTotalTime + 1;
			double dWBTemp = (double)((double long)pRealTime->m_lTotal * nTotalTime / 
				((double long)pStockInfo->m_l5DayVol * (pOther->m_nTime+1)));
			dValue = dWBTemp;
// 			if( dWBTemp > 0 )
// 			{
// 				dValue = (long)(dWBTemp+0.5);
// 			}
// 			else
// 			{
// 				dValue = (long)(dWBTemp-0.5);
// 			}
		}
	}
	CGeneralHelper::LongToString(dValue, 0, 100, 2, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetLiangbi(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, const LevelStockOtherData* pLevelOther, IDataSourceEx* pDataSource, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL || pLevelOther == NULL || pDataSource == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = pStockInfo->m_l5DayVol;
	if (dValue > 0)
	{
		CArray<StockType> ayStockType;
		WORD wMarket = pStockInfo->m_ciStockCode.m_cCodeType;
		long nCount = pDataSource->HSDataSourceEx_GetSystemMarketInfo(&wMarket, 1, ayStockType);
		if (nCount == 1)
		{
			short nTotalTime = ayStockType.GetAt(0).m_nTotalTime + 1;
			double dWBTemp = (double)((double long)pLevel->m_lTotal * nTotalTime / 
				((double long)pStockInfo->m_l5DayVol * (pLevelOther->m_nTime+1)));
			dValue = dWBTemp;
			// 			if( dWBTemp > 0 )
			// 			{
			// 				dValue = (long)(dWBTemp+0.5);
			// 			}
			// 			else
			// 			{
			// 				dValue = (long)(dWBTemp-0.5);
			// 			}
		}
	}
	CGeneralHelper::LongToString(dValue, 0, 100, 2, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetHuanshou( const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	CodeInfo* pCodeInfo = (CodeInfo* )&pStockInfo->m_ciStockCode;
	HSDouble dRet = pFinanceData->CAPITAL(pCodeInfo);
	if ( dRet.m_dData != hx_DefMaxValues && dRet.m_dData != 0 )
	{
		dValue = dRet;
	}
	else
	{
		dValue = 0;
	}

	if( dValue > 10000 )
	{
		dValue = pRealTime->m_lTotal / (dValue / 10000 * pStockInfo->m_nHandNum);
	}
	else if( dValue != 0 )
	{
		dValue = pRealTime->m_lTotal / (dValue * pStockInfo->m_nHandNum);
	}

	CGeneralHelper::LongToString(dValue, 0, 100, 2, strText, LTOA_MASK_SHOW_PERSENT);
	return dValue;
}

double CHSCalculate::GetHuanshou(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	CodeInfo* pCodeInfo = (CodeInfo* )&pStockInfo->m_ciStockCode;
	HSDouble dRet = pFinanceData->CAPITAL(pCodeInfo);
	if ( dRet.m_dData != hx_DefMaxValues && dRet.m_dData != 0 )
	{
		dValue = dRet;
	}
	else
	{
		dValue = 0;
	}

	if( dValue > 10000 )
	{
		dValue = pLevel->m_lTotal / (dValue / 10000 * pStockInfo->m_nHandNum);
	}
	else if( dValue != 0 )
	{
		dValue = pLevel->m_lTotal / (dValue * pStockInfo->m_nHandNum);
	}

	CGeneralHelper::LongToString(dValue, 0, 100, 2, strText, LTOA_MASK_SHOW_PERSENT);
	return dValue;
}

double CHSCalculate::GetQuanjia(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if( pRealTime->m_lNewPrice != 0 )
	{
		dValue = pRealTime->m_lNationalDebtRatio * pStockInfo->m_nPriceUnit / 1000 + pRealTime->m_lNewPrice;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	}
	else 
	{// �����¼�Ϊ0ʱ��ȫ��Ϊ��Ϣ��������
		dValue = pRealTime->m_lNationalDebtRatio * pStockInfo->m_nPriceUnit / 1000 + pStockInfo->m_lPrevClose;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	}
	return dValue;
}

double CHSCalculate::GetQuanjia(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL)
	{
		memset(strText, 0, STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if( pLevel->m_lNewPrice != 0 )
	{
		dValue = pLevel->m_IPOV + pLevel->m_lNewPrice;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO); 
	}
	else 
	{// �����¼�Ϊ0ʱ��ȫ��Ϊ��Ϣ��������
		dValue = pLevel->m_IPOV + pStockInfo->m_lPrevClose;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);  
	}
	return dValue;
}

double CHSCalculate::GetShijing( const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pRealTime == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	HSDouble dRet = pFinanceData->GetDataByID(COLUMN_FINANCE_PER_ASSETS);
	if (dRet > 0)
	{
		dValue = pRealTime->m_lNewPrice / dRet;
	}
	CGeneralHelper::LongToString(dValue, 0, 1000, 2, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetShijing(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pLevel == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	HSDouble dRet = pFinanceData->GetDataByID(COLUMN_FINANCE_PER_ASSETS);
	if (dRet > 0)
	{
		dValue = pLevel->m_lNewPrice / dRet;
	}
	CGeneralHelper::LongToString(dValue, 0, 1000, 2, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetZonggu( CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = pFinanceData->GetDataByID(COLUMN_CAPITALIZATION_TOTAL);
	CGeneralHelper::LongToString(dValue*10000, 0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetZongzhi( const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] )
{
	double dValue = 0.0;
	if (pRealTime == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	HSDouble dRet = pFinanceData->GetDataByID(COLUMN_CAPITALIZATION_TOTAL);
	dValue = dRet.m_dData * pRealTime->m_lNewPrice;
	CGeneralHelper::LongToString(dValue * 10, 0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetZongzhi(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64])
{
	double dValue = 0.0;
	if (pLevel == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	HSDouble dRet = pFinanceData->GetDataByID(COLUMN_CAPITALIZATION_TOTAL);
	dValue = dRet.m_dData * pLevel->m_lNewPrice;
	CGeneralHelper::LongToString(dValue * 10, 0, 1, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetWaipan(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pOther == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}
	
	dValue = pOther->m_lOutside;
	CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nHandNum, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetWaipan(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevelOther == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = pLevelOther->m_lOutside;
	CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nHandNum, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetNeipan( const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pOther == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = pOther->m_lInside;
	CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nHandNum, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetNeipan(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevelOther == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = pLevelOther->m_lInside;
	CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nHandNum, 2, strText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_SHOWEX_UNITAUTO);

	return dValue;
}

double CHSCalculate::GetIOPV( const StockUserInfo* pStockInfo, const HSStockRealTime_Ext* pRealTimeExt, char strText[64] )
{
	double dValue = 0.0;
	if (pRealTimeExt == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if ((MakeMidMarket(pStockInfo->m_ciStockCode.m_cCodeType) == SZ_BOURSE))
		CGeneralHelper::LongToString(pRealTimeExt->m_stockOther.m_lExt1/100, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	else
		CGeneralHelper::LongToString(pRealTimeExt->m_stockOther.m_lExt1, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);

	return dValue;
}

double CHSCalculate::GetJingZhi(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if ((MakeMidMarket(pStockInfo->m_ciStockCode.m_cCodeType) == SZ_BOURSE) &&
		(MakeSubMarket(pStockInfo->m_ciStockCode.m_cCodeType) == KIND_LOF) && (pRealTime->m_lNationalDebtRatio> 0.01))
		CGeneralHelper::LongToString(pRealTime->m_lNationalDebtRatio/100, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	else
	{
		HSDouble dRet = pFinanceData->GetDataByID(COLUMN_FINANCE_PER_ASSETS);
		double fRatCur = dRet.m_dData ;
		CGeneralHelper::LongToString(fRatCur, 0, 1, 3, strText, LTOA_MASK_HORLINE_IFZERO);
	}

	return dValue;
}

double CHSCalculate::GetJingZhi(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL || pFinanceData == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if ((MakeMidMarket(pStockInfo->m_ciStockCode.m_cCodeType) == SZ_BOURSE) &&
		(MakeSubMarket(pStockInfo->m_ciStockCode.m_cCodeType) == KIND_LOF) && (pLevel->m_IPOV > 0.01))
		CGeneralHelper::LongToString(pLevel->m_IPOV/100, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	else
	{
		HSDouble dRet = pFinanceData->GetDataByID(COLUMN_FINANCE_PER_ASSETS);
		double fRatCur = dRet.m_dData ;
		CGeneralHelper::LongToString(fRatCur, 0, 1, 3, strText, LTOA_MASK_HORLINE_IFZERO);
	}

	return dValue;
}

double CHSCalculate::GetFanHuiLv( const StockUserInfo* pStockInfo, const HSStockRealTime_Ext* pRealTimeExt, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTimeExt == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}


	if( pRealTimeExt->m_stockRealTime.m_lNewPrice == 0 )
	{
		dValue = 0.0;
	}
	else
	{
		dValue = 1.0*pStockInfo->m_nPriceUnit*pStockInfo->m_nPriceUnit/pRealTimeExt->m_stockRealTime.m_lNewPrice;
	}
	CGeneralHelper::LongToString(dValue, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetFanHuiLv(const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pIndex == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if( pIndex->m_lNewPrice == 0 )
	{
		dValue = 0.0;
	}
	else
	{
		dValue = 1.0*pStockInfo->m_nPriceUnit*pStockInfo->m_nPriceUnit/pIndex->m_lNewPrice;
	}
	CGeneralHelper::LongToString(dValue, pStockInfo->m_lPrevClose, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetZhangSu( const StockUserInfo* pStockInfo, const HSStockRealTimeOther* pRealOther, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealOther == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = pRealOther->m_fSpeedUp;
	CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_nPriceUnit, pStockInfo->m_nDecimal, strText, LTOA_MASK_FIXEDLEN_IFZERO | LTOA_MASK_SHOW_PERSENT);
	return dValue;
}

double CHSCalculate::GetZhenFu( const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pRealTime == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if( pStockInfo->m_lPrevClose <= 0 )
	{
		CGeneralHelper::LongToString(dValue, 0, 0, 2, strText, LTOA_MASK_FIXEDLEN_IFZERO);
	}
	else
	{
		dValue = (pRealTime->m_lMaxPrice - pRealTime->m_lMinPrice) * 100;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_lPrevClose, 2, strText, LTOA_MASK_FIXEDLEN_IFZERO | LTOA_MASK_SHOW_PERSENT);
	}
	return dValue;
}

double CHSCalculate::GetZhenFu(const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64])
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pIndex == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	if( pStockInfo->m_lPrevClose <= 0 )
	{
		CGeneralHelper::LongToString(dValue, 0, 0, 2, strText, LTOA_MASK_FIXEDLEN_IFZERO);
	}
	else
	{
		dValue = (pIndex->m_lMaxPrice - pIndex->m_lMinPrice) * 100;
		CGeneralHelper::LongToString(dValue, 0, pStockInfo->m_lPrevClose, 2, strText, LTOA_MASK_FIXEDLEN_IFZERO | LTOA_MASK_SHOW_PERSENT);
	}
	return dValue;
}

double CHSCalculate::GetMeiBi(const LevelRealTime* pLevel, char strText[64])
{
	double dValue = 0.0;
	if (pLevel == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	dValue = (double)pLevel->m_lTotal / (double)pLevel->m_lTickCount;
	CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	return dValue;
}

double CHSCalculate::GetSellAvg(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long lClose = pStockInfo->m_lPrevClose;
	int nUnit = pStockInfo->m_nPriceUnit;
	int nDecimal = pStockInfo->m_nDecimal;
	
	dValue = max(pLevel->WeightedAvgOfferPx, pLevel->AltWeightedAvgOfferPx);
	CGeneralHelper::LongToString(dValue, lClose, 1, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	
	return dValue;
}

double CHSCalculate::GetBuyAvg(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64] )
{
	double dValue = 0.0;
	if (pStockInfo == NULL || pLevel == NULL)
	{
		memset(strText, 0 , STR_LENTH);
		strcpy(strText,  _T(" -"));
		return dValue;
	}

	long lClose = pStockInfo->m_lPrevClose;
	int nUnit = pStockInfo->m_nPriceUnit;
	int nDecimal = pStockInfo->m_nDecimal;

	dValue = max(pLevel->WeightedAvgBidPx, pLevel->AltWeightedAvgBidPx);
	CGeneralHelper::LongToString(dValue, lClose, 1, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
	
	return dValue;
}


CString CHSCalculate::CreateHtml(const CString& strContent, const InfoIndex* pInfoIndex)
{
	if (pInfoIndex == NULL)
	{
		return "";
	}

	// ����ʽ�ļ�
	CString strPath = CGeneralHelper::GetAppPath() + _T("face\\html\\") + HTMLFORMAT_PATH;
	CFile  fRead(strPath, CFile::modeRead); 
    long flen = (long)fRead.GetLength(); 
	if (flen <= 0)
	{
		return "";
	}
	char*  pBuffer = new char[flen]; 
	fRead.Read(pBuffer,flen); 
	CString strFotmat(pBuffer, flen);
	delete[] pBuffer;
	fRead.Close();

	//////////////////////////////////////////////////////////////////////////
	// �γ�html�ļ�
	// ����
	strFotmat.Replace(HTML_REPLACE_TITLE, pInfoIndex->m_cTitle);
	// ����
	// �����з��滻��<\br>
	CString strFormatContent(strContent, strContent.GetLength());
	strFormatContent.Replace("\r\n", "</br>&nbsp&nbsp");
	strFotmat.Replace(HTML_REPLACE_CONTENT, strContent);
	// ʱ��
	CString strTime;
	strTime.Format("%04d-%02d-%02d ", pInfoIndex->m_lDate/10000, (pInfoIndex->m_lDate%10000)/100, pInfoIndex->m_lDate%100);
	strTime.AppendFormat("%02d:%02d:%02d", pInfoIndex->m_lTime/10000, (pInfoIndex->m_lTime%10000)/100, pInfoIndex->m_lTime%100);
	strFotmat.Replace(HTML_REPLACE_TIME, strTime);
	// ����
	CString strAuthor = "";
	strFotmat.Replace(HTML_REPLACE_AUTHOR, strAuthor);
	// ��Դ
	CString strOrigin = "";
	strFotmat.Replace(HTML_REPLACE_ORIGIN, strOrigin);
	//////////////////////////////////////////////////////////////////////////

	// дhtml�ļ�
	strPath = CGeneralHelper::GetAppPath() + _T("face\\html\\") + HTMLCONTENT_PATH;
	CFile fWrite(strPath, CFile::modeCreate | CFile::modeWrite);
	fWrite.Write(strFotmat.GetBuffer(), strFotmat.GetLength());
	fWrite.Close();

	return strPath;
}

CTime CHSCalculate::GetTimer(const StockUserInfo* pStockInfo, const StockTick* pTick, StockType* pTypeInfo, BOOL bIsFullTime, long lDate, long lPreTime, char strText[64] )
{
	CTime tm(0);
	if (pStockInfo == NULL || pTick == NULL || pTypeInfo == NULL)
	{
		tm = 0;
		memset(strText, 0 , STR_LENTH);
		return tm;
	}

	WORD wCodeType = pStockInfo->m_ciStockCode.m_cCodeType;
	int nSize = sizeof(pTypeInfo->m_union.m_nTimes) / sizeof(HSTypeTime) + 2;
	HSTypeTime* pTypeTime = (HSTypeTime*)&pTypeInfo->m_union.m_nAheadOpenTime;

	int nCount = 0;
	int nPreCount = 0;
	int nCurTimePos = pTick->m_nTime;
	int nSecond = pTick->m_sDetailTime.m_nSecond;
	for( int i = 0; i < nSize; i++)
	{
		nCount += nPreCount;
		if( nCurTimePos  <= (pTypeTime[i].m_nCloseTime - pTypeTime[i].m_nOpenTime + nCount) )
		{
			nCurTimePos -= nCount;
			nCurTimePos  = pTypeTime[i].m_nOpenTime + nCurTimePos;
			tm = ConstructTime( lDate / 10000, lDate % 10000 / 100, lDate % 10000 % 100 , 
				nCurTimePos/60, nCurTimePos%60, nSecond);
			break;
		}
		nPreCount = pTypeTime[i].m_nCloseTime - pTypeTime[i].m_nOpenTime;
	}
	
	if (bIsFullTime)
	{
		if( (SZ_BOURSE & wCodeType) && tm.GetHour() == 15)
		{
			sprintf(strText, "%02d:%02d:%02d", tm.GetHour(), tm.GetMinute(), pTick->m_sDetailTime.m_nSecond);
		}
		else
		{
			if(tm.GetMinute() < 1)
				sprintf(strText, "%02d:%02d:%02d", tm.GetHour() - 1, tm.GetMinute() - 1 + 60, pTick->m_sDetailTime.m_nSecond);
			else
				sprintf(strText, "%02d:%02d:%02d", tm.GetHour(), tm.GetMinute() - 1, pTick->m_sDetailTime.m_nSecond);
		}
	}
	else if(lPreTime == pTick->m_nTime)
	{// Ϊ�����System���� ��������������� �ո���������в�ͬ
		sprintf(strText, "%-9s:%02d", "", pTick->m_sDetailTime.m_nSecond);
	}
	else
	{
		if( (SZ_BOURSE & wCodeType) && tm.GetHour() == 15)
		{
			sprintf(strText, "%02d:%02d:%02d", tm.GetHour(), tm.GetMinute(), pTick->m_sDetailTime.m_nSecond);
		}
		else
		{
			if(tm.GetMinute() < 1)
				sprintf(strText, "%02d:%02d:%02d", tm.GetHour() - 1, tm.GetMinute() - 1 + 60, pTick->m_sDetailTime.m_nSecond);
			else
				sprintf(strText, "%02d:%02d:%02d", tm.GetHour(), tm.GetMinute() - 1, pTick->m_sDetailTime.m_nSecond);
		}
	}

	//20100305
	if (!_strnicmp(strText, "09:29", 5) && STOCK_MARKET == MakeMarket(wCodeType))
	{
		strncpy(strText, "09:25", 5);
	}
	
	return tm;
}

CTime CHSCalculate::ConstructTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,int nDST /*= -1*/ )
{
	if( !(nDay >= 1 && nDay <= 31) )
		nDay = 1;
	if( !(nMonth >= 1 && nMonth <= 12) )
		nMonth = 1;
	if( !(nYear >= 1970 && nYear <= 2038) )
	{
		CTime curTime = CTime::GetCurrentTime();
		nYear = curTime.GetYear();
	}
	return CTime( nYear,nMonth, nDay, nHour, nMin, nSec, nDST);
}

void  CHSCalculate::GetTextByID(int nID,  CurrentFinanceData* pFinanceData, char strText[64])
{
	if (pFinanceData == NULL)
	{
		memset(strText, 0, STR_LENTH);
		return;
	}

	float fValue = 0.0;
	double dValue = 0.0;
	CString str = "";
	fValue = GetDataByID(nID, pFinanceData);

	if (nID == COLUMN_FINANCE_REPORT_DATE)
	{
		fValue = GetDataByID(0, pFinanceData);
		if (abs(fValue - 1) < 0.0001)
			str = _T("һ����");
		else if (abs(fValue - 2) < 0.0001)
			str = _T("������");
		else if (abs(fValue - 3) < 0.0001)
			str = _T("������");
		else
			str = _T("�ļ���");
	}
	else
	{
		int	nUnit	= 10000;	
		CString	strUnit;
		AutoAdjustUnit(nID, fValue, strUnit, &nUnit);

		if ( !fValue )
		{
			str.Empty();
		}
		else
		{
			dValue = fValue / nUnit;
			str.Format(_T("%16.5f"), dValue);

			//////////////////////////////////////////////////////////////////////////
			//�������ʵ�������̬��ʽ���ı�
			TCHAR	ch;
			/*	����ȥ���ַ����к�����ܴ��ڵ�һ������0�ַ�*/
			str.TrimLeft();
			int	n	= str.GetLength() - 1;
			while(1)
			{
				ch	= str[n];
				if ( ch == '0' )
				{
					//	�ÿո����
					str.SetAt(n,' ');
					n --;
				}
				else
				{
					//	ȥ���ұߵ�һ�����ո�
					str.TrimRight();
					break;
				}
			}
			//	����ı�����ˡ�0.�������ĸ�ʽ��ֱ���ÿո����
			if ( str == _T("0."))
			{
				str.Empty() ;
			}
			//	����ı�����С������������ȫ���ǿո���ȥ��
			//	�����С����
			else if ( str[str.GetLength() - 1] == _T('.'))
			{
				str	= str.Left(str.GetLength() - 1);
			}
			//	���С�������ĳ���̫�����Ǿ�ȡ��С�������ڶ�λΪֹ���ɡ�
			if ( dValue * 100 > 1 || dValue * 100 < -1 )
			{
				int	nDot	= str.Find(_T('.'));
				if ( nDot != -1 )
				{
					//ÿ�������ÿ������ʱ������4λС��--------------------
					if ((COLUMN_FINANCE_PER_INCOME == nID) || (COLUMN_FINANCE_PER_UNPAID == nID) ||
						(COLUMN_FINANCE_PER_ASSETS == nID) || (COLUMN_FINANCE_ADJUST_PER_ASSETS == nID) )
						str	= str.Left(nDot + 5);
					else
						str	= str.Left(nDot + 3);
				}
				//	����µõ����ı�ΪXXX.00��ʽ��ȥ�������.00
				if ( str.Right(3) == _T(".00"))
				{
					str	= str.Left(str.GetLength() - 3);
				}
			}
		}
		if (dValue && !str.IsEmpty() )
		{
			str	+= strUnit;
		}
	}
	strncpy(strText, str.GetBuffer(), min(str.GetLength()+1, STR_LENTH));
}

float CHSCalculate::GetDataByID( int nID, CurrentFinanceData* pFinanceData)
{
	if ( nID >= COLUMN_CAPITALIZATION_TOTAL && nID <= COLUMN_CAPITALIZATION_A2_GIVE )
	{
		nID = nID - COLUMN_CAPITALIZATION_TOTAL;
		return pFinanceData->m_fFinanceData[2+nID];
	}
	else if ( nID >= COLUMN_FINANCE_TOTAL_ASSETS && nID <= COLUMN_FINANCE_ASSETS_YIELD )
	{
		nID = COLUMN_CAPITALIZATION_A2_GIVE - COLUMN_CAPITALIZATION_TOTAL + 1 + nID - COLUMN_FINANCE_TOTAL_ASSETS;
		return pFinanceData->m_fFinanceData[2+nID];
	}
	else if ( nID >= 0 && nID < sizeof(pFinanceData->m_fFinanceData)) 
	{
		return pFinanceData->m_fFinanceData[nID];
	}
	return 0.0;
}


void CHSCalculate::AutoAdjustUnit(WORD wKey, double dbData, CString &strUnit, int *pUnit)
{
	ASSERT(pUnit && AfxIsValidAddress(pUnit, sizeof(int), TRUE));

	//	���ȶ��ڹɱ������
	if( wKey >=COLUMN_CAPITALIZATION_TOTAL 
		&& wKey <= COLUMN_CAPITALIZATION_A2_GIVE )
	{
		if ( dbData >= 10000 || dbData <= -10000 )
		{
			*pUnit	= 10000;
			strUnit	= _T("��");
		}
		else
		{
			*pUnit	= 1;
			strUnit	= _T("��");
		}
	}

	//	���ڽ�������
	else if ( wKey == COLUMN_FINANCE_TOTAL_ASSETS		//	���ʲ�

		|| wKey == COLUMN_FINANCE_CURRENT_ASSETS		//	�����ʲ�
		|| wKey == COLUMN_FINANCE_CAPITAL_ASSETS		//	�̶��ʲ�
		|| wKey == COLUMN_FINANCE_UNBODIED_ASSETS		//	�����ʲ�
		|| wKey == COLUMN_FINANCE_LONG_INVESTMENT		//	����Ͷ��
		|| wKey == COLUMN_FINANCE_CURRENT_LIABILITIES	//	������ծ

		|| wKey == COLUMN_FINANCE_LONG_LIABILITIES		//	���ڸ�ծ

		|| wKey == COLUMN_FINANCE_CAPITAL_ACCFUND		//	�ʱ�������
		|| wKey == COLUMN_FINANCE_PARTNER_RIGHT			//	�ɶ�Ȩ��
		|| wKey == COLUMN_FINANCE_MAIN_INCOME			//	��Ӫ����
		|| wKey == COLUMN_FINANCE_MAIN_PROFIT			//	��Ӫ����
		|| wKey == COLUMN_FINANCE_OTHER_PROFIT			//	��������

		|| wKey == COLUMN_FINANCE_TAKING_PROFIT			//	Ӫҵ����
		|| wKey == COLUMN_FINANCE_YIELD					//	Ͷ������
		|| wKey == COLUMN_FINANCE_SUBSIDY				//	��������
		|| wKey == COLUMN_FINANCE_OTHER_INCOME			//	Ӫҵ������
		|| wKey == COLUMN_FINANCE_LAST_PROFIT_LOSS		//	�����������

		|| wKey == COLUMN_FINANCE_TOTAL_PROFIT			//	�����ܶ�
		|| wKey == COLUMN_FINANCE_SCOT_PROFIT			//	˰������
		|| wKey == COLUMN_FINANCE_RETAINED_PROFITS		//	������
		|| wKey == COLUMN_FINANCE_UNPAID_PROFIT			//	δ��������
		)
	{
		if ( dbData >= 100000 || dbData <= -100000 )
		{
			*pUnit	= 100000;
			strUnit	= _T("��");
		}
		else
		{
			*pUnit	= 10;
			strUnit	= _T("��");
		}
	}
	//	����ÿ���йص���
	else if ( wKey == COLUMN_FINANCE_PERSTOCK_ACCFUND	//	ÿ�ɹ�����
		|| wKey == COLUMN_FINANCE_PER_UNPAID			//	ÿ��δ����
		|| wKey == COLUMN_FINANCE_PER_INCOME			//	ÿ������
		|| wKey == COLUMN_FINANCE_PER_ASSETS			//	ÿ�ɾ��ʲ�
		|| wKey == COLUMN_FINANCE_ADJUST_PER_ASSETS )	//	����ÿ�ɾ��ʲ�
	{
		*pUnit	= 1;
		strUnit	= _T("Ԫ");
	}
	//	���ں���%������˵
	else if ( wKey == COLUMN_FINANCE_PARTNER_RIGHT_RATIO//	�ɶ�Ȩ���
		|| wKey == COLUMN_FINANCE_ASSETS_YIELD )		//	���ʲ�������
	{
		*pUnit	= 1;
		strUnit	= "��";
	}
	else
	{
		*pUnit	= 1;
		strUnit	= _T("Ԫ");
	}
}

