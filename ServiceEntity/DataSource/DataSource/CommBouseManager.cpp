//////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "CommBourseManager.h"
#include "DataSourceImpl.h"
#include "DSWriteFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

CHsBouseManager::CHsBouseManager()
{
	m_pDataSource = NULL;
}

CHsBouseManager::~CHsBouseManager()
{
	RemoveAllBourse();
}

void CHsBouseManager::SetDataSourceImpl(CDataSourceImpl* pImpl)
{
	m_pDataSource = pImpl;
}

int	CHsBouseManager::GetBouserSize()
{
	int nRet = 0;
	CSingleLock lock(&m_mutexBourse);
	if (lock.Lock())
	{
		nRet = m_listBourseInfo.size();
		lock.Unlock();
	}
	return nRet;
}

//市场信息加入到队列
BOOL CHsBouseManager::PushBourse(BourseInfo* pBourse)
{
	CSingleLock lock(&m_mutexBourse);
	if (lock.Lock())
	{
		m_listBourseInfo.push_back(pBourse);
		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}
//获取市场信息 
BourseInfo*	CHsBouseManager::GetBourse(unsigned short nMarket)
{
	return NULL;
}
//删除某个市场信息
BOOL CHsBouseManager::RemoveBourse(BourseInfo* pBourse)
{
	return FALSE;
}
BOOL CHsBouseManager::RemoveBourse(HSMarketDataType nMarket )
{
	BOOL bRet = FALSE;
	CSingleLock lock(&m_mutexBourse);
	if (lock.Lock())
	{
		for (std::list<BourseInfo*>::iterator it = m_listBourseInfo.begin(); it != m_listBourseInfo.end(); it ++)
		{
			BourseInfo* pBourseInfo = (BourseInfo*)*it;
			if (pBourseInfo != NULL && (nMarket == pBourseInfo->m_nMarketType )) 
			{
				delete pBourseInfo;
				m_listBourseInfo.erase(it);
				bRet = TRUE;
				break;
			}
		}
		lock.Unlock();
		return bRet;
	}
	return FALSE;
}
//清空本地市场数据
BOOL CHsBouseManager::RemoveAllBourse()
{
	CSingleLock lock(&m_mutexBourse);
	if (lock.Lock())
	{
		while(m_listBourseInfo.size() > 0)
		{
			BourseInfo* pBourse = m_listBourseInfo.back();
			m_listBourseInfo.pop_back();
			if (pBourse != NULL)
			{
				delete pBourse;
				pBourse = NULL;
			}
		}
		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}

BOOL CHsBouseManager::LookupMainMarket(unsigned short* pMarket, int nMarketCount, unsigned short nMarketSource)
{
	if (pMarket == NULL && nMarketCount == 0)
	{
		return TRUE;
	}
	for (int i = 0; i < nMarketCount; i ++)
	{
		unsigned short* pMarketCur = pMarket + i;
		if (MakeMainMarket(*pMarketCur) == MakeMainMarket(nMarketSource))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CHsBouseManager::LookupSameMarket(unsigned short* pMarket, int nMarketCount, unsigned short nMarketSource)
{
	if (pMarket == NULL && nMarketCount == 0)
	{
		return TRUE;
	}
	for (int i = 0; i < nMarketCount; i ++)
	{
		unsigned short* pMarketCur = pMarket + i;
		if (*pMarketCur == nMarketSource)
		{
			return TRUE;
		}
	}
	return FALSE;
}

StockType* CHsBouseManager::GetStockType(unsigned short nMarketCode)
{
	StockType* pStockType = NULL;
	CSingleLock lock(&m_mutexBourse);
	if (lock.Lock())
	{
		for (std::list<BourseInfo*>::iterator it = m_listBourseInfo.begin(); it != m_listBourseInfo.end(); it ++)
		{
			BourseInfo* pBourseInfo = (BourseInfo*)*it;
			if (pBourseInfo != NULL)
			{
				if(nMarketCode & pBourseInfo->m_nMarketType)
				{
					for (int i = 0; i < STOCKTYPE_COUNT; i ++)
					{
						if (pBourseInfo->m_stNewType[i].m_stTypeName.m_szName[0] == '\0' && pBourseInfo->m_stNewType[i].m_nTotal == 0 && pBourseInfo->m_stNewType[i].m_nStockType == 0)
						{
							break;
						}
						if (nMarketCode == pBourseInfo->m_stNewType[i].m_nStockType && pBourseInfo->m_stNewType[i].m_nPriceUnit > 0
							&& pBourseInfo->m_stNewType[i].m_nTotal > 0 && pBourseInfo->m_stNewType[i].m_nOffset >= 0 
							&&pBourseInfo->m_stNewType[i].m_nTotal <= pBourseInfo->m_nTotalCount && pBourseInfo->m_stNewType[i].m_nOffset <= pBourseInfo->m_nTotalCount)
						{
							pStockType = &pBourseInfo->m_stNewType[i];
							break;
						}
					}
				}
			}
		}
		lock.Unlock();
	}
	return pStockType;
}

int CHsBouseManager::GetSystemMarketInfo(unsigned short* pMarket, int nMarketCount, CArray<StockType>& blockArray)
{
	CSingleLock lock(&m_mutexBourse);
	if (lock.Lock())
	{
		for (std::list<BourseInfo*>::iterator it = m_listBourseInfo.begin(); it != m_listBourseInfo.end(); it ++)
		{
			BourseInfo* pBourseInfo = (BourseInfo*)*it;
			if (pBourseInfo != NULL)
			{
				if(LookupMainMarket(pMarket, nMarketCount, pBourseInfo->m_nMarketType))
				{
					for (int i = 0; i < STOCKTYPE_COUNT; i ++)
					{
						if (pBourseInfo->m_stNewType[i].m_stTypeName.m_szName[0] == '\0' && pBourseInfo->m_stNewType[i].m_nTotal == 0 && pBourseInfo->m_stNewType[i].m_nStockType == 0)
						{
							break;
						}
						if (pBourseInfo->m_stNewType[i].m_nTotal >= 0 && pBourseInfo->m_stNewType[i].m_nOffset >= 0 
							&&pBourseInfo->m_stNewType[i].m_nTotal <= pBourseInfo->m_nTotalCount && pBourseInfo->m_stNewType[i].m_nOffset <= pBourseInfo->m_nTotalCount)
						{
							CString strMarketName = pBourseInfo->m_stNewType[i].m_stTypeName.m_szName;
							strMarketName.Trim();
							if (!strMarketName.IsEmpty() && LookupSameMarket(pMarket, nMarketCount, pBourseInfo->m_stNewType[i].m_nStockType))
							{
								StockType stockType;
								memcpy(&stockType, &pBourseInfo->m_stNewType[i], sizeof(StockType));
								blockArray.Add(stockType);
							}
						}
					}
				}
			}
		}
		lock.Unlock();
	}
	return blockArray.GetSize();
}

short CHsBouseManager::GetMarketBlockPriceUnit(unsigned short nMarketCode)
{
	StockType* pStockType = GetStockType(nMarketCode);
	if (pStockType != NULL)
	{
		return pStockType->m_nPriceUnit;
	}
	return RetainGetPriceUnit(nMarketCode);
}

short CHsBouseManager::RetainGetPriceUnit(unsigned short nMarketCode)
{
	return 100;
}

short CHsBouseManager::RetainGetPriceDecimal(unsigned short nMarketCode)
{
	if( MakeMarket(nMarketCode) == STOCK_MARKET/*SH_Bourse*/ )
	{
		if( (MakeMidMarket(nMarketCode) == SH_BOURSE && MakeSubMarket(nMarketCode)  == KIND_STOCKB) || 	//B股只有上海是3位小数
			MakeETF(nMarketCode) )
		{
			return 3;
		}
		else if( MakeSubMarket(nMarketCode)  == KIND_FUND   || 
			MakeSubMarket(nMarketCode)  == KIND_LOF	  )
		{
			return 3; //20081008 YJT 改为3，但是场外开放式基金可能会有问题
		}
	}
	else if( MakeMarket(nMarketCode)  == QH_Bourse )
	{
		if ((FUTURES_MARKET|SHANGHAI_BOURSE|KIND_QHGOLD) == nMarketCode)  //20091211 黄金期货价格相关项显示保留两位小数
		{
			return 2;
		}
		else if (nMarketCode == (FUTURES_MARKET|GUZHI_BOURSE|KIND_GUZHI))//yulx add 股指期货保留一位小数
		{
			return 1;
		}
		return 0;
	}
	else if( MakeMarket(nMarketCode) == (HSMarketDataType)WP_MARKET )
	{
		if( nMarketCode == (WP_MARKET | WP_NYMEX | 0x01) )
			return 3;
		else if( nMarketCode == (WP_MARKET | WP_NYMEX | 0x02) ||
			nMarketCode == (WP_MARKET | WP_NYMEX | 0x03) )
			return 4;
		else if( nMarketCode == (WP_MARKET | WP_NOBLE_METAL | 0x00) ||
			//nMarketCode == (WP_MARKET | WP_NOBLE_METAL | 0x01) ||
			nMarketCode == (WP_MARKET | WP_NOBLE_METAL | 0x02) )
			return 3;
		return 2;
	}
	else if( MakeMarket(nMarketCode) == (HSMarketDataType)FOREIGN_MARKET )
	{
		return 4;
	}
	else if( MakeMarket(nMarketCode) == HK_MARKET )
	{
		return 3;
	}
	return 2;
}

short CHsBouseManager::GetStockPriceDecimal(unsigned short nMarketCode)
{
	StockType* pStockType = GetStockType(nMarketCode);
	if( pStockType != NULL && pStockType->m_union.m_nPriceDecimal.m_nCloseTime > 0 )
		return pStockType->m_union.m_nPriceDecimal.m_nCloseTime;
	return RetainGetPriceDecimal(nMarketCode);

}

BOOL CHsBouseManager::GetStockPriceUnitDecimal(unsigned short nMarketCode, short& nPriceUnit, short& nDecimal)
{
	nPriceUnit = 0;
	nDecimal = 0;
	StockType* pStockType = GetStockType(nMarketCode);
	if( pStockType != NULL && pStockType->m_union.m_nPriceDecimal.m_nCloseTime > 0 )
		nDecimal = pStockType->m_union.m_nPriceDecimal.m_nCloseTime;
	else
		nDecimal = RetainGetPriceDecimal(nMarketCode);

	if (pStockType != NULL && pStockType->m_nPriceUnit > 0)
	{
		nPriceUnit = pStockType->m_nPriceUnit;
	}
	else
		nPriceUnit = RetainGetPriceUnit(nMarketCode);
	return FALSE;
}

BOOL CHsBouseManager::WriteLocalFile()
{
	CDSWriteFile* pWrite = NULL;
	if (m_pDataSource != NULL)
	{
		pWrite = m_pDataSource->GetDSWriterFile();
	}
	if (pWrite == NULL)
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	CSingleLock lock(&m_mutexBourse);
	if (lock.Lock())
	{
		for (std::list<BourseInfo*>::iterator it = m_listBourseInfo.begin(); it != m_listBourseInfo.end(); it ++)
		{
			BourseInfo* pBourseInfo = (BourseInfo*)*it;
			pWrite->WriteComBouseData(pBourseInfo);
			bRet = TRUE;
		}
		lock.Unlock();
	}
	return bRet;
}