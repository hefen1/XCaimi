//////////////////////////////////////////////////////////////////////////
//stockManager.cpp

#include "stdafx.h"
#include "stockManager.h"
#include "DataSourceImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_STOCK_SIZE	4096

bool comp( StockUserInfo *lhs,  StockUserInfo *rhs)
{
	return lhs->GetCode()< rhs->GetCode();
}
//////////////////////////////////////////////////////////////////////////
CStockManager::CStockManager()
{
//	m_mapStocks.InitHashTable(MAX_STOCK_SIZE);
	m_pDataSource = NULL;
	m_psiExtend = NULL;
	m_mapFinanceData.RemoveAll();
}

CStockManager::~CStockManager()
{
	RemoveAllStocks();
	CString strKey;
	CurrentFinanceData* pFinanceData = NULL;
	POSITION pos;
	pos = m_mapFinanceData.GetStartPosition();
	while( pos)
	{
		m_mapFinanceData.GetNextAssoc(pos, strKey, (void*&)pFinanceData);
		if ( pFinanceData && !IsBadReadPtr(pFinanceData,1))
			delete pFinanceData;
	}
	m_mapFinanceData.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 本函数不能单独使用
CStringArray* CStockManager::GetPyjcByHZ(char*& pHZ,int& nNext)
{
	CStringArray* pAry = new CStringArray;
	CString str;
	CString strValue;

	if( (*pHZ & 0x80) && ( *(pHZ+1) & 0x80) )
	{
		str = CString(pHZ,2);
		pHZ += 2;
		nNext += 2;
	}
	else
	{
		str = *pHZ;
		pHZ++;
		nNext++;

		if(str.GetAt(0) >= _T('0') && str.GetAt(0) <= _T('9'))
		{
			pAry->Add(str);
			return pAry;
		}
	}


	if( !m_pyjcManager.Lookup(str,strValue) )
		return pAry;
	if( strValue.IsEmpty() )
		return pAry;

	for(int i = 0; i < strValue.GetLength(); i++)
		pAry->Add(strValue.Mid(i, 1));

	return pAry;
}

void CStockManager::WordToPYJC(const char* pcWord,char* pcVoice, int nSize)
{
	// 低位是汉字长度，高位是拼音长度
	short l = LOWORD(nSize);
	short h = HIWORD(nSize);
	if( h <= 0 )
		h = PYJC_MAX_LENGTH;

	short nPyjc = 0;
	//pcVoice.Empty();
	memset(pcVoice, 0, h);

	char* s = (char*)pcWord;

	CString str;
	CString strValue;
	int i = 0;

	CArray<CStringArray*,CStringArray*> ay;
	CStringArray* pItem;

	for( ;; )
	{
		if( i >= nSize )
			break;

		pItem = GetPyjcByHZ(s,i);
		ay.Add(pItem);
	}


	// 现在只支持4种不同拼音简称,形如:
	//  A B      ab   ad
	//       ==>      
	//  C D		 cd   cb

	short nNext = 0;
	short nCount;
	// 1
	for( i = 0; i < ay.GetSize() && nPyjc < 4; i++ )
	{
		pItem = ay.GetAt(i);		
		nCount = pItem->GetSize();
		if( nCount > 0 )
		{
			pcVoice[ nPyjc ] = pItem->GetAt(0).GetAt(0);
			nPyjc++;
		}

		if( nCount > 1 )
		{
			if( nNext == 1 )
				nNext = 2;
			else if( nNext != 2 )
				nNext = 1;
		}
	}

	if( nPyjc != 4 )
		nPyjc = 4;
	// 2
	for( i = 0; nNext  && i < ay.GetSize() && nPyjc < 8; i++ )
	{
		pItem = ay.GetAt(i);	
		nCount = pItem->GetSize();
		if( nCount > 0 )
		{
			nCount = max((nCount-1),0);
			pcVoice[ nPyjc ] = pItem->GetAt(nCount).GetAt(0);
			nPyjc++;
		}
	}

	_delArrayObj(ay);

}

void CStockManager::SetDataSourceImpl(CDataSourceImpl* pImpl)
{
	m_pDataSource = pImpl;
	if (m_pDataSource != NULL)
	{
		CDSWriteFile* pFile = m_pDataSource->GetDSWriterFile();
		if (pFile != NULL)
		{
			m_pyjcManager.Init(pFile->GetSettingFilePath());
		}
	}
}

BOOL CStockManager::PushStock(CHSDSStockInfoIntact* pStock)
{
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		if ( pStock && !pStock->m_stockInfo.GetKey().IsEmpty() && pStock->m_stockInfo.m_ciStockCode.m_cCodeType>0 )
		{
			WordToPYJC(pStock->m_stockInfo.m_cStockName, pStock->m_stockInfo.m_cShortPyjc, 8);
			MakeShortName(&pStock->m_stockInfo);
			m_ayStocks.Add(pStock);
			m_mapIndex.SetAt(pStock->m_stockInfo.GetCode(), (CObject*)(m_ayStocks.GetCount()-1));
		}		
		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}
BOOL CStockManager::CreateIndex()
{
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		m_mapIndex.RemoveAll();
		for ( int i=0; i<m_ayStocks.GetCount(); i++)
		{
			CHSDSStockInfoIntact* pStock = m_ayStocks.GetAt(i);
			if ( pStock )
			{
				memset(&pStock->m_stockInfo.m_cShortPyjc[12], 0, PYJC_MAX_LENGTH-12);	
				m_mapIndex.SetAt(pStock->m_stockInfo.GetCode(), (CObject*)i);
				MakeShortName(&pStock->m_stockInfo);
			}
		}
		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}
void CStockManager::MakeShortName(StockUserInfo *pStock)
{
	// 低位是汉字长度，高位是拼音长度
	short nPyjc = 0;

	CString StockName = pStock->GetName(TRUE);
	StockInfoEx *pStockEx = GetStockInfoEx(&pStock->m_ciStockCode);
	memcpy(&pStock->m_cShortPyjc[12], &pStockEx, sizeof(StockInfoEx *));

	int i = 0;
	int bufsz = 0;
	CArray<CStringArray*,CStringArray*> ay;
	CStringArray* pItem;
	char *s = StockName.GetBuffer(StockName.GetLength());

	while(i < StockName.GetLength())
	{
		pItem = GetPyjcByHZ(s,i);
		ay.Add(pItem);
		bufsz += pItem->GetSize();
		if(pItem->GetSize() > 1)
			bufsz++;
	}

	StockName.ReleaseBuffer();

	if(ay.GetSize())
	{
		int j = 0;
		pStockEx->pPyjcArray = new CString;
		LPSTR pBuffer = pStockEx->pPyjcArray->GetBufferSetLength(bufsz + 1);
		for(i = 0; i < ay.GetSize(); i++)
		{
			pItem = ay.GetAt(i);
			if(pItem->GetSize() > 1)
			{
				pBuffer[j++] = (char)pItem->GetSize();

				for(int k = 0; k < pItem->GetSize(); k++)
					pBuffer[j++] = pItem->GetAt(k).GetAt(0);
			}
			else if(pItem->GetSize())
			{
				char c = pItem->GetAt(0).GetAt(0);
				pBuffer[j++] = pItem->GetAt(0).GetAt(0);
			}
		}

		pStockEx->pPyjcArray->ReleaseBuffer(j);
	}

	_delArrayObj(ay);
}
void CStockManager::MakeAllShortName()
{

}
StockInfoEx* CStockManager::GetStockInfoEx(const CodeInfo *pCode)
{
	if(m_psiExtend == NULL)
	{
		m_psiExtend = new CArray<StockInfoEx *, StockInfoEx *>;
	}

	for(int i = 0; i < m_psiExtend->GetSize(); i++)
	{
		StockInfoEx *pStockEx = m_psiExtend->GetAt(i);
		if(memcmp(&pStockEx->Code, pCode, sizeof(CodeInfo)) == 0)
			return pStockEx;
	}

	StockInfoEx *pStockEx = new StockInfoEx(pCode);
	m_psiExtend->Add(pStockEx);

	return pStockEx;
}

CHSDSStockInfoIntact* CStockManager::GetStock(CodeInfo* pInfo)
{
	CHSDSStockInfoIntact* pStockReturn = NULL;
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		int nIndex = -1;
		m_mapIndex.Lookup(pInfo->GetCode(), (CObject*&)nIndex);
		if ( nIndex >=0 && m_ayStocks.GetCount() > nIndex)
		{
			CHSDSStockInfoIntact* pStock = m_ayStocks.GetAt(nIndex);
			if (pStock != NULL && pStock->m_stockInfo.m_ciStockCode.CompareCode(pInfo))//memcmp(&pStock->m_stockInfo.m_ciStockCode, pInfo, sizeof(CodeInfo)) == 0)
			{
				pStockReturn = pStock;
			}
		}
		lock.Unlock();
		return pStockReturn;
	}
	return NULL;;
}

CHSDSStockInfoIntact* CStockManager::GetStock(char* pszCode, unsigned short nMarket, unsigned short nMarketException)
{
	if (pszCode == NULL)
		return NULL;
	CHSDSStockInfoIntact* pStockReturn = NULL;
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{

		CHSDSStockInfoIntact* pStock = NULL;
		int nIndex = -1;
		m_mapIndex.Lookup(pszCode, (CObject*&)nIndex);
		if ( nIndex >=0 && nIndex < m_ayStocks.GetCount())
		{
			pStock = m_ayStocks.GetAt(nIndex);
		}
		CString strCode1 = pStock->m_stockInfo.m_ciStockCode.GetCode();
		CString strCode2 = pszCode;
		if (pStock != NULL && strCode1 == strCode2)
		{
			if ((nMarket & pStock->m_stockInfo.m_ciStockCode.m_cCodeType) != 0 && (pStock->m_stockInfo.m_ciStockCode.m_cCodeType & nMarketException) == 0)
			{
				pStockReturn = pStock;
			}
		}
		lock.Unlock();
	}
	return pStockReturn;
}

BOOL CStockManager::RemoveStock(CHSDSStockInfoIntact* pStock)
{
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		int nIndex = -1;
		m_mapIndex.Lookup(pStock->m_stockInfo.GetCode(), (CObject*&)nIndex);
		if ( nIndex >=0 && nIndex < m_ayStocks.GetCount())
			m_ayStocks.RemoveAt(nIndex);

		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}
BOOL CStockManager::RemoveStockInfoEx()
{
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		if (m_psiExtend != NULL)
		{
			while(m_psiExtend->GetSize() > 0)
			{
				StockInfoEx* pStockInfoEx = m_psiExtend->GetAt(0);
				if (pStockInfoEx != NULL)
				{
					delete pStockInfoEx;
					pStockInfoEx = NULL;
				}
				m_psiExtend->RemoveAt(0);
			}
			delete m_psiExtend;
			m_psiExtend = NULL;
		}
		for ( int i=0; i<m_ayStocks.GetCount(); i++)
		{
			CHSDSStockInfoIntact* pStock = m_ayStocks.GetAt(i);
			if ( pStock )
			{
				memset(&pStock->m_stockInfo.m_cShortPyjc[12], 0, PYJC_MAX_LENGTH-12);	
			}
		}
		lock.Unlock();
		return TRUE;
	}
	return FALSE;	
}
BOOL CStockManager::RemoveStocksByMarket(HSMarketDataType nMarket)
{
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		for ( int i=0 ;i<m_ayStocks.GetCount(); i++)
		{
			CHSDSStockInfoIntact* pInfoIntact = m_ayStocks.GetAt(i);
			if ( pInfoIntact && ( MakeMainMarket( pInfoIntact->m_stockInfo.m_ciStockCode.m_cCodeType) == nMarket))
			{
				m_mapIndex.RemoveKey(pInfoIntact->m_stockInfo.m_ciStockCode.GetCode());
				delete pInfoIntact;
				m_ayStocks.RemoveAt(i);
				i--;
			}			
		}
		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}

BOOL CStockManager::RemoveAllStocks()
{
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		for ( int i=0 ;i<m_ayStocks.GetCount(); i++)
		{
			delete m_ayStocks.GetAt(i);
		}
		m_ayStocks.RemoveAll();
		m_mapIndex.RemoveAll();

		if (m_psiExtend != NULL)
		{
			while(m_psiExtend->GetSize() > 0)
			{
				StockInfoEx* pStockInfoEx = m_psiExtend->GetAt(0);
				m_psiExtend->RemoveAt(0);
				if (pStockInfoEx != NULL)
				{
					delete pStockInfoEx;
					pStockInfoEx = NULL;
				}
			}
			delete m_psiExtend;
			m_psiExtend = NULL;
		}

		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}
int	CStockManager::GetMarketStockList(HSMarketDataType* pMarket, int nMarketCount,CArray<StockUserInfo>& ayRef)
{
	
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		int nCount = 0;		
		for ( int i=0; i<m_ayStocks.GetCount(); i++ )
		{
			CHSDSStockInfoIntact* pStockIntact = (CHSDSStockInfoIntact*)m_ayStocks.GetAt(i);
			HSMarketDataType hsMarketCur = pStockIntact->m_stockInfo.m_ciStockCode.m_cCodeType;
			BOOL bInMarket = FALSE;

			if ( pStockIntact && !IsBadReadPtr(pStockIntact,sizeof(CHSDSStockInfoIntact)))
			{
				HSMarketDataType hsMarketCur = pStockIntact->m_stockInfo.m_ciStockCode.m_cCodeType;
				BOOL bInMarket = FALSE;
				if (pStockIntact != NULL )
				{
					for (int i = 0; i < nMarketCount; i ++)
					{
						if (hsMarketCur == pMarket[i])
						{
							bInMarket = TRUE;
							break;
						}
					}
				}
				if (bInMarket )
				{
					ayRef.Add(pStockIntact->m_stockInfo);					
				}
			}			
		}
		lock.Unlock();
	}
	return ayRef.GetCount();
}

int	CStockManager::GetStocksList(CArray<CodeInfo>* pArInfo, CArray<StockUserInfo>& stockInfo, BOOL bCompareOnlyMainMarket)
{
	if ( m_ayStocks.GetCount() <= 0)
	{
		return 0;
	}
	if (pArInfo != NULL )
	{
		CHSDSStockInfoIntact* pStock = NULL;
		CSingleLock lock(&m_mutexStock);
		if (lock.Lock())
		{
			if (pArInfo->GetSize() > 0)
			for ( int i=0; i<pArInfo->GetCount(); i++ )
			{
				CodeInfo* pCode = &pArInfo->GetAt(i);
				if ( pCode && !IsBadReadPtr(pCode, sizeof(CodeInfo)))
				{
					int nIndex = -1;
					m_mapIndex.Lookup(pCode->GetCode(), (CObject*&)nIndex);
					if ( nIndex >=0 && nIndex<m_ayStocks.GetCount() )
					{
						CHSDSStockInfoIntact* pStock = m_ayStocks.GetAt(nIndex);
						if (pStock != NULL)
						{
							StockUserInfo stock;
							memset(&stock, 0, sizeof(StockUserInfo));
							stock.Copy(&pStock->m_stockInfo);
							stockInfo.Add(stock);
						}
					}
				}
			}
		}
		lock.Unlock();
		return stockInfo.GetSize();
	}
	return 0;
}

int	CStockManager::GetStockCodeInfoList(CArray<CodeInfo*>& arCodeInfo)
{
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		for (int i=0; i<m_ayStocks.GetCount(); i++)
		{
			CHSDSStockInfoIntact* pStock = m_ayStocks.GetAt(i);
			if (pStock != NULL && MakeMarket(pStock->m_stockInfo.m_ciStockCode.m_cCodeType) == STOCK_MARKET 
				&& MakeSubMarket(pStock->m_stockInfo.m_ciStockCode.m_cCodeType) != KIND_INDEX)
			{
				arCodeInfo.Add(&pStock->m_stockInfo.m_ciStockCode);
			}
		}
		lock.Unlock();
	}
	return arCodeInfo.GetSize();
}

int	CStockManager::RecvServerCalcData(AnsSeverCalculate* pData)
{
	if (pData == NULL || ::IsBadReadPtr(pData, sizeof(AnsSeverCalculate)))
	{
		return -1;
	}
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		int nCount = pData->m_nSize;
		SeverCalculateData* pSrvCalData = (SeverCalculateData*)pData->m_cData;

		for (int i = 0; i < nCount; i ++)
		{
			SeverCalculateData* pDataCur = &pSrvCalData[i];
			if ( pDataCur == NULL )
				continue;
			int nIndex = -1;
			char cCode[7];
			memset(cCode,0,7);
			strncpy_s(cCode,pDataCur->m_cCode,6);
			CString strCode(cCode);		
			m_mapIndex.Lookup(strCode, (CObject*&)nIndex);
			if ( nIndex >= 0 && nIndex < m_ayStocks.GetCount())
			{
				CHSDSStockInfoIntact* pStock = m_ayStocks.GetAt(nIndex);
				if (pStock != NULL && memcmp(pStock->m_stockInfo.m_ciStockCode.m_cCode, pDataCur->m_cCode, sizeof(pStock->m_stockInfo.m_ciStockCode.m_cCode)) == 0
					&& pStock->m_stockInfo.m_ciStockCode.m_cCodeType == pDataCur->m_cCodeType)
				{
					pStock->m_stockInfo.m_fDownPrice = pDataCur->m_fDownPrice;
					pStock->m_stockInfo.m_fUpPrice = pDataCur->m_fUpPrice;
				}
			}
		}
		lock.Unlock();
	}
	return 0;
}

BOOL CStockManager::WriteLocalFile()
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
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		if(pWrite->StartWriteStockData())
		{
			for (int i=0; i<m_ayStocks.GetCount(); i++)
			{
				CHSDSStockInfoIntact* pStock = m_ayStocks.GetAt(i);
				if (pStock != NULL )
				{
					pWrite->WriteStocksData(&pStock->m_stockInfo);
				}
			}
			pWrite->EndWriteStockData();
			bRet = TRUE;
		}
		lock.Unlock();
	}
	return bRet;
}
BOOL	CStockManager::SetFinanceDataToMap()
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

	CString strKey;
	CurrentFinanceData* pFinanceData = NULL;
	POSITION pos;
	pos = m_mapFinanceData.GetStartPosition();
	while( pos)
	{
		m_mapFinanceData.GetNextAssoc(pos, strKey, (void*&)pFinanceData);
		if ( pFinanceData && !IsBadReadPtr(pFinanceData,1))
			delete pFinanceData;
	}
	m_mapFinanceData.RemoveAll();

	return pWrite->SetStocksFinanceData(m_mapFinanceData);
}
int	CStockManager::GetStocksFinanceData(CArray<CodeInfo>* pArInfo, CArray<CurrentFinanceData>& ayFinanceData)
{
	if (pArInfo == NULL )
		return 0;
	for ( int i=0; i<pArInfo->GetCount(); i++)
	{
		CodeInfo Code = pArInfo->GetAt(i);
		CString strCode = Code.GetCode();
		CurrentFinanceData* pFinanceData = NULL;
		if ( m_mapFinanceData.Lookup(strCode, (void*&)pFinanceData) && pFinanceData )
		{
			ayFinanceData.Add(*pFinanceData);
		}
	}
	return ayFinanceData.GetSize();
}

int	CStockManager::GetStocksExrighData(CArray<CodeInfo>* pArInfo, CArray<CurrentExRightData>& ayExrightData)
{
	CDSWriteFile* pWrite = NULL;
	if (m_pDataSource != NULL)
	{
		pWrite = m_pDataSource->GetDSWriterFile();
	}
	if (pWrite == NULL || pArInfo == NULL || pArInfo->GetSize() <= 0)
	{
		return 0;
	}
	CString strCodeCompare = _T("");
	for (int i = 0; i < pArInfo->GetSize(); i ++)
	{
		CodeInfo codeInfo = pArInfo->GetAt(i);
		if(codeInfo.m_cCode[0] != '\0')
		{
			CString strCode = CString(codeInfo.m_cCode, sizeof(codeInfo.m_cCode));
			strCodeCompare += strCode;
			strCodeCompare += _T(";");
		}
	}
	pWrite->ReadStocksExrightData(strCodeCompare, ayExrightData, pArInfo->GetSize());
	return ayExrightData.GetSize();
}

short CStockManager::GetStockHand( HSMarketDataType nMarket )
{
	if ( MakeFundMarket(nMarket) )   //基金
		return 100;
	if( MakeMarket(nMarket) == QH_Bourse || MakeMarket(nMarket) == WP_MARKET
		|| MakeMarket(nMarket) == (HSMarketDataType)FOREIGN_MARKET )    // 期货
	{
		return 1;
	}
	 if( MakeMainMarket(nMarket) == SH_Bourse && MakeHexSubMarket(nMarket) == KIND_QuanZhen) // 上证权证
	{
		return 100;
	}
	 if( MakeMarket(nMarket) == HK_MARKET || MakeMarket(nMarket) == FOREIGN_MARKET)        //港股 外汇
	{
		return 1;
	}
	 if( MakeMainMarket(nMarket) == SZ_Bourse && MakeHexSubMarket(nMarket) == KIND_BOND)  //深圳债券为10张每手
	{
		return 10;
	}
	
	if( ((MakeMarket(nMarket) == STOCK_MARKET) &&            	
		(MakeSubMarket(nMarket) == KIND_BOND)) ||			    // 债券
		MakeIndexMarket(nMarket) )								// 指数
	{
		return 1;
	}
	return 100;      //股票                                           	

}

int CStockManager::FindStockFromKey( StockUserInfo* pStock, const CString& strKey,int& nUnique, int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex )
{
	if ( pStock == NULL )
		return -1;
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		int nIndex = pStock->FindFromTail(strKey, nUnique, nStart, nEnd, szMarch, nBetter, nCurIndex);
		lock.Unlock();
		return nIndex;
	}
	return -1;
}

CString CStockManager::GetStockPyjc( StockUserInfo* pStock )
{
	if ( pStock == NULL )
		return "";
	CSingleLock lock(&m_mutexStock);
	if (lock.Lock())
	{
		CString str = pStock->GetDefaultPyjc();
		return str;
		lock.Unlock();
	}
	return "";
}