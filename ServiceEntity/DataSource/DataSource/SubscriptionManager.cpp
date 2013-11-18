//////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "SubscriptionManager.h"
#include "MapPreSubscrition.h"
#include "AutoRequest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
CSubscriptionManager::CSubscriptionManager()
{
	InitializeCriticalSection(&m_critical);
}

CSubscriptionManager::~CSubscriptionManager()
{
	RemoveAllSubscriptions();
	DeleteCriticalSection(&m_critical);
}


BOOL CSubscriptionManager::RemoveAllSubscriptions()
{
	EnterCriticalSection(&m_critical);
	while(m_listSubscription.size() > 0)
	{
		CDataSourceSubscription* pSubscription = m_listSubscription.back();	
		m_listSubscription.pop_back();
		if (pSubscription != NULL)
		{
			delete pSubscription;
			pSubscription = NULL;
		}
	}
	LeaveCriticalSection(&m_critical);
	return TRUE;
}

CDataSourceSubscription* CSubscriptionManager::GetSubscription(UINT uAskType, CodeInfo* pCodeInfo, UINT uPacketID )
{
	CDataSourceSubscription* pDataSubscription = NULL;
	EnterCriticalSection(&m_critical);
	if (m_listSubscription.size() > 0)
	{
		int nMaxSize = 0;
		for (std::list<CDataSourceSubscription*>::iterator it = m_listSubscription.begin(); it != m_listSubscription.end(); it ++)
		{
			CDataSourceSubscription* pSubscriptionTemp = (CDataSourceSubscription*)*it;
			if (pSubscriptionTemp->m_uAskType == uAskType )
			{
				//如果pCodeInfo == NULL, 需要查找到一个有最大模块使用数的订阅
				if(pCodeInfo == NULL || pCodeInfo->GetCode().IsEmpty())
				{
 					if ( uPacketID != 0 )
 					{
 						if ( pSubscriptionTemp->m_dwPacketID == uPacketID )
 						{
							pDataSubscription = pSubscriptionTemp;
							break;
 						}
 					}
					int nCurSinkSize = pSubscriptionTemp->m_listDataSinkID.size();
					if (nMaxSize < nCurSinkSize)
					{
						nMaxSize = nCurSinkSize;
						pDataSubscription = pSubscriptionTemp;
					}
				}
				else if(memcmp(pCodeInfo, &pSubscriptionTemp->m_codeInfo, sizeof(CodeInfo)) == 0)
				{
  					if ( uPacketID != 0 )
  					{
  						if ( pSubscriptionTemp->m_dwPacketID == uPacketID )
  						{
							pDataSubscription = pSubscriptionTemp;
							break;
  						}
  					}  				
					pDataSubscription = pSubscriptionTemp;
					break;
				
				}
			}
		}
	}
	LeaveCriticalSection(&m_critical);
	return pDataSubscription;
}

BOOL CSubscriptionManager::FindCodeInfo(CodeInfo* pSource, CodeInfo* pCodeArray, int nArSize)
{
	if(pSource != NULL && pCodeArray != NULL && nArSize > 0)
	{
		CodeInfo* pCodeCompare = pCodeArray;
		for (int i = 0; i < nArSize; i ++)
		{
			CodeInfo* pCurCompare = pCodeCompare + i;
			if (memcmp(pSource, pCurCompare, sizeof(CodeInfo)) == 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CSubscriptionManager::RemoveSink(long lSinkID)
{
	EnterCriticalSection(&m_critical);
	if (m_listSubscription.size() > 0)
	{
		for (std::list<CDataSourceSubscription*>::iterator it = m_listSubscription.begin(); it != m_listSubscription.end(); it ++)
		{
			CDataSourceSubscription* pSubscriptionTemp = (CDataSourceSubscription*)*it;
			if (pSubscriptionTemp != NULL && pSubscriptionTemp->m_listDataSinkID.size() > 0)
			{
				for (std::list<long>::iterator itSink = pSubscriptionTemp->m_listDataSinkID.begin(); itSink != pSubscriptionTemp->m_listDataSinkID.end(); itSink ++)
				{
					long lSinkCur = (long)*itSink;
					if (lSinkID == lSinkCur)
					{
						pSubscriptionTemp->m_listDataSinkID.erase(itSink);
						break;
					}
				}
			}
		}
	}
	LeaveCriticalSection(&m_critical);
	return FALSE;
}

char* CSubscriptionManager::GetRequestPack(CDataSourceReqestResponceData* pData, int& nAskCount)
{
	char* pszRetPack = NULL;
	nAskCount = 0;
	EnterCriticalSection(&m_critical);

	if (pData != NULL)
	{
		long lDataSinkID = pData->m_lDataSinkID;
		AskData* pAsk = (AskData*)pData->m_pRequestResponceData;
		UINT uAskType = pAsk->m_nType;
		int nAskSize = pAsk->m_nSize;
		CodeInfo* pInfoStart = pAsk->m_pCode;

		UINT uOption = pData->m_uOption;
		BOOL bIsAutoPush = (0x0A00 == (uAskType & 0x0F00));
		
		CMapPreSubscrition mapPreSub; //可能需要增加（删除）的请求
		for (int i = 0; i < nAskSize; i ++)
		{
			CodeInfo* pInfoCur = pInfoStart + i;
			mapPreSub.SetAt(pInfoCur, FALSE);
		}
		mapPreSub.SetAt( &pAsk->m_nPrivateKey.m_pCode,FALSE,TRUE);
		
		BOOL bFindSub = FALSE;
		BOOL bFindCode = FALSE;
		TRACE("开始处理订阅列表，当前到服务器发送的请求=%x，代码数量=%d，来自%x\n", uAskType, nAskCount, lDataSinkID);
		if (m_listSubscription.size() > 0)
		{
			std::list<CDataSourceSubscription*>::iterator itSub = m_listSubscription.begin();
			for (;;)	//全循环
			{
				BOOL bEraseit = FALSE;
				CDataSourceSubscription* pSubscriptionCur = (CDataSourceSubscription*)*itSub;
				if (pSubscriptionCur != NULL )
				{
					//先清除非主推的超时订阅数据
					if(0x0A00 != (pSubscriptionCur->m_uAskType & 0x0F00))	//非主推
					{
						DWORD dwCurTick = GetTickCount();

						if (pSubscriptionCur->m_dwTickResponce != 0 && dwCurTick - pSubscriptionCur->m_dwTickRequest >= HS_SUBSCRIPTION_IGNORE_INTERVAL)		//非主推请求超时
						{
							std::list<CDataSourceSubscription*>::iterator itTemp = itSub;
							if (itTemp != m_listSubscription.end())
							{
								itTemp ++;
							}
							if (pSubscriptionCur->m_listDataSinkID.size() > 0)
							{
								pSubscriptionCur->m_listDataSinkID.empty();
							}
							m_listSubscription.erase(itSub);
							if (pSubscriptionCur != NULL)
							{
								delete pSubscriptionCur;
								pSubscriptionCur = NULL;
							}
							itSub = itTemp;
							bEraseit = TRUE;
							if (itTemp == m_listSubscription.end())
							{
								break;
							}
							continue;
						}
					}

					if (pSubscriptionCur->m_uAskType == uAskType)
					{
						bFindSub = TRUE;
						if(uOption & e_DataSouceSend_Clear)		//带删除标志
						{
							if (!FindCodeInfo(&pSubscriptionCur->m_codeInfo, pInfoStart, nAskSize))		//当前订阅代码不在当前请求包内
							{
								//清除掉原来的订阅
								for (std::list<long>::iterator itSink = pSubscriptionCur->m_listDataSinkID.begin(); itSink != pSubscriptionCur->m_listDataSinkID.end(); itSink ++)
								{
									long lSinkCur = (long)*itSink;
									if (lSinkCur == lDataSinkID)
									{
										pSubscriptionCur->m_listDataSinkID.erase(itSink);
										break;
									}
								}
								if(pSubscriptionCur->m_listDataSinkID.size() <= 0)		//该代码、功能不存在的订阅已经取消
								{
									std::list<CDataSourceSubscription*>::iterator itTemp = itSub;
									if (itTemp != m_listSubscription.end())
									{
										itTemp ++;
									}
									m_listSubscription.erase(itSub);
									if (pSubscriptionCur != NULL)
									{
										delete pSubscriptionCur;
										pSubscriptionCur = NULL;
									}
									itSub = itTemp;
									bEraseit = TRUE;
									if (itTemp == m_listSubscription.end())
									{
										break;
									}
								}
								else
								{
									if(bIsAutoPush)
									{
										mapPreSub.SetAt(&pSubscriptionCur->m_codeInfo, TRUE);
									}
								}
							}
							else	//当前订阅列表中-已经有了该代码、功能;  所以校验回调值
							{
								BOOL bFind = FALSE;
								for (std::list<long>::iterator itSink = pSubscriptionCur->m_listDataSinkID.begin(); itSink != pSubscriptionCur->m_listDataSinkID.end(); itSink ++)
								{
									long lSinkCur = (long)*itSink;
									if (lSinkCur == lDataSinkID)
									{
										bFind = TRUE;
										break;
									}
								}
								if (!bFind)
								{
									pSubscriptionCur->m_listDataSinkID.push_back(lDataSinkID);
								}
								mapPreSub.SetAt(&pSubscriptionCur->m_codeInfo, TRUE, TRUE);
							}
						}
						else		//非清除原订阅
						{
							if(FindCodeInfo(&pSubscriptionCur->m_codeInfo, pInfoStart, nAskSize))		//匹配当前代码
							{
								bFindCode = TRUE;
								BOOL bFind = FALSE;
								for (std::list<long>::iterator itSink = pSubscriptionCur->m_listDataSinkID.begin(); itSink != pSubscriptionCur->m_listDataSinkID.end(); itSink ++)
								{
									long lSinkCur = (long)*itSink;
									if (lSinkCur == lDataSinkID)
									{
										bFind = TRUE;
										break;
									}
								}
								if (!bFind)
								{
									pSubscriptionCur->m_listDataSinkID.push_back(lDataSinkID);
								}
								mapPreSub.SetAt(&pSubscriptionCur->m_codeInfo, TRUE, TRUE);
							}
							else  if( bIsAutoPush )//叠加代码时，需要把原主推加进来
								mapPreSub.SetAt(&pSubscriptionCur->m_codeInfo, FALSE, TRUE);
							//目前状况下，好像服务器并没有可以附加传相同主推
							if(bIsAutoPush)
							{
//								listRequest.push_back(&pSubscriptionCur->m_codeInfo);
							}
						}
					}
				}
				if (!bEraseit)
				{
					if (itSub != m_listSubscription.end())
					{
						itSub ++;
					}
				}
				if (m_listSubscription.size() <= 0 || itSub == m_listSubscription.end())
				{
					break;
				}
			}
		}

		nAskCount = mapPreSub.GetSize();
		TRACE("当前到服务器发送的请求=%x，代码数量=%d，来自%x\n", uAskType, nAskCount, lDataSinkID);
		
		int nNewSub = 0;
		if (nAskCount > 0 && lDataSinkID != TEMP_DATASOURCE_ID)		//更新订阅列表
		{
			std::list<CodeInfo*> listUnSub;
			nNewSub = mapPreSub.GetUnSetSubList(listUnSub);
			TRACE("当前新的订阅请求=%x，订阅数量=%d\n", uAskType, nNewSub);
			if (nNewSub > 0)
			{
				for (std::list<CodeInfo*>::iterator it = listUnSub.begin(); it != listUnSub.end(); it ++)
				{
					CodeInfo* pInfoCur = (CodeInfo*)*it;
					if (pInfoCur != NULL)
					{
						CDataSourceSubscription* pSubscriptionNew = new CDataSourceSubscription;
						memcpy(&pSubscriptionNew->m_codeInfo, pInfoCur, sizeof(CodeInfo));
						pSubscriptionNew->m_uAskType = uAskType;
						pSubscriptionNew->m_dwTickRequest = GetTickCount();
						pSubscriptionNew->m_dwTickResponce = 0;
						pSubscriptionNew->m_listDataSinkID.push_back(lDataSinkID);
						pSubscriptionNew->m_dwPacketID = pAsk->m_lKey;
						if (pAsk->m_nType == RT_TECHDATA_EX )
						{
							ReqDayData* pDay = (ReqDayData*)pAsk->m_pCode;
							pSubscriptionNew->m_uResever = pDay->m_cPeriod;
						}
						m_listSubscription.push_back(pSubscriptionNew);
					}
				}
			}
		}

		if ( nAskCount > 0 )		//生成本次到服务器的请求包;(没有新的订阅，就不需要到服务器更新主推请求数据包2011.1.6;待添加该逻辑)
		{
			int nRewAllocCharSize = sizeof(AskData) + sizeof(CodeInfo) * (nAskCount - 1);
			pszRetPack = new char[nRewAllocCharSize];
			memset(pszRetPack, 0, nRewAllocCharSize);
			AskData* pNewAsk = (AskData*)pszRetPack;
			memcpy(pNewAsk, pAsk, sizeof(AskData) - sizeof(CodeInfo));
			pNewAsk->m_cNotEmptyPacket = (!(uOption & e_DataSouceSend_Clear) && bIsAutoPush);
			pNewAsk->m_nSize = nAskCount;
			CodeInfo* pInfoNew = pNewAsk->m_pCode;

			mapPreSub.CopyCodeInfo(pInfoNew, nAskCount);
		}
		mapPreSub.RemoveAll();
	}
	
	LeaveCriticalSection(&m_critical);
	return pszRetPack;
}

BOOL CSubscriptionManager::UpdateOneSubscription(UINT uAskType, long lDataSinkID, AskData* pAsk /*= NULL*/)
{
	BOOL bRet = FALSE;
	EnterCriticalSection(&m_critical);

	CDataSourceSubscription* pSubscription = NULL;
	CodeInfo* pInfo = NULL;
	
	if (pAsk != NULL && uAskType == RT_TECHDATA_EX )
	{
		pInfo = &((ReqDayData*)pAsk->m_pCode)->m_ciCode;
	}
	if ( pAsk && pAsk->m_nType == RT_HISTREND ) 
		pInfo = pAsk->m_pCode;
	if (m_listSubscription.size() > 0)
	{
		if ( pAsk->m_nType == RT_TECHDATA_EX )
		{
			for (std::list<CDataSourceSubscription*>::iterator it = m_listSubscription.begin(); it != m_listSubscription.end(); it ++)
			{
				CDataSourceSubscription* pSubscriptionTemp = (CDataSourceSubscription*)*it;
				if (pSubscriptionTemp != NULL && pSubscriptionTemp->m_uAskType == uAskType && 
					(pInfo == NULL || memcmp(pInfo, &pSubscriptionTemp->m_codeInfo, sizeof(CodeInfo)) == 0)  )					
				{
					pSubscription = pSubscriptionTemp;
					break;
				}
			}
		}
	
	}
	if (pSubscription != NULL)
	{
		BOOL bFind = FALSE;
		for (std::list<long>::iterator itSink = pSubscription->m_listDataSinkID.begin(); itSink != pSubscription->m_listDataSinkID.end(); itSink ++)
		{
			long lSinkCur = (long)*itSink;
			if (lDataSinkID == lSinkCur)
			{
				bFind = TRUE;
				break;
			}
		}
		if (!bFind )
		{
			pSubscription->m_listDataSinkID.push_back(lDataSinkID);
		}
		else
		{
			if (GetTickCount() - pSubscription->m_dwTickRequest >= HS_SUBSCRIPTION_IGNORE_INTERVAL && pSubscription->m_dwTickResponce > 0)	//请求超时
			{
				pSubscription->m_dwTickRequest = GetTickCount();
				bRet = TRUE;
			}			
		}
		//盘后分析 增加周期类型比较,多日分时，连续请求1分钟k线时，需要发送请求包
		if ( pAsk->m_nType == RT_TECHDATA_EX && (pSubscription->m_dwPacketID != pAsk->m_lKey  || pAsk->m_lKey == PERIOD_TYPE_MINUTE1) ) 
		{
			pSubscription->m_dwTickRequest = GetTickCount();
			bRet = TRUE;
		}
	}
	else
	{
		CDataSourceSubscription* pSubscriptionNew = new CDataSourceSubscription;
		if (pInfo != NULL)
		{
			memcpy(&pSubscriptionNew->m_codeInfo, pInfo, sizeof(CodeInfo));
		}
		else
		{
			memset(&pSubscriptionNew->m_codeInfo, 0, sizeof(CodeInfo));
		}
		pSubscriptionNew->m_uAskType = uAskType;
		pSubscriptionNew->m_dwPacketID = pAsk->m_lKey;
		if (pAsk->m_nType == RT_TECHDATA_EX )
		{
			ReqDayData* pDay = (ReqDayData*)pAsk->m_pCode;
			pSubscriptionNew->m_uResever = pDay->m_cPeriod;
		}
	
		pSubscriptionNew->m_dwTickRequest = GetTickCount();
		pSubscriptionNew->m_dwTickResponce = 0;
		pSubscriptionNew->m_listDataSinkID.push_back(lDataSinkID);
		m_listSubscription.push_back(pSubscriptionNew);
		bRet = TRUE;
	}
	LeaveCriticalSection(&m_critical);
	return bRet;
}

BOOL CSubscriptionManager::RemoveSubscriptions( UINT uAskType, CodeInfo* pCodeInfo, UINT uPacketID /*= 0*/ )
{
	if (m_listSubscription.size() > 0)
	{
		std::list<CDataSourceSubscription*>::iterator itSub = m_listSubscription.begin();
		for (;;)	//全循环
		{
			BOOL bEraseit = FALSE;
			CDataSourceSubscription* pSubscriptionCur = (CDataSourceSubscription*)*itSub;
			if (pSubscriptionCur != NULL )
			{
				//先清除非主推的超时订阅数据
				if(0x0A00 != (pSubscriptionCur->m_uAskType & 0x0F00) && pSubscriptionCur->m_uAskType == uAskType)	//非主推
				{
					DWORD dwCurTick = GetTickCount();

					if (pSubscriptionCur->m_dwTickResponce != 0 && dwCurTick - pSubscriptionCur->m_dwTickRequest >= HS_SUBSCRIPTION_IGNORE_INTERVAL)		//非主推请求超时
					{
						std::list<CDataSourceSubscription*>::iterator itTemp = itSub;
						if (itTemp != m_listSubscription.end())
						{
							itTemp ++;
						}
						if (pSubscriptionCur->m_listDataSinkID.size() > 0)
						{
							pSubscriptionCur->m_listDataSinkID.empty();
						}
						m_listSubscription.erase(itSub);
						if (pSubscriptionCur != NULL)
						{
							delete pSubscriptionCur;
							pSubscriptionCur = NULL;
						}
						itSub = itTemp;
						bEraseit = TRUE;
						if (itTemp == m_listSubscription.end())
						{
							break;
						}
						continue;
					}
				}
			}
		}
	}
	return TRUE;
}

