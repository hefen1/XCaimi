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
				//���pCodeInfo == NULL, ��Ҫ���ҵ�һ�������ģ��ʹ�����Ķ���
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
		
		CMapPreSubscrition mapPreSub; //������Ҫ���ӣ�ɾ����������
		for (int i = 0; i < nAskSize; i ++)
		{
			CodeInfo* pInfoCur = pInfoStart + i;
			mapPreSub.SetAt(pInfoCur, FALSE);
		}
		mapPreSub.SetAt( &pAsk->m_nPrivateKey.m_pCode,FALSE,TRUE);
		
		BOOL bFindSub = FALSE;
		BOOL bFindCode = FALSE;
		TRACE("��ʼ�������б���ǰ�����������͵�����=%x����������=%d������%x\n", uAskType, nAskCount, lDataSinkID);
		if (m_listSubscription.size() > 0)
		{
			std::list<CDataSourceSubscription*>::iterator itSub = m_listSubscription.begin();
			for (;;)	//ȫѭ��
			{
				BOOL bEraseit = FALSE;
				CDataSourceSubscription* pSubscriptionCur = (CDataSourceSubscription*)*itSub;
				if (pSubscriptionCur != NULL )
				{
					//����������Ƶĳ�ʱ��������
					if(0x0A00 != (pSubscriptionCur->m_uAskType & 0x0F00))	//������
					{
						DWORD dwCurTick = GetTickCount();

						if (pSubscriptionCur->m_dwTickResponce != 0 && dwCurTick - pSubscriptionCur->m_dwTickRequest >= HS_SUBSCRIPTION_IGNORE_INTERVAL)		//����������ʱ
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
						if(uOption & e_DataSouceSend_Clear)		//��ɾ����־
						{
							if (!FindCodeInfo(&pSubscriptionCur->m_codeInfo, pInfoStart, nAskSize))		//��ǰ���Ĵ��벻�ڵ�ǰ�������
							{
								//�����ԭ���Ķ���
								for (std::list<long>::iterator itSink = pSubscriptionCur->m_listDataSinkID.begin(); itSink != pSubscriptionCur->m_listDataSinkID.end(); itSink ++)
								{
									long lSinkCur = (long)*itSink;
									if (lSinkCur == lDataSinkID)
									{
										pSubscriptionCur->m_listDataSinkID.erase(itSink);
										break;
									}
								}
								if(pSubscriptionCur->m_listDataSinkID.size() <= 0)		//�ô��롢���ܲ����ڵĶ����Ѿ�ȡ��
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
							else	//��ǰ�����б���-�Ѿ����˸ô��롢����;  ����У��ص�ֵ
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
						else		//�����ԭ����
						{
							if(FindCodeInfo(&pSubscriptionCur->m_codeInfo, pInfoStart, nAskSize))		//ƥ�䵱ǰ����
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
							else  if( bIsAutoPush )//���Ӵ���ʱ����Ҫ��ԭ���Ƽӽ���
								mapPreSub.SetAt(&pSubscriptionCur->m_codeInfo, FALSE, TRUE);
							//Ŀǰ״���£������������û�п��Ը��Ӵ���ͬ����
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
		TRACE("��ǰ�����������͵�����=%x����������=%d������%x\n", uAskType, nAskCount, lDataSinkID);
		
		int nNewSub = 0;
		if (nAskCount > 0 && lDataSinkID != TEMP_DATASOURCE_ID)		//���¶����б�
		{
			std::list<CodeInfo*> listUnSub;
			nNewSub = mapPreSub.GetUnSetSubList(listUnSub);
			TRACE("��ǰ�µĶ�������=%x����������=%d\n", uAskType, nNewSub);
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

		if ( nAskCount > 0 )		//���ɱ��ε��������������;(û���µĶ��ģ��Ͳ���Ҫ�����������������������ݰ�2011.1.6;����Ӹ��߼�)
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
			if (GetTickCount() - pSubscription->m_dwTickRequest >= HS_SUBSCRIPTION_IGNORE_INTERVAL && pSubscription->m_dwTickResponce > 0)	//����ʱ
			{
				pSubscription->m_dwTickRequest = GetTickCount();
				bRet = TRUE;
			}			
		}
		//�̺���� �����������ͱȽ�,���շ�ʱ����������1����k��ʱ����Ҫ���������
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
		for (;;)	//ȫѭ��
		{
			BOOL bEraseit = FALSE;
			CDataSourceSubscription* pSubscriptionCur = (CDataSourceSubscription*)*itSub;
			if (pSubscriptionCur != NULL )
			{
				//����������Ƶĳ�ʱ��������
				if(0x0A00 != (pSubscriptionCur->m_uAskType & 0x0F00) && pSubscriptionCur->m_uAskType == uAskType)	//������
				{
					DWORD dwCurTick = GetTickCount();

					if (pSubscriptionCur->m_dwTickResponce != 0 && dwCurTick - pSubscriptionCur->m_dwTickRequest >= HS_SUBSCRIPTION_IGNORE_INTERVAL)		//����������ʱ
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

