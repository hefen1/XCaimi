#include "stdafx.h"
#include "CombCacheImp.h"
#include "WinnerApplication.h"

BOOL  CCombCacheSet::GetStockPriceUnitDecimal(CString strCode, short& nPriceUnit, short& nDecimal)
{
	nPriceUnit = 0;
	nDecimal = 0;
	StockUserInfo* pStkInfo = NULL;
	if ( m_mapStkInfoCache.Lookup(strCode,(CObject*&)pStkInfo) )
	{
		nPriceUnit = pStkInfo->m_nPriceUnit;
		nDecimal = pStkInfo->m_nDecimal;
	} 
	if( 0 == nPriceUnit)
		nPriceUnit = Default_Price_Unit;
	if(0 == nDecimal )
		nDecimal = Default_Decimal;
	return TRUE;
}

double CCombCacheSet::GetRealTimeValue(CString strCode,ValueFlag vf)
{
	//加锁保护
	CSingleLock singleLock(&m_critSection,TRUE);

	CodeShareRealTime* pCodeShareRealData = NULL;
	if( m_mapQuoteCache.Lookup(strCode,(CObject*&)pCodeShareRealData) == FALSE )
	{
		CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::GetRealTimeValue]取实时行情信息失败.代码[%s]."),strCode);
		return Default_Result_Value;
	}

	if( vf == VF_NewPrice)
	{
		double dResult = 0;
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult = pCodeShareRealData->m_shareRealTime.m_qhData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult = pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lNewPrice / (double)Default_Price_Unit;

		//早上开市前:市场初始化后是取不到最新价的，所以取不到最新价时取昨收
		if(dResult < 0.000001)
			dResult = this->GetClose(strCode);
		return dResult;
	}
	if( vf == VF_Open)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			return pCodeShareRealData->m_shareRealTime.m_indData.m_lOpen / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			return pCodeShareRealData->m_shareRealTime.m_qhData.m_lOpen / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			return pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lOpen / (double)Default_Price_Unit;
	}
	else if( vf == VF_NationalDebtRatio)
	{
		if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			return pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lNationalDebtRatio / (double)100000;	
	}
	return Default_Result_Value;
}

double CCombCacheSet::GetPrice(CString strCode)
{
	double dValue = GetRealTimeValue(strCode,VF_NewPrice);
	return dValue;
}

double CCombCacheSet::GetOpen(CString strCode)
{
	double dValue = GetRealTimeValue(strCode,VF_Open);
	return dValue;
}

double CCombCacheSet::GetNetValue(CString strCode)
{
	double dValue = GetRealTimeValue(strCode,VF_NationalDebtRatio);
	return dValue;
}

double CCombCacheSet::GetClose(CString strCode)
{
	StockUserInfo* pStkInfo = NULL;
	if( m_mapStkInfoCache.Lookup(strCode,(CObject*&)pStkInfo) == FALSE )
		return 0;
	return pStkInfo->m_lPrevClose / (double)Default_Price_Unit;
}

double CCombCacheSet::GetBPrice(CString strCode,int nFlag)
{
	//加锁保护
	CSingleLock singleLock(&m_critSection,TRUE);

	CodeShareRealTime* pCodeShareRealData = NULL;
	if( m_mapQuoteCache.Lookup(strCode,(CObject*&)pCodeShareRealData) == FALSE )
	{
		CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::GetBPrice]取实时行情信息失败.代码[%s]."),strCode);
		return Default_Result_Value;
	}

	double dResult = 0;
	if(nFlag == 1)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lBuyPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lBuyPrice1 / (double)Default_Price_Unit;
	}
	else if(nFlag == 2)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lBuyPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lBuyPrice2 / (double)Default_Price_Unit;
	}
	else if(nFlag == 3)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lBuyPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lBuyPrice3 / (double)Default_Price_Unit;
	}
	else if(nFlag == 4)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lBuyPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lBuyPrice4 / (double)Default_Price_Unit;
	}
	else if(nFlag == 5)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lBuyPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lBuyPrice5 / (double)Default_Price_Unit;
	}

	if(dResult < 0.000001)
		dResult = this->GetClose(strCode);
	return dResult;
}

double CCombCacheSet::GetSPrice(CString strCode,int nFlag)
{
	//加锁保护
	CSingleLock singleLock(&m_critSection,TRUE);

	CodeShareRealTime* pCodeShareRealData = NULL;
	if( m_mapQuoteCache.Lookup(strCode,(CObject*&)pCodeShareRealData) == FALSE )
	{
		CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::GetSPrice]取实时行情信息失败.代码[%s]."),strCode);
		return Default_Result_Value;
	}

	double dResult = 0;

	if(nFlag == 1)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lSellPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lSellPrice1 / (double)Default_Price_Unit;
	}
	else if(nFlag == 2)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lSellPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lSellPrice2 / (double)Default_Price_Unit;
	}
	else if(nFlag == 3)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lSellPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lSellPrice3 / (double)Default_Price_Unit;
	}
	else if(nFlag == 4)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lSellPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lSellPrice4 / (double)Default_Price_Unit;
	}
	else if(nFlag == 5)
	{
		if( MakeIndexMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) ) //指数代码	
			dResult =  pCodeShareRealData->m_shareRealTime.m_indData.m_lNewPrice / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//期货代码
			dResult =  pCodeShareRealData->m_shareRealTime.m_qhData.m_lSellPrice1 / (double)Default_Price_Unit;
		else if(MakeMarket(pCodeShareRealData->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//股票
			dResult =  pCodeShareRealData->m_shareRealTime.m_nowDataExt.m_stockRealTime.m_lSellPrice5 / (double)Default_Price_Unit;
	}

	if(dResult < 0.000001)
		dResult = this->GetClose(strCode);

	return dResult;
}

double CCombCacheSet::GetCombPrice(const char* code)
{
	CString strKey = code;
	CCombCache* pCache = NULL;
	if( m_mapCombCache.Lookup(strKey,(CObject*&)pCache ) )
	{
		return pCache->GetCombSZ();
	}
	else
	{
		CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::GetCombPrice]取组合市值失败.组合[%s]."),code);
		return Default_Result_Value;
	}
}

double CCombCacheSet::GetCombBPKSZ(const char* code)
{
	CString strKey = code;
	CCombCache* pCache = NULL;
	if( m_mapCombCache.Lookup(strKey,(CObject*&)pCache ) )
	{
		return pCache->GetCombBuyPKSZ();
	}
	else
	{
		CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::GetCombBPKSZ]取组合买盘口市值失败.组合[%s]."),code);
		return Default_Result_Value;
	}
}

double CCombCacheSet::GetCombSPKSZ(const char* code)
{
	CString strKey = code;
	CCombCache* pCache = NULL;
	if( m_mapCombCache.Lookup(strKey,(CObject*&)pCache ) )
	{
		return pCache->GetCombSellPKSZ();
	}
	else
	{
		CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::GetCombSPKSZ]取组合买盘口市值失败.组合[%s]."),code);
		return Default_Result_Value;
	}
}