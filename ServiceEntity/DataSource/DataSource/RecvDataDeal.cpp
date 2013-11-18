#include "stdafx.h"
#include "DataSourceImpl.h"
#include "DataCache.h"
#include "ProgressDlg.h"
#include "..\..\HelperLib\GeneralHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif 
//////////////////////////////////////////////////////////////////////////
//


int smartcmp(const void* pBuff1,const void* pBuff2,short count)
{
	int nRet = 0;
	switch(count)
	{
	case QS_INT:
		nRet = (*(int*)pBuff1 == *(int*)pBuff2) ? 0 :
			(*(int*)pBuff1 >  *(int*)pBuff2) ? 1 : -1;
		break;		
	case QS_UNINT:
		nRet = (*(unsigned int*)pBuff1 == *(unsigned int*)pBuff2) ? 0 :
			(*(unsigned int*)pBuff1 >  *(unsigned int*)pBuff2) ? 1 : -1;
		break;		
	case QS_LONG:
		nRet = (*(long*)pBuff1 == *(long*)pBuff2) ? 0 :
			(*(long*)pBuff1 >  *(long*)pBuff2) ? 1 : -1;
		break;		
	case QS_UNLONG:
		nRet = (*(unsigned long*)pBuff1 == *(unsigned long*)pBuff2) ? 0 :
			(*(unsigned long*)pBuff1 >  *(unsigned long*)pBuff2) ? 1 : -1;
		break;		
	case QS_SHORT:
		nRet = (*(short*)pBuff1 == *(short*)pBuff2) ? 0 :
			(*(short*)pBuff1 >  *(short*)pBuff2) ? 1 : -1;
		break;		
	case QS_UNSHORT:

		nRet = (*(unsigned short*)pBuff1 == *(unsigned short*)pBuff2) ? 0 :
			(*(unsigned short*)pBuff1 >  *(unsigned short*)pBuff2) ? 1 : -1;
		break;		
	case QS_FLOAT:
		nRet = (*(float*)pBuff1 == *(float*)pBuff2) ? 0 :
			(*(float*)pBuff1 >  *(float*)pBuff2) ? 1 : -1;
		break;		
	case QS_DOUBLE:
		nRet = (*(double*)pBuff1 == *(double*)pBuff2) ? 0 :
			(*(double*)pBuff1 >  *(double*)pBuff2) ? 1 : -1;
		break;		
	default:
		if(count > 0)
		{
			const void* p;
			short sLen,sMin = count;
			if((p = memchr(pBuff1,'\0',count)) != NULL)
			{
				sLen = (char*)p - (char*)pBuff1;
				if((sLen > 0) && (sLen < sMin))
				{
					sMin = sLen;
				}	
			}
			if((p = memchr(pBuff2,'\0',count)) != NULL)
			{
				sLen = (char*)p - (char*)pBuff2;
				if((sLen > 0) && (sLen < sMin))
				{
					sMin = sLen;
				}	
			}
			nRet = memcmp(pBuff1,pBuff2,sMin);
		}
		break;					
	}
	return nRet;
}

void QuickSortAscend2(struct KQuickSort* pQSData)
{
	long i,j,l,r,top = 0,PivotIndex = 0;

	if(pQSData == NULL) return;
	i = pQSData->m_nLow;
	j = pQSData->m_nHigh;
	int iSize = j - i + 1;

	short  nDataLen = pQSData->m_nDataLen;
	short  nKeyLen  = pQSData->m_nKeyLen;

	if(nDataLen <= 0 || iSize <= 1) return;

	char* pivot = NULL;
	char* pKey	= (char*)pQSData->m_pData + pQSData->m_nKeyOffset;
	char* pData = (char*)pQSData->m_pData;

	char* pTemp = new char[nDataLen];

	if(iSize == 2)
	{
		if(smartcmp(pKey + i * nDataLen,pKey + j * nDataLen,nKeyLen) > 0)
		{
			memcpy(pTemp,pData + j * nDataLen,nDataLen);
			memcpy(pData + j * nDataLen,pData + i * nDataLen,nDataLen);
			memcpy(pData + i * nDataLen,pTemp,nDataLen);
		}
		delete[] pTemp;
		return;
	}

	static short nStackSize = 1000;
	Qentry* pStack = new Qentry[nStackSize];
	pStack->left  = i;
	pStack->right = j;
	while(top >= 0)
	{
		if( top >= nStackSize )
			break;

		//Pop Stack
		i = (pStack + top)->left;
		j = (pStack + top--)->right;

		//Find Pivot
		PivotIndex = (i + j) >> 1;

		pivot = pKey + PivotIndex * nDataLen;

		//Stick pivot at end
		memcpy(pTemp,pData + PivotIndex * nDataLen,nDataLen);
		memcpy(pData + PivotIndex * nDataLen,pData + j * nDataLen,nDataLen);
		memcpy(pData + j * nDataLen,pTemp,nDataLen);

		//Partition
		l = i - 1;
		r = j;
		do
		{
			while(smartcmp((char*)(pKey + (++l) * nDataLen),pivot,nKeyLen) < 0);
			while(r && smartcmp((char*)(pKey + (--r) * nDataLen),pivot,nKeyLen) > 0);
			memcpy(pTemp,pData + l * nDataLen,nDataLen);
			memcpy(pData + l * nDataLen,pData + r * nDataLen,nDataLen);
			memcpy(pData + r * nDataLen,pTemp,nDataLen);
		} while ( l < r );

		//==============================
		//Undo final swap
		memcpy(pTemp,pData + l * nDataLen,nDataLen);
		memcpy(pData + l * nDataLen,pData + r * nDataLen,nDataLen);
		memcpy(pData + r * nDataLen,pTemp,nDataLen);
		//==============================
		//Put pivot value in place
		memcpy(pTemp,pData + l * nDataLen,nDataLen);
		memcpy(pData + l * nDataLen,pData + j * nDataLen,nDataLen);
		memcpy(pData + j * nDataLen,pTemp,nDataLen);
		//==============================		
		if(l - i > 9)
		{
			top++;
			if( top >= nStackSize )
				break;

			(pStack + top)->left = i;
			(pStack + top)->right = l - 1;
		}
		if(j - l > 9)
		{
			top++;					
			if( top >= nStackSize )
				break;

			(pStack + top)->left = l + 1;
			(pStack + top)->right = j;
		}
	}

	delete[] pStack;

	//Insertion Sort
	for(i = pQSData->m_nLow;i < pQSData->m_nHigh + 1;i ++)
		for(j = i;(j > pQSData->m_nLow) && (smartcmp((char*)(pKey + j * nDataLen),(char*)(pKey + (j - 1) * nDataLen),nKeyLen) < 0);j --)
		{
			memcpy(pTemp,pData + j * nDataLen,nDataLen);
			memcpy(pData + j * nDataLen,pData + (j - 1) * nDataLen,nDataLen);
			memcpy(pData + (j  - 1) * nDataLen,pTemp,nDataLen);
		}

		delete[] pTemp;		
}


int CDataSourceImpl::DataProcessing(PacketInfo* pRecv, CReplyDataManger& manager)
{
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if (pHead != NULL && !::IsBadReadPtr(pHead, sizeof(DataHead)))
	{
		if( pHead->m_nType == RT_INITIALINFO)
		{
			if ( m_bInitInfo && m_pWndProgressDlg == NULL)
			{
				m_pWndProgressDlg = new CProgressDlg;
				m_pWndProgressDlg->Create( NULL);
				m_pWndProgressDlg->CenterWindow();
				m_pWndProgressDlg->ShowWindow(SW_SHOW);	
			}	
			if ( m_pWndProgressDlg )
			{
				m_pWndProgressDlg->SetText("正在处理代码....");
				m_pWndProgressDlg->SetPos(0);
			}
			m_bIniting = FALSE;
			int nRet = DataProcess_Init(pRecv, manager);
			if ( m_pWndProgressDlg )
			{
				m_pWndProgressDlg->DestroyWindow();
				delete m_pWndProgressDlg;
				m_pWndProgressDlg = NULL;
			}
			
// 			if (m_pWndDataRecv != NULL)
// 			{
// 				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_servercalc);
// 			}
// 			char cSvrType = m_pChannels->GetServerTypeByHandle(pRecv->m_lHandle);
// 			m_autoRequest.AutoRequestServerCalc(cSvrType);
			char cSvrType = m_pChannels->GetServerTypeByHandle(pRecv->m_lHandle);
			if(cSvrType == CEV_Connect_HQ)
 				m_stockManager.SetFinanceDataToMap();

			if (m_pWndDataRecv != NULL)
			{
				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_succ);
			}
			
			//9点初始化时,通知外部模块重新初始化
			if ( m_bInitInfo)
			{
				CommNotify notify;
				notify.m_uType = eDataSource_Init;
				m_datasinkconnectManager.NotifyCommStatus(&notify, -1);
			}
			m_bIniting = TRUE;
			m_bInitInfo = TRUE;
			return nRet;
		}
		else if (pHead->m_nType == RT_SEVER_CALCULATE)
		{
			int nRet = DataProcess_ServerCalc(pRecv, manager);
			if (m_pWndDataRecv != NULL)
			{
				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_succ);
			}
			return nRet;
		}
		else if ( RT_REALTIME_EXT == pHead->m_nType || RT_AUTOPUSH_EXT == pHead->m_nType)
		{
			return DataProcess_RealTimeExt(pRecv, manager);
		}
		else if (RT_REALTIME ==  pHead->m_nType   ||  RT_AUTOPUSH == pHead->m_nType )
		{
			return DataProcess_RealTime(pRecv, manager);
		}
		else if (RT_FILEREQUEST == pHead->m_nType )
		{
			AnsTextData* pText = (AnsTextData*)pHead;
			TextMarkData* pMarkData = &pText->m_sMarkData;
			CString strFileName = pMarkData->m_cFilePath;
//			if( !strFileName.CompareNoCase(DYN_NEWS_INI_FILE) )
			{
				m_dsWriteFile.WriteRequestFile(strFileName, pText->m_cData, pText->m_nSize);
			}
			if (m_pWndDataRecv != NULL)
			{
				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_config);
			}
		}
		else if ((RT_EXRIGHT_DATA | RT_JAVA_MARK) == pHead->m_nType)
		{
			AnsTextData* pText = (AnsTextData*)pHead;
			m_dsWriteFile.WriteAnsExrightData(pText->m_cData, pText->m_nSize);
			if (m_pWndDataRecv != NULL)
			{
				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_exright);
			}
		}
		else if ((RT_CURRENTFINANCEDATA | RT_JAVA_MARK) == pHead->m_nType)
		{
			AnsTextData* pText = (AnsTextData*)pHead;
			m_dsWriteFile.WriteAnsFinanceData(pText->m_cData, pText->m_nSize);
			if (m_pWndDataRecv != NULL && pText->m_nSize > 0)
			{
				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_currentfinancedata);
			}
		}
		else if (RT_BLOCK_DATA == pHead->m_nType)
		{
			AnsTextData* pText = (AnsTextData*)pHead;
			m_dsWriteFile.WriteAnsSysBlockData(pText->m_cData, pText->m_nSize);
			if (m_pWndDataRecv != NULL)
			{
				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_blockdata);
			}
		}
		else if (RT_USER_BLOCK_DATA == pHead->m_nType)
		{
			if (m_pWndDataRecv != NULL)
			{
				m_pWndDataRecv->PostMessage(WM_THREAD_STATUS, 0, InitStatus_userblockdata);
			}
		}
		else if (RT_SERVERINFO == pHead->m_nType)
		{
			AnsServerInfo* pServerInfo = (AnsServerInfo*)pHead;
			//暂时不需要处理
		}
		else if (RT_SRV_SYNC == pHead->m_nType)
		{
			AnsSrvSync *pData = (AnsSrvSync *)pHead;
		}
		else if (RT_TECHDATA_EX == pHead->m_nType) //盘后分析
		{
			return  DataProcess_TechDataEx(pRecv, manager);
		}
		else if ( RT_HISTREND == pHead->m_nType || RT_TREND_EXT == pHead->m_nType   //分时走势，历史分时，tick，资讯
			|| RT_TICK ==  pHead->m_nType ||   RT_STOCKTICK == pHead->m_nType ||  RT_BYINDEXRETDATA == pHead->m_nType  ||
				RT_TEXTDATAWITHINDEX_NEGATIVE == pHead->m_nType || 	 RT_TEXTDATAWITHINDEX_PLUS == pHead->m_nType)  
		{
			return DataProcess_CacheDataEx(pRecv, manager);
		}
		else if ( RT_SERVERTIME == pHead->m_nType )
		{
		   return DataProcess_ServerTimeEx(pRecv, manager);
		}
		else 
		{
			return DataProcess_UnDeal(pRecv, manager);
		}
	}
	return 0;
}

int CDataSourceImpl::DataProcess_Init(PacketInfo* pRecv, CReplyDataManger& manager)
{	
	DataHead* pHead = (DataHead*)pRecv->m_pszData;	
	long lOffset = (long)&((AnsInitialData*)(NULL))->m_sOneMarketData;
	BOOL bRemoveStock = FALSE;
	long nSize = 0;
	if( pRecv != NULL &&
		pRecv->m_pszData != NULL && 
		pRecv->m_lDataTotal > lOffset && 
		pRecv->m_lDataTransmited != 0 )
	{
		AnsInitialData* pAnsInitialData = (AnsInitialData*)pHead;
		OneMarketData* pOneMarketData;

		char* ppData = (char*)pAnsInitialData->m_sOneMarketData;
		pOneMarketData = (OneMarketData*)ppData;		
		int nStockCount = 0;
		char* pCurData = NULL;
		int nBegin = 0;
	
		m_stockManager.RemoveStockInfoEx();
		// 分市场处理	
		TRACE("\r\n  initing.......\r\n");
		for( int i = 0; i < pAnsInitialData->m_nSize; i++ )
		{
			TRACE("\r\n index is %d\r\n", i);
			//处理市场
			CommBourseInfo* pCommBourse = &pOneMarketData->m_biInfo;
  			 if ( m_bourseManager.RemoveBourse(pCommBourse->m_nMarketType) )
  			 {
  				  m_stockManager.RemoveStocksByMarket(pCommBourse->m_nMarketType);
  				  bRemoveStock = TRUE;
  			 }
			BourseInfo* pBourse = new BourseInfo;
			memset(pBourse, 0, sizeof(BourseInfo));
			pBourse->m_nMarketType = pCommBourse->m_nMarketType;
			pBourse->m_dwCRC = pCommBourse->m_dwCRC;
			pBourse->m_lDate = pCommBourse->m_lDate;
			pBourse->m_cType = pCommBourse->m_cCount;

			StockType* pStockType = pCommBourse->m_stNewType;
			if (pCommBourse->m_cCount < STOCKTYPE_COUNT)
			{
				for (int i = 0; i < pCommBourse->m_cCount; i ++)
				{				
					StockType* pStockTypeCur = pStockType + i;
					StockType* pStockTypeNew = &pBourse->m_stNewType[i];
					memcpy(pStockTypeNew, pStockTypeCur, sizeof(StockType));
					TRACE("\r\n market type is %x", pStockTypeCur->m_nStockType);
				}
			}
			m_bourseManager.PushBourse(pBourse);

			// 处理股票    得到股票总数
			pCurData = ppData;
			pCurData += sizeof(CommBourseInfo) + (pOneMarketData->m_biInfo.m_cCount - 1) * sizeof(StockType);
			nSize += sizeof(CommBourseInfo) + (pOneMarketData->m_biInfo.m_cCount - 1) * sizeof(StockType);
			nStockCount = *(short*)pCurData;
			pBourse->m_nTotalCount = nStockCount;
			if (m_lServerTime <= 0 && nStockCount > 0)
			{
				m_lServerTime = pCommBourse->m_lDate;
			}
			StockInitInfo* pStockInitInfo = (StockInitInfo*)(pCurData + sizeof(int));
			nSize += sizeof(int);
			for (int j = 0; j < nStockCount; j ++)
			{
				CHSDSStockInfoIntact* pStockInfoIntact = new CHSDSStockInfoIntact;

				pStockInfoIntact->m_stockInfo.m_ciStockCode.m_cCodeType = pStockInitInfo->m_ciStockCode.m_cCodeType;
				strncpy(pStockInfoIntact->m_stockInfo.m_ciStockCode.m_cCode, pStockInitInfo->m_ciStockCode.m_cCode, min(sizeof(pStockInfoIntact->m_stockInfo.m_ciStockCode.m_cCode), strlen(pStockInitInfo->m_ciStockCode.m_cCode)));
				memcpy(&pStockInfoIntact->m_stockInfo.m_cStockName, pStockInitInfo->m_cStockName, sizeof(pStockInfoIntact->m_stockInfo.m_cStockName));
				pStockInfoIntact->m_stockInfo.m_lPrevClose = pStockInitInfo->m_lPrevClose;
				pStockInfoIntact->m_stockInfo.m_l5DayVol = pStockInitInfo->m_l5DayVol;
				short nHand = m_stockManager.GetStockHand(pStockInitInfo->m_ciStockCode.m_cCodeType);
				pStockInfoIntact->m_stockInfo.m_nHandNum = nHand;
				short nPriceUnit = 0, nPriceDecimal = 0;
				m_bourseManager.GetStockPriceUnitDecimal(pStockInfoIntact->m_stockInfo.m_ciStockCode.m_cCodeType, nPriceUnit, nPriceDecimal);
				pStockInfoIntact->m_stockInfo.m_nPriceUnit = nPriceUnit;
				pStockInfoIntact->m_stockInfo.m_nDecimal = nPriceDecimal;
				/*pStockInfoIntact->m_stockInfo.m_fUpPrice = */
				pStockInfoIntact->m_lSize = 0;
				pStockInfoIntact->m_pData = NULL;

				pStockInitInfo ++;
				nSize += sizeof(StockInitInfo);
				m_stockManager.PushStock(pStockInfoIntact);

				int nPos = (nSize*100)/(pRecv->m_lDataTotal-lOffset);
				if ( m_pWndProgressDlg->GetSafeHwnd() && m_pWndProgressDlg->IsWindowVisible() )
				{
					m_pWndProgressDlg->SetPos(nPos);
				}
			}
			// 下一个市场
			ppData += sizeof(CommBourseInfo) + (pOneMarketData->m_biInfo.m_cCount - 1) * sizeof(StockType);
			ppData += ( sizeof(OneMarketData) - sizeof(CommBourseInfo) + (*(short*)ppData - 1) * sizeof(StockInitInfo) );

			pOneMarketData = (OneMarketData*)ppData;
		}
	}
 	if ( bRemoveStock )
 	{
 		m_stockManager.RemoveStockInfoEx();
 		m_stockManager.CreateIndex();
 	}

	m_bInitFinished = TRUE;
	return TRUE;
}

int CDataSourceImpl::DataProcess_Login(PacketInfo* pRecv, CReplyDataManger& manager)
{
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	AnsLogin* pLogin = (AnsLogin*)pHead;
	char* pCurData = &pLogin->m_szRet[pLogin->m_nSize];  //
	TestSrvInfoData* pInfoData = (TestSrvInfoData*)pCurData;

	short* pSrvCfg = (short*)&pLogin->m_dhHead.m_nPrivateKey;

	if (pInfoData != NULL && pInfoData->m_lSrvDate > 0 && pInfoData->m_lSrvTime > 0)
	{
		CTime tmcur = CTime::GetCurrentTime();
		long lCurDate = atol(tmcur.Format("%Y%m%d"));
		m_nServerDateSpan = lCurDate - pInfoData->m_lSrvDate;

		long lCurTime = atol(tmcur.Format("%H%M%S"));
		m_nServerTimeSpan = lCurTime - pInfoData->m_lSrvTime;
	}
	CString strHtmlFileName;
	CStringArray arStr;
	m_dsWriteFile.WriteAnsLoginData(pLogin, strHtmlFileName, arStr);
	return 0;
}

int	CDataSourceImpl::DataProcess_TechDataEx(PacketInfo* pRecv, CReplyDataManger& manager)
{	
	AnsDayDataEx*  pDayData  = (AnsDayDataEx*)pRecv->m_pszData;
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if(pDayData->m_nSize <= 0)
	{
		return FALSE;
	}
	TRACE("\r\n DataProcess_TechDataEx key is %d", pHead->m_lKey);
	void* pBeginBuffer=NULL;
	if(pDayData->m_nSize > 0)
	{
		CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, (CodeInfo*)&pHead->m_nPrivateKey/*, pHead->m_lKey*/);
		if (pDataSubscription != NULL)
		{
			if ( pHead->m_lKey == PERIOD_TYPE_MINUTE1)
				pDataSubscription->m_dwTickRequest = GetTickCount();
 			char* buffer = NULL;
			long nSize = 0;
			//服务器重复错误数据处理
		//	else
			{
  				KQuickSort qsData;
  				qsData.m_pData      = pDayData->m_sdData;
  				qsData.m_nDataLen   = sizeof(StockCompDayDataEx);
  				qsData.m_nKeyOffset = 0;
  				qsData.m_nKeyLen    = QS_LONG;
  				qsData.m_nLow       = 0;
  				qsData.m_nHigh      = pDayData->m_nSize - 1;
  				QuickSortAscend2(&qsData);
  			
  				StockDay * pPreTemp = NULL;
  				StockCompDayDataEx* pStockDay = (StockCompDayDataEx*)(pDayData->m_sdData);
  				CArray<StockCompDayDataEx*,StockCompDayDataEx*> ayStock;
  				int nCount = pDayData->m_nSize ;
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
  				nSize = ayStock.GetCount()*sizeof(StockCompDayDataEx);
  				buffer = new char[nSize];
  				pBeginBuffer = buffer;
  				memset(buffer, 0, nSize);
  				for (int i=0; i<ayStock.GetCount(); i++)
  				{							
  					memcpy((void*)buffer, (void*)ayStock.GetAt(i), sizeof(StockCompDayDataEx));
  					buffer += sizeof(StockCompDayDataEx);
  				}
  				nSize = ayStock.GetCount(); 		
 				
 			}
 			pDayData->m_nSize = nSize;
			manager.AddElementHead((char*)pRecv->m_pszData, sizeof(DataHead) + sizeof(long));
			for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
			{
				long lDataSourceSink = (long)*it;
				if ( pBeginBuffer )
				{
					manager.AddOneElement(lDataSourceSink, (char*)pBeginBuffer, nSize*sizeof(StockCompDayDataEx),FALSE);
					manager.ReplyToSink(m_pWndDataRecv->m_hWnd);
					m_DataCache.WriteDayData( (CodeInfo*)&pHead->m_nPrivateKey, pHead->m_lKey,(StockCompDayDataEx*)pBeginBuffer,nSize);
					manager.RemoveAllElement();	
					delete[] pBeginBuffer;
				}
				else
				{
					manager.AddOneElement(lDataSourceSink, (char*)pDayData->m_sdData, sizeof(StockCompDayDataEx) * pDayData->m_nSize,FALSE);
					manager.ReplyToSink(m_pWndDataRecv->m_hWnd);			
					m_DataCache.WriteDayData( (CodeInfo*)&pHead->m_nPrivateKey,  pHead->m_lKey,pDayData->m_sdData,pDayData->m_nSize );
					manager.RemoveAllElement();	
				}
			}	
			pDataSubscription->m_dwTickResponce = GetTickCount();
		}
	}
	return manager.GetSize();

}

void CDataSourceImpl::DataProcess_DiskDataEx(PacketInfo* pRecv)
{
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if(pRecv->m_lDataTotal <= 0)
	{
		return ;
	}
	CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, (CodeInfo*)&pHead->m_nPrivateKey/*, pHead->m_lKey*/);
	if ( pDataSubscription )
	{
		for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
		{
			long lDataSourceSink = (long)*it;	
			m_datasinkconnectManager.SendSinkData(lDataSourceSink, (void*)pRecv->m_pszData, pRecv->m_lDataTotal);
		}
	}	
}
//
int	CDataSourceImpl::DataProcess_CacheDataEx(PacketInfo* pRecv, CReplyDataManger& manager)
{
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if(pRecv->m_lDataTotal <= 0)
	{
		return FALSE;
	}	
	if(pRecv->m_lDataTotal > 0)
	{
		//服务器没有返回代码，增加按包id来查询订阅列表
		CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, (CodeInfo*)&pHead->m_nPrivateKey,pHead->m_lKey);
		if (pDataSubscription )
		{
			if ( pHead->m_nPrivateKey.m_pCode.GetCode().IsEmpty() && (pHead->m_nType == RT_TREND_EXT || pHead->m_nType == RT_HISTREND))
			{
				memcpy(&pHead->m_nPrivateKey.m_pCode, &pDataSubscription->m_codeInfo, sizeof(CodeInfo));
			}
			manager.AddElementHead((char*)pHead, sizeof(DataHead) + sizeof(long));
			
			for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
			{
				long lDataSourceSink = (long)*it;
				manager.AddOneElement(lDataSourceSink, (char*)((char*)pHead + sizeof(DataHead) + sizeof(long)), pRecv->m_lDataTotal - sizeof(DataHead) - sizeof(long));
			}
			pDataSubscription->m_dwTickResponce = GetTickCount();
		}
	}
	return manager.GetSize();
}
int	CDataSourceImpl::DataProcess_ServerTimeEx(PacketInfo* pRecv, CReplyDataManger& manager)
{
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if(pRecv->m_lDataTotal <= 0)
	{
		return FALSE;
	}
	manager.AddElementHead((char*)pHead, sizeof(DataHead) );
	if(pRecv->m_lDataTotal > 0)
	{
		CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, (CodeInfo*)&pHead->m_nPrivateKey);
		if (pDataSubscription == NULL)
		{
			pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, NULL);
		}
		if (pDataSubscription != NULL)
		{
			for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
			{
				long lDataSourceSink = (long)*it;
				manager.AddOneElement(lDataSourceSink, (char*)((char*)pHead + sizeof(DataHead) ), pRecv->m_lDataTotal - sizeof(DataHead) );
			}
			pDataSubscription->m_dwTickResponce = GetTickCount();
		}
		
	}
// 	AnsServerTime* pTime = (AnsServerTime*)pHead;
// 	long lTime = pTime->m_nDateTime;
// 	CTime t = CGeneralHelper::LongToDate(m_lServerTime);
// 	CTime time(t.GetYear(),t.GetMonth(),t.GetDay(), 9,30,0);
// 	if ( time.GetTime() - lTime >0 )
// 		m_DataCache.DelTraceFile();
	CTime t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%04d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay());
	m_lCurTime = atoi(strTime.GetBuffer());
	m_DataCache.SetDate(m_lCurTime, m_lServerTime);
	return manager.GetSize();
}
int	CDataSourceImpl::DataProcess_UnDeal(PacketInfo* pRecv, CReplyDataManger& manager)
{
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if(pRecv->m_lDataTotal <= 0)
	{
		return FALSE;
	}

	manager.AddElementHead((char*)pHead, sizeof(DataHead) + sizeof(long));
	if(pRecv->m_lDataTotal > 0)
	{
		CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, (CodeInfo*)&pHead->m_nPrivateKey);
		if (pDataSubscription == NULL)
		{
			pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, NULL);
		}
		if (pDataSubscription != NULL)
		{			
			for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
			{
				long lDataSourceSink = (long)*it;
				manager.AddOneElement(lDataSourceSink, (char*)((char*)pHead + sizeof(DataHead) + sizeof(long)), pRecv->m_lDataTotal - sizeof(DataHead) - sizeof(long));
			}
			pDataSubscription->m_dwTickResponce = GetTickCount();
		}
	}
	return manager.GetSize();
}

int CDataSourceImpl::DataProcess_RealTimeExt(PacketInfo* pRecv, CReplyDataManger& manager)
{
	AnsRealTime*  pNowData  = (AnsRealTime*)pRecv->m_pszData;
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if(pNowData->m_nSize <= 0)
	{
		return FALSE;
	}

	CommRealTimeData_Ext* pData = (CommRealTimeData_Ext*)pNowData->m_pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	int nNowSize = 0;
	int nSize = pNowData->m_nSize;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, &pData->m_ciStockCode);
		if (pDataSubscription != NULL)
		{
			manager.AddElementHead((char*)pRecv->m_pszData, sizeof(DataHead) + 2 * sizeof(short));
			
			for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
			{
				long lDataSourceSink = (long)*it;
				manager.AddOneElement(lDataSourceSink, (char*)pData, pData->GetAddSize() + pData->GetStructSize(&pData->m_ciStockCode));
			}
		}
		pData = pNextData;		
	}

	return manager.GetSize();
}
int	CDataSourceImpl::DataProcess_RealTime(PacketInfo* pRecv, CReplyDataManger& manager)
{
	AnsRealTime*  pNowData  = (AnsRealTime*)pRecv->m_pszData;
	DataHead* pHead = (DataHead*)pRecv->m_pszData;
	if(pNowData->m_nSize <= 0)
	{
		return FALSE;
	}

	CommRealTimeData* pData = (CommRealTimeData*)pNowData->m_pnowData;
	CommRealTimeData* pNextData = NULL;

	int nNowSize = 0;
	int nSize = pNowData->m_nSize;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}
		CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(pHead->m_nType, &pData->m_ciStockCode);
		if (pDataSubscription != NULL)
		{
			manager.AddElementHead((char*)pRecv->m_pszData, sizeof(DataHead) + 2 * sizeof(short));

			for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
			{
				long lDataSourceSink = (long)*it;
				manager.AddOneElement(lDataSourceSink, (char*)pData, pData->GetAddSize() + pData->GetStructSize(&pData->m_ciStockCode));
			}
		}
		pData = pNextData;		
	}
	return manager.GetSize();
}
int	CDataSourceImpl::DataProcess_ServerCalc(PacketInfo* pRecv, CReplyDataManger& manager)
{
	AnsSeverCalculate* pData  = (AnsSeverCalculate*)pRecv->m_pszData;

	m_stockManager.RecvServerCalcData(pData);

//	m_bourseManager.WriteLocalFile();
//	m_stockManager.WriteLocalFile();
	return 0;
}

BOOL CDataSourceImpl::InitMarketStocksInfo()
{
	//由于服务器每次都需要更新内容，所以暂时不读本地文件
//	return m_dsWriteFile.ReadComBouseData(&m_bourseManager) && m_dsWriteFile.ReadStocksData(&m_stockManager);
	return FALSE;
}