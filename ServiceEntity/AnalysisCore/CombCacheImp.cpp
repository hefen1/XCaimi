#include "StdAfx.h"
#include "CombCacheImp.h"
#include "WinnerApplication.h"
#include "NetEngineInterface.h"
#include "AnalysisCore.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�����߳�ִ�к���
UINT DoCalculateWorkThreadProc( LPVOID pParam )
{
	CCombCacheSet* pObject = (CCombCacheSet*)pParam;

	//if (pObject == NULL || !pObject->IsKindOf(RUNTIME_CLASS(CCombCacheSet)))
	//	return 1; 

	while(TRUE)
	{
		if ( pObject->DoCalculateWork() == FALSE )
		{
			::AfxEndThread(0, FALSE); 
			break;
		}
	}
	return 0L; 
}

BOOL IsStockStop(const ShareRealTimeData_Ext &srtData)
{
	if(
		srtData.m_nowDataExt.m_stockRealTime.m_lTotal
		+ srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount1
		+ srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount2
		+ srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount3
		+ srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount4
		+ srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount5
		+ srtData.m_nowDataExt.m_stockRealTime.m_lSellCount1
		+ srtData.m_nowDataExt.m_stockRealTime.m_lSellCount2
		+ srtData.m_nowDataExt.m_stockRealTime.m_lSellCount3
		+ srtData.m_nowDataExt.m_stockRealTime.m_lSellCount4
		+ srtData.m_nowDataExt.m_stockRealTime.m_lSellCount5
		== 0
		)
	{
		return TRUE;
	}
	return FALSE;
}

void LogStockRealTimeInfo(CString strCode,const ShareRealTimeData_Ext &srtData)
{
	CString strFormat="[%s]\n";
	strFormat += _T("srtData.m_nowDataExt.m_stockOther.m_lStopFlag=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockOther.m_lNewPrice=%d\n");

	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount1=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount2=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount3=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount4=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount5=%d\n");

	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice1=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice2=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice3=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice4=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice5=%d\n");

	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellCount1=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellCount2=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellCount3=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellCount4=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellCount5=%d\n");

	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice1=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice2=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice3=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice4=%d\n");
	strFormat += _T("srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice5=%d\n");

	CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,strFormat,strCode,
		srtData.m_nowDataExt.m_stockOther.m_lStopFlag,
		srtData.m_nowDataExt.m_stockRealTime.m_lNewPrice,

		srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount1,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount2,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount2,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount4,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount5,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice1,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice2,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice3,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice4,
		srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice5,

		srtData.m_nowDataExt.m_stockRealTime.m_lSellCount1,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellCount2,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellCount3,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellCount4,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellCount5,

		srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice1,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice2,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice3,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice4,
		srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice5
		);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CChangedQuoteData::CChangedQuoteData()
{
}

CChangedQuoteData::~CChangedQuoteData()
{
	for( int i = 0; i < m_changedSTRData.GetCount();i++)
	{
		ShareRealTimeChanged* pRealDataChg = (ShareRealTimeChanged*)m_changedSTRData.GetAt(i);
		delete pRealDataChg;
	}
}

void CChangedQuoteData::AddChangedData(ShareRealTimeChanged *pChangedData)
{
	if(pChangedData != NULL)
	{
		ShareRealTimeChanged* pDataNew = new 	ShareRealTimeChanged;
		memcpy(pDataNew,pChangedData,sizeof(ShareRealTimeChanged));
		m_changedSTRData.Add(pDataNew);
	}	
}

CCombCache::CCombCache(CCombCacheSet*  pOwner)
{
	m_pDataSourceEx = NULL;
	m_nAmount = 0;
	m_dCombSZ = 0;
	m_dCombBuyPKSZ = 0;
	m_dCombSellPKSZ = 0;
	m_pOwner = pOwner;
}

CCombCache::~CCombCache(void)
{
	POSITION pos = m_componentCatchMap.GetStartPosition();
	CString  strKey;
	CComponentCache* pComponentCache;
	while (pos != NULL)
	{
		m_componentCatchMap.GetNextAssoc(pos, strKey, (void*&)pComponentCache);
		if(pComponentCache)
			delete pComponentCache;
	}
}

BOOL CCombCache::InitCombCache(ICombInfo* pCombInfo)
{
	m_strCombName = pCombInfo->GetCombName();
	m_nAmount = pCombInfo->GetAmount();
	int nCount = pCombInfo->GetComponentCount();
	for( int i = 0; i < nCount; i++)
	{
		ICombComponent* pComponent = pCombInfo->GetCombComponent(i);
		CComponentCache* pComponentCache = new CComponentCache();
		pComponentCache->m_lAmount = pComponent->GetAmount();
		CString strCode = pComponent->GetCode();
		m_componentCatchMap.SetAt(strCode,pComponentCache);
		if(m_pDataSourceEx)
		{
			StockUserInfo   stkInfo; 
			BOOL bFind = m_pDataSourceEx->HSDataSourceEx_GetStockUserInfo(strCode.GetBuffer(),0xFFFF,stkInfo);
			if( bFind )
				pComponentCache->m_stkInfo.Copy(&stkInfo);
			else
				CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[%s] �Ҳ�����Ӧ�Ĵ�����Ϣ."),strCode);
		}
	}
	return TRUE;
}

void   CCombCache::GetAllComponentCode(CArray<StockUserInfo,StockUserInfo> &ayStkCode)
{
	ayStkCode.RemoveAll();
	POSITION pos = m_componentCatchMap.GetStartPosition();
	CString  strKey;
	CComponentCache* pComponentCache;
	while (pos != NULL)
	{
		m_componentCatchMap.GetNextAssoc(pos, strKey, (void*&)pComponentCache);
		if(pComponentCache)
			ayStkCode.Add(pComponentCache->m_stkInfo);
	}
}

BOOL	CCombCache::OnUpdateData(DataHead* pData)
{
	if (pData->m_nType == RT_REALTIME_EXT ||  pData->m_nType == RT_AUTOPUSH_EXT)
	{
		AnsRealTime* pNowData = (AnsRealTime*)pData;
		if (pNowData->m_nSize <= 0)
			return FALSE;
		HandleNowData(pData,pNowData->m_pnowData,pNowData->m_nSize);
	}
	return TRUE;
}

void CCombCache::HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	//��������
	CSingleLock singleLock(&m_critSection,TRUE);

	CommRealTimeData_Ext* pData = (CommRealTimeData_Ext*)pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	CChangedQuoteData* pChangeQuoteData = NULL;
	StockUserInfo* pStock = NULL;
	int nPos = 0;
	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		ShareRealTimeData_Ext* pCurrentRealTimeExt =  (ShareRealTimeData_Ext*)pData->m_cNowData;
		CString strCode = pData->m_ciStockCode.GetCode();
		CComponentCache* pComponentCache = NULL;
		if( m_componentCatchMap.Lookup(strCode,(void*&)pComponentCache) == TRUE )
		{
			if(memcmp(&pComponentCache->m_realTimeData,pCurrentRealTimeExt,sizeof(ShareRealTimeData_Ext)) != 0)
			{
				if(pChangeQuoteData == NULL)
					pChangeQuoteData = new CChangedQuoteData();

				//�������̿���ֵ
				double dBuyPKSZ = 0;
				if(pComponentCache->m_lAmount)
				{
					dBuyPKSZ = CalStockCompentBuyPKSZ(strCode,*pCurrentRealTimeExt,pComponentCache->m_lAmount);
					if(dBuyPKSZ <= 0)
					{
						CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::HandleNowData]����ɷݹ�������ֵ�쳣.����[%s]m_lAmount=%d,sz=%d"),
							strCode,
							pComponentCache->m_lAmount,
							dBuyPKSZ);
						LogStockRealTimeInfo(strCode,*pCurrentRealTimeExt);
					}
				}

				//�������̿���ֵ
				double dSellPKSZ = 0;
				if(pComponentCache->m_lAmount>0)
				{
					dSellPKSZ = CalStockCompentSellPKSZ(strCode,*pCurrentRealTimeExt,pComponentCache->m_lAmount);
					if(dSellPKSZ <= 0)
					{
						CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::HandleNowData]����ɷݹ�������ֵ�쳣.����[%s]."),strCode);
					}
				}

				//��������䶯
				ShareRealTimeChanged srtChanged;
				memcpy(&srtChanged.m_ciStockCode,&pData->m_ciStockCode,sizeof(CodeInfo));
				srtChanged.m_lAmount = pComponentCache->m_lAmount;
				memcpy(&srtChanged.m_lastData,pCurrentRealTimeExt,sizeof(ShareRealTimeData_Ext));//����ʵʱ����
				memcpy(&srtChanged.m_preData,&pComponentCache->m_realTimeData,sizeof(ShareRealTimeData_Ext));//��һ������ʵʱ����

				//���¼�Ϊ0�����ó�����
				if( (MakeMarket(srtChanged.m_ciStockCode.m_cCodeType ) == STOCK_MARKET)
					&& (srtChanged.m_lastData.m_nowDataExt.m_stockRealTime.m_lNewPrice == 0)
					)
				{
					long lClose = (long)(m_pOwner->GetClose(strCode) * Default_Price_Unit);
					srtChanged.m_lastData.m_nowDataExt.m_stockRealTime.m_lNewPrice = lClose;
					CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::HandleNowData][%s]�ɷݹ�[%s]�����¼�Ϊ0,�����¼��ó����ռ�(%d)."),
						m_strCombName,
						strCode,lClose);
				}

				srtChanged.m_dlastBuyPKSZ = dBuyPKSZ;//�������̿���ֵ
				srtChanged.m_dPreBuyPKSZ = pComponentCache->m_dBuyPKSZ;//��һ�����̿���ֵ

				srtChanged.m_dlastSellPKSZ = dSellPKSZ;//�������̿���ֵ
				srtChanged.m_dPreSellPKSZ = pComponentCache->m_dSellPKSZ;//��һ�����̿���ֵ

				pChangeQuoteData->AddChangedData(&srtChanged);

				//���³ɷݹ���������
				pComponentCache->m_realTimeData = *pCurrentRealTimeExt;
				//���³ɷݹ����̿���ֵ
				pComponentCache->m_dBuyPKSZ = dBuyPKSZ;
				pComponentCache->m_dSellPKSZ = dSellPKSZ;
			}		
		}
		pData = pNextData;		
	}

	if (pChangeQuoteData != NULL)
	{
		m_quoteChangeDataList.AddTail(pChangeQuoteData);
	}
}

void CCombCache::DoCalculateWork()
{
	//��������
	CSingleLock singleLock(&m_critSection,TRUE);

	if(m_nAmount < 0)
		return;

	if (m_quoteChangeDataList.GetCount() <= 0)
		return;

	CChangedQuoteData* pChangeQuoteData = m_quoteChangeDataList.GetHead();
	pChangeQuoteData = m_quoteChangeDataList.GetHead(); 
	m_quoteChangeDataList.RemoveHead();

	if(pChangeQuoteData)
	{
		CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::DoCalculateWork]�䶯����.����[%d]."),
			pChangeQuoteData->m_changedSTRData.GetCount());

		for( int i = 0; i <= pChangeQuoteData->m_changedSTRData.GetCount() -1; i++)
		{
			ShareRealTimeChanged* pSRTChanged = (ShareRealTimeChanged*)pChangeQuoteData->m_changedSTRData.GetAt(i);
			if( MakeIndexMarket(pSRTChanged->m_ciStockCode.m_cCodeType ) ) //ָ������
			{
				//���������ֵ
				m_dCombSZ += (pSRTChanged->m_lastData.m_indData.m_lNewPrice - pSRTChanged->m_preData.m_indData.m_lNewPrice) 
					* pSRTChanged->m_lAmount  / (double)Default_Price_Unit;
				CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::DoCalculateWork]ָ���������,�������̿���ֵ=[%s]."),
					pSRTChanged->m_ciStockCode.GetCode());
			}
			else if(MakeMarket(pSRTChanged->m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//�ڻ�����
			{
				//���������ֵ
				m_dCombSZ += (pSRTChanged->m_lastData.m_qhData.m_lNewPrice - pSRTChanged->m_preData.m_qhData.m_lNewPrice )  
					* pSRTChanged->m_lAmount  / (double) Default_Price_Unit;
				CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::DoCalculateWork]�ڻ�����,�������̿���ֵ[%s]."),
					pSRTChanged->m_ciStockCode.GetCode());
			}
			else if(MakeMarket(pSRTChanged->m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//��Ʊ
			{				
				//���������ֵ:
				double dInc =  (pSRTChanged->m_lastData.m_nowDataExt.m_stockRealTime.m_lNewPrice - pSRTChanged->m_preData.m_nowDataExt.m_stockRealTime.m_lNewPrice)
					* pSRTChanged->m_lAmount  /  (double)Default_Price_Unit;
				m_dCombSZ += dInc;

// 				CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("code[%x-%s] Ȩ�� = %d, �ɷݹ���ֵ=          %f"),
// 					pSRTChanged->m_ciStockCode.m_cCodeType,
// 					pSRTChanged->m_ciStockCode.GetCode(),
// 					 pSRTChanged->m_lAmount,
// 					dInc);

				//�����̿���ֵ
				m_dCombBuyPKSZ += pSRTChanged->m_dlastBuyPKSZ - pSRTChanged->m_dPreBuyPKSZ;
				m_dCombSellPKSZ += pSRTChanged->m_dlastSellPKSZ - pSRTChanged->m_dPreSellPKSZ;
			}
			else
			{
				CWinnerApplication::FormatLog(WARN_LOG_LEVEL,LN_WINNER,_T("[CCombCacheSet::DoCalculateWork]δ����Ĵ���=[%s]."),
					pSRTChanged->m_ciStockCode.GetCode());
			}		
		}
		delete pChangeQuoteData;
	}
}

double CCombCache::CalStockCompentBuyPKSZ(CString strCode,const ShareRealTimeData_Ext &srtData,long nAmount)
{
	//if(srtData.m_nowDataExt.m_stockOther.m_lStopFlag == 1)
	if(IsStockStop(srtData))
	{
		double dPKPirce = m_pOwner->GetClose(strCode);
		CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,LN_WINNER,_T("�������̿���ֵ:code[%s] ͣ��,������[%f]�������¼�"),
			strCode,dPKPirce);
		return dPKPirce * nAmount;
	}

	double dBuyPKSZ = 0;
	int nNoPrice = 0;
	//��һ
	long nPKAmount = 0;
	double dPKPirce = 0;
	if(srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice1 > 0)
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount1 ;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice1 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}

	//���
	if(srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice2 > 0)
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount2;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice2 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}
	

	//����
	if(srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice3 > 0 )
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount3;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice3 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}

	//����
	if(srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice4  > 0)
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lBuyCount4;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice4 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}

	//����
	if(srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice5 > 0 )
	{
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice5 / (double)1000;
		dBuyPKSZ += nAmount  * dPKPirce;
		return dBuyPKSZ;
	}
	else
	{
		nNoPrice++;
	}
	
	//���Ͼ���ʱֻ��һ���мۣ�����̿ڲ��㣬��ȥ��ȡһ���۸�����
	if(nNoPrice > 0 )
	{
		//�������ͣ����1��ҲΪ0����ȡ���¼�
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice1 != 0 ?
			srtData.m_nowDataExt.m_stockRealTime.m_lBuyPrice1 : srtData.m_nowDataExt.m_stockRealTime.m_lNewPrice;
		dPKPirce = dPKPirce / (double)1000;
		dBuyPKSZ += nAmount  * dPKPirce;
		return dBuyPKSZ;
	}

	return 0;
}

double CCombCache::CalStockCompentSellPKSZ(CString strCode,const ShareRealTimeData_Ext &srtData,long nAmount)
{
	//if(srtData.m_nowDataExt.m_stockOther.m_lStopFlag == 1)
	if(IsStockStop(srtData))
	{
		double dPKPirce = m_pOwner->GetClose(strCode);
		CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,LN_WINNER,_T("�������̿���ֵ:code[%s] ͣ��,������[%f]�������¼�"),
			strCode,dPKPirce);
		return dPKPirce * nAmount;
	}

	double dBuyPKSZ = 0;
	int nNoPrice = 0;
	long nPKAmount = 0;
	double dPKPirce = 0;
	//��һ
	if( srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice1 > 0 )
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lSellCount1 ;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice1 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}
	

	//����
	if( srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice2 > 0 )
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lSellCount2;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice2 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}
	

	//����
	if(srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice3 > 0)
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lSellCount3;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice3 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}

	//����
	if(srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice4>0)
	{
		nPKAmount = srtData.m_nowDataExt.m_stockRealTime.m_lSellCount4;
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice4 / (double)1000;
		if(nAmount > nPKAmount )
		{
			dBuyPKSZ += nPKAmount *  dPKPirce; 
			nAmount = nAmount - nPKAmount;
		}
		else
		{
			dBuyPKSZ += nAmount  * dPKPirce;
			return dBuyPKSZ;
		}
	}
	else
	{
		nNoPrice++;
	}
	

	//����
	if(srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice5 > 0)
	{	
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice5 / (double)1000;
		dBuyPKSZ += nAmount  * dPKPirce;
		return dBuyPKSZ;
	}
	
	//���Ͼ���ʱֻ��һ���мۣ�����̿ڲ��㣬��ȥ��ȡһ���۸�����
	if(nNoPrice > 0 )
	{
		//�������ͣ����1��ҲΪ0����ȡ���¼�
		dPKPirce = srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice1 != 0 ?
			srtData.m_nowDataExt.m_stockRealTime.m_lSellPrice1 : srtData.m_nowDataExt.m_stockRealTime.m_lNewPrice;
		dPKPirce = dPKPirce / (double)1000;
		dBuyPKSZ += nAmount  * dPKPirce;
		return dBuyPKSZ;
	}
	return 0;
}

double CCombCache::GetCombSZ()
{
	CSingleLock singleLock(&m_critSection,TRUE);
	return m_dCombSZ;
}

double CCombCache::GetCombBuyPKSZ()
{
	CSingleLock singleLock(&m_critSection,TRUE);
	return m_dCombBuyPKSZ;
}

double CCombCache::GetCombSellPKSZ()
{
	CSingleLock singleLock(&m_critSection,TRUE);
	return m_dCombSellPKSZ;
}


//////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC( CCombCacheSet, CObject )

CCombCacheSet::CCombCacheSet()
{
	m_bStopCalculate = TRUE;
	m_pCalThread = NULL;
}


CCombCacheSet::~CCombCacheSet()
{
	Clear();
}

void CCombCacheSet::Clear()
{
	CString strKey;
	CCombCache* pCache = NULL;
	for(POSITION pos = m_mapCombCache.GetStartPosition(); pos != NULL; )
	{
		m_mapCombCache.GetNextAssoc(pos,strKey,(CObject*&) pCache);
		if(pCache)
			delete pCache;
	}
	m_mapCombCache.RemoveAll();

	StockUserInfo* pStkInfo = NULL;
	for(POSITION pos = m_mapStkInfoCache.GetStartPosition(); pos != NULL; )
	{
		m_mapStkInfoCache.GetNextAssoc(pos,strKey,(CObject*&) pStkInfo);
		if(pStkInfo)
			delete pStkInfo;
	}
	m_mapStkInfoCache.RemoveAll();
}

//IDataSourceSinkʵ��
BOOL CCombCacheSet::HSDataSourceSink_OnCommNotify(void* pData)
{
	if (pData == NULL)
	{
		return FALSE;
	}
	CommNotify* pNotify = (CommNotify*)pData;
	// ����������Ҫ��������һ��
	if (pNotify->m_uType == eDataSource_ReConnect)
	{
		this->RequestRealTimeData(pNotify->m_svrType);
	}
	else if( pNotify->m_uType == eDataSource_Init )
	{
		this->RequestRealTimeData();
	}
	return TRUE;
}

BOOL CCombCacheSet::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{
	this->OnUpdateData(pData,nLen);
	return TRUE;
}

//��ʼ������Ϣ��Ϣ
BOOL CCombCacheSet::InitCacheSet()
{
	if( m_pDataSourceEx == NULL )
	{
		m_pDataSourceEx = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
		if (m_pDataSourceEx && !IsBadReadPtr(m_pDataSourceEx,1))
		{
			m_nDataSourceID = m_pDataSourceEx->HSDataSource_DataSouceInit(this,NULL);
		}
	}
	ReflashCombCacheSet();
	RequestRealTimeData();
	return TRUE;
}

void CCombCacheSet::ReflashCombCacheSet()
{
	ICombManager *pCombManager = (ICombManager*)CWinnerApplication::GetObject(_T("AnalysisCore.CCombManager"));
	if(pCombManager)
	{
		pCombManager->LoadCombs();
		for( int i = 0; i < pCombManager->GetCombCount(); i++ )
		{
			ICombInfo* pCombInfo = (ICombInfo*)pCombManager->GetComb(i);
			CCombCache* pCache = new CCombCache(this);
			pCache->SetDataSourceEx(this->m_pDataSourceEx);
			pCache->InitCombCache(pCombInfo);
			m_mapCombCache.SetAt(pCombInfo->GetCombName(),(CObject*)pCache);
		}
	}
}

//���󲢶�����������
BOOL CCombCacheSet::RequestRealTimeData(Server_type svrType /*= CEV_Connect_ALL*/)
{
	//ȡ��������еĴ�����Ϣ
	CMapStringToString  codeMap;
	CArray<StockUserInfo> ayStockCodeinfo;
	CArray<StockUserInfo> ayFutureCodeinfo;
	CArray<StockUserInfo> ayIndexCodeinfo;
	POSITION pos;
	CString strKey;
	CCombCache* pCache = NULL;
	for(pos = m_mapCombCache.GetStartPosition()  ;pos != NULL;)
	{
		m_mapCombCache.GetNextAssoc(pos,strKey,(CObject*&)pCache);
		CArray<StockUserInfo,StockUserInfo> ayStkCode;
		ASSERT(pCache!=NULL);
		pCache->GetAllComponentCode(ayStkCode);
		for( int i = 0; i < ayStkCode.GetCount(); i++)
		{	
			StockUserInfo stockInfo = ayStkCode.GetAt(i);

			CString strKey = stockInfo.GetCode();
			CString strValue;
			if( !codeMap.Lookup(strKey,strKey) )
			{
				codeMap.SetAt(strKey,strKey);
				//��¼������Ϣ
				StockUserInfo *pStkInfo = new StockUserInfo();
				memcpy(pStkInfo,&stockInfo,sizeof(stockInfo));
				m_mapStkInfoCache.SetAt(strKey,(CObject*&)pStkInfo);

				if( MakeIndexMarket(stockInfo.m_ciStockCode.m_cCodeType ) ) //ָ������
				{			
					ayIndexCodeinfo.Add(stockInfo);	
				}
				else if(MakeMarket(stockInfo.m_ciStockCode.m_cCodeType ) == FUTURES_MARKET)//�ڻ�����
				{
					ayFutureCodeinfo.Add(stockInfo);	
				}
				else if(MakeMarket(stockInfo.m_ciStockCode.m_cCodeType ) == STOCK_MARKET)//��Ʊ
				{
					ayStockCodeinfo.Add(stockInfo);
				}			
			}
		}
	}

	if(svrType == CEV_Connect_ALL ||  svrType == CEV_Connect_HQ)
	{
		//����ָ��
		RequestRealTimeData(ayIndexCodeinfo,RT_REALTIME,CEV_Connect_HQ);
		//�����Ʊ
		RequestRealTimeData(ayStockCodeinfo,RT_REALTIME_EXT,CEV_Connect_HQ);
	}
	if(svrType == CEV_Connect_ALL ||  svrType == CEV_Connect_QH)
	{
		//�����ڻ�
		RequestRealTimeData(ayFutureCodeinfo,RT_REALTIME,CEV_Connect_QH);
	}
	
	//�ڼ����߳�����ǰ���ȼ���һ�β�ѯ������������
	this->CalculateCombCache();

	return TRUE;
}

void CCombCacheSet::RequestRealTimeData(const CArray<StockUserInfo> &ayCodeinfo,unsigned short nType,char	cOperator)
{
	int nCount = ayCodeinfo.GetCount();
	if(nCount <= 0)
		return;

	int lRetLen = sizeof(AskData) + sizeof(CodeInfo)*(nCount-1);
	AskData*  paskData = (AskData*) new char[lRetLen];
	memset(paskData, 0, lRetLen);

	paskData->m_cOperator = cOperator;
	paskData->m_nType  = nType;
	paskData->m_nSize  = nCount;
	CodeInfo* pCodeInfo = paskData->m_pCode;
	for(int i =0 ; i < nCount; i++)
	{
		StockUserInfo stk = ayCodeinfo.GetAt(i);
		memcpy(&pCodeInfo[i], &stk.m_ciStockCode,sizeof(CodeInfo));
	}

	//��ѯ����
	m_pDataSourceEx->HSDataSource_RequestAsyncData(m_nDataSourceID, (char*)paskData, lRetLen, 0);
	int nAnsLen = 0;
	void* pAnsData = m_pDataSourceEx->HSDataSource_RequestSyncData(m_nDataSourceID,(char*)paskData,lRetLen,Query_Quote_Data_Time_Out,nAnsLen);
	if( pAnsData != NULL && nAnsLen > 0)
	{
		this->OnUpdateData(pAnsData,nAnsLen);
	}

	//����
	if(RT_REALTIME_EXT == paskData->m_nType)
		paskData->m_nType = RT_AUTOPUSH_EXT;
	else
		paskData->m_nType = RT_AUTOPUSH;
	m_pDataSourceEx->HSDataSource_RequestAsyncData(m_nDataSourceID, (char*)paskData, lRetLen, 0);

	delete[] (char*)paskData;
}

void CCombCacheSet::OnUpdateData(void* pData, int nLen)
{
	DataHead* pHead = (DataHead*)pData;
	if (pHead->m_nType == RT_REALTIME_EXT ||  pHead->m_nType == RT_AUTOPUSH_EXT)
	{
		AnsRealTime* pNowData = (AnsRealTime*)pData;
		if (pNowData->m_nSize > 0)
		{
			HandleNowDataExt(pHead,pNowData->m_pnowData,pNowData->m_nSize);
		}
	}
	else if(pHead->m_nType == RT_REALTIME ||  pHead->m_nType == RT_AUTOPUSH)
	{
		AnsRealTime* pNowData = (AnsRealTime*)pData;
		if (pNowData->m_nSize > 0)
		{
			HandleNowData(pHead,pNowData->m_pnowData,pNowData->m_nSize);
		}
	}

	CString strKey;
	CCombCache* pCache = NULL;
	for(POSITION pos = m_mapCombCache.GetStartPosition(); pos != NULL; )
	{
		m_mapCombCache.GetNextAssoc(pos,strKey,(CObject*&) pCache);
		if(pCache)
		{
			pCache->OnUpdateData(pHead);
		}
	}

	m_event.SetEvent();
}

void CCombCacheSet::ConvertRealTimeToRealTimeExt(CodeInfo ci,ShareRealTimeData* pCurrentRealTime,ShareRealTimeData_Ext &rtdExt)
{
	if( MakeIndexMarket(ci.m_cCodeType ) ) //ָ������	
		memcpy(&rtdExt.m_indData,&pCurrentRealTime->m_indData,sizeof(pCurrentRealTime->m_indData));
	else if(MakeMarket(ci.m_cCodeType ) == FUTURES_MARKET)//�ڻ�����
		memcpy(&rtdExt.m_qhData,&pCurrentRealTime->m_qhData,sizeof(pCurrentRealTime->m_qhData));
	else if(MakeMarket(ci.m_cCodeType ) == STOCK_MARKET)//��Ʊ
		memcpy(&rtdExt.m_nowDataExt.m_stockRealTime,&pCurrentRealTime->m_nowData,sizeof(pCurrentRealTime->m_nowData));
}

void CCombCacheSet::HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData* pData = (CommRealTimeData*)pnowData;
	CommRealTimeData* pNextData = NULL;

	StockUserInfo* pStock = NULL;
	int nPos = 0;
	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		ShareRealTimeData* pCurrentRealTime =  (ShareRealTimeData*)pData->m_cNowData;

		ShareRealTimeData_Ext rtdExt;
		memset(&rtdExt,0,sizeof(rtdExt));
		this->ConvertRealTimeToRealTimeExt(pData->m_ciStockCode,pCurrentRealTime,rtdExt);

		CString strCode = pData->m_ciStockCode.GetCode();
		CodeShareRealTime* pCodeShareTimeData = NULL;

		if( m_mapQuoteCache.Lookup(strCode,(CObject*&)pCodeShareTimeData) == TRUE )
		{
			memcpy(&pCodeShareTimeData->m_shareRealTime,&rtdExt,sizeof(ShareRealTimeData_Ext));		
		}
		else
		{
			pCodeShareTimeData = new CodeShareRealTime(); 
			memcpy(&pCodeShareTimeData->m_ciStockCode,&pData->m_ciStockCode,sizeof(pData->m_ciStockCode));		
			memcpy(&pCodeShareTimeData->m_shareRealTime,&rtdExt,sizeof(ShareRealTimeData_Ext));		
			m_mapQuoteCache.SetAt(strCode,(CObject*)pCodeShareTimeData);
		}
		pData = pNextData;		
	}
}

void CCombCacheSet::HandleNowDataExt(DataHead* pHead, const CommRealTimeData* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData_Ext* pData = (CommRealTimeData_Ext*)pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	StockUserInfo* pStock = NULL;
	int nPos = 0;
	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		ShareRealTimeData_Ext* pCurrentRealTimeExt =  (ShareRealTimeData_Ext*)pData->m_cNowData;
		CString strCode = pData->m_ciStockCode.GetCode();
		CodeShareRealTime* pCodeShareTimeData = NULL;
		if( m_mapQuoteCache.Lookup(strCode,(CObject*&)pCodeShareTimeData) == TRUE )
		{
			memcpy(&pCodeShareTimeData->m_shareRealTime,pCurrentRealTimeExt,sizeof(ShareRealTimeData_Ext));		
		}
		else
		{
			pCodeShareTimeData = new CodeShareRealTime(); 
			memcpy(&pCodeShareTimeData->m_ciStockCode,&pData->m_ciStockCode,sizeof(pData->m_ciStockCode));		
			memcpy(&pCodeShareTimeData->m_shareRealTime,pCurrentRealTimeExt,sizeof(ShareRealTimeData_Ext));		
			m_mapQuoteCache.SetAt(strCode,(CObject*)pCodeShareTimeData);
		}
		pData = pNextData;		
	}
}

BOOL CCombCacheSet::DoCalculateWork(BOOL bForceCal /*= FALSE*/)
{
	if(m_bStopCalculate)
	{
		m_event.ResetEvent();
		return FALSE;
	}

	if(!bForceCal)
		::WaitForSingleObject(m_event.m_hObject, INFINITE); 

	CSingleLock singleLock(&m_critSection,TRUE);

	//��ϻ������ 
	CalculateCombCache();
	//�ⲿ֪ͨ
	DoExternalNotify();

	m_event.ResetEvent();
	return TRUE;
}

void CCombCacheSet::CalculateCombCache()
{
	CString strKey;
	CCombCache* pCache = NULL;
	for(POSITION pos = m_mapCombCache.GetStartPosition(); pos != NULL; )
	{
		m_mapCombCache.GetNextAssoc(pos,strKey,(CObject*&) pCache);
		if(pCache)
		{
			pCache->DoCalculateWork();
		}
	}
}

void CCombCacheSet::DoExternalNotify()
{
	for(int i = 0; i < m_ayNoifiers.GetCount(); i++)
	{
		m_ayNoifiers.GetAt(i)->OnCombCacheDataUpdate();
	}
}

void CCombCacheSet::StartCalculateWork(BOOL bStart /*= TRUE*/)
{
	if(bStart)
	{
		if(m_pCalThread == NULL)
		{
			m_bStopCalculate = FALSE;
			m_pCalThread = AfxBeginThread(DoCalculateWorkThreadProc,(LPVOID)this, 0, 0, CREATE_SUSPENDED, NULL);
			m_pCalThread->m_bAutoDelete = TRUE; 
			m_pCalThread->ResumeThread();
		}	
	}
	else
	{
		m_bStopCalculate = TRUE;
		m_event.SetEvent();
		if(m_pCalThread)
		{
			 ::WaitForSingleObject(m_pCalThread->m_hThread, INFINITE); 
			 delete m_pCalThread;
		}
		 m_pCalThread = NULL;
	}
}

void CCombCacheSet::RegisterCombCacheNotifier(ICombCacheNotifier *pNotifier)
{
	CSingleLock singleLock(&m_critSection,TRUE);

	for( int i = 0; i < m_ayNoifiers.GetCount(); i++ )
	{
		if ( pNotifier == m_ayNoifiers.GetAt(i) )
			return;
	}
	m_ayNoifiers.Add(pNotifier);
}

void CCombCacheSet::UnregisterCombCacheNotifier(ICombCacheNotifier *pNotifier)
{
	CSingleLock singleLock(&m_critSection,TRUE);

	for( int i = 0; i < m_ayNoifiers.GetCount(); i++ )
	{
		if ( pNotifier == m_ayNoifiers.GetAt(i) )
		{
			m_ayNoifiers.RemoveAt(i);
			return;
		}
	}
}

