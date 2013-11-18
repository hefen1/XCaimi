//////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "DataSinkConnectManager.h"
#include "AutoRequest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
CDataSinkConnectManager::CDataSinkConnectManager()
{

}

CDataSinkConnectManager::~CDataSinkConnectManager()
{

}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDataSinkConnectManager::AddUpdateNew(long lDataSinkID, long lConnectHandle, long lPackID, UINT uSinkType)
{
	if (lConnectHandle == 0 )
	{
		return FALSE;
	}
	BOOL bNewConnect = TRUE;
	BOOL bFind = FALSE;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		for (std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
		{
			CDataSourceSinkToConnectID* pElementCur = (CDataSourceSinkToConnectID*)*it;
			if (pElementCur != NULL )
			{				
				if(pElementCur->m_lConnectHandle == lConnectHandle)		//先判断有没有新的连接
				{
					bNewConnect = FALSE;
				}
				if(pElementCur->m_lDataSinkID == lDataSinkID && pElementCur->m_lPackID == lPackID )
				{
					bFind = TRUE;
				}
				if(!bNewConnect && bFind)
				{
					break;
				}
			}
		}
		if (!bFind)
		{
			CDataSourceSinkToConnectID* pElement = new CDataSourceSinkToConnectID;
			pElement->m_lConnectHandle = lConnectHandle;
			pElement->m_lDataSinkID = lDataSinkID;
			pElement->m_lPackID = lPackID;
			pElement->m_uSinkType = uSinkType;
			m_listData.push_back(pElement);
		}
		lock.Unlock();
	}
	return bNewConnect;
}

long CDataSinkConnectManager::RemoveSink(long lDataSinkID)
{
	long lRetVal = 0;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		if(m_listData.size() > 0)
		{
			std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin();
			for (; ; )
			{
				CDataSourceSinkToConnectID* pElememtCur = (CDataSourceSinkToConnectID*)*it;
				std::list<CDataSourceSinkToConnectID*>::iterator itTemp = it;
				if (pElememtCur != NULL && pElememtCur->m_lDataSinkID == lDataSinkID)
				{
					if (itTemp != m_listData.end())
					{
						itTemp ++;
					}
					m_listData.erase(it);
					if (pElememtCur != NULL)
					{
						lRetVal = pElememtCur->m_lConnectHandle;
						delete pElememtCur;
						pElememtCur = NULL;
					}
					it = itTemp;
					if (it == m_listData.end())
					{
						break;
					}
				}
				else
				{
					it ++;
				}
				if (m_listData.size() <= 0 || it == m_listData.end())
				{
					break;
				}
			}
		}
		lock.Unlock();
	}
	return lRetVal;
}

int CDataSinkConnectManager::GetSize()
{
	int nRetVal = 0;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		nRetVal = m_listData.size();
		lock.Unlock();
	}
	return nRetVal;
}

BOOL CDataSinkConnectManager::RemoveAllElement()
{
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		while(m_listData.size())
		{
			CDataSourceSinkToConnectID* pElement = m_listData.front();
			m_listData.pop_front();
			if (pElement != NULL)
			{
				delete pElement ;
				pElement = NULL;
			}
		}
		lock.Unlock();
		return TRUE;
	}
	return FALSE;
}

CDataSourceSinkToConnectID* CDataSinkConnectManager::GetDataSinkID(long lConnectHandle, long lPackID)
{
	CDataSourceSinkToConnectID* pData = NULL;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		CDataSourceSinkToConnectID* pElememt = NULL;
		for (std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
		{
			CDataSourceSinkToConnectID* pElememtCur = (CDataSourceSinkToConnectID*)*it;
			if (pElememtCur != NULL && pElememtCur->m_lConnectHandle == lConnectHandle)
			{
				if (lPackID == 0)
				{
					pData = pElememtCur;
					break;
				}
				else
				{
					if (pElememtCur->m_lPackID == lPackID)
					{
						pData = pElememtCur;
						break;
					}
				}
			}
		}
		lock.Unlock();
	}
	return pData;
}

BOOL CDataSinkConnectManager::NotifySinkData(long lSinkID, void* pszDataBuff, int nDataLen)
{
	BOOL bRet = FALSE;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		try
		{
			CDataSourceSinkToConnectID* pElememt = NULL;
			for (std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
			{
				CDataSourceSinkToConnectID* pElememtCur = (CDataSourceSinkToConnectID*)*it;
				if (pElememtCur != NULL && pElememtCur->m_lDataSinkID == lSinkID)
				{
					IDataSourceSinkEx* pSink = (IDataSourceSinkEx*)pElememtCur->m_lDataSinkID;
					if (pSink != NULL && !::IsBadReadPtr(pSink, sizeof(long)))
					{
						pSink->HSDataSourceSinkEx_RecvDataNotify();
						bRet = TRUE;
						break;
					}
				}
			}
		}
		catch(...)
		{

		}
		lock.Unlock();
	}
	return bRet;
}
BOOL CDataSinkConnectManager::NotifyCommStatus(void* pszDataBuff,long lSinkID)
{
	BOOL bRet = FALSE;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		try
		{
			CDataSourceSinkToConnectID* pElememt = NULL;
			for (std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
			{
				CDataSourceSinkToConnectID* pElememtCur = (CDataSourceSinkToConnectID*)*it;
				if (pElememtCur != NULL )
				{
					if ( lSinkID != -1 && pElememtCur->m_lDataSinkID == lSinkID)
					{
						IDataSourceSinkEx* pSink = (IDataSourceSinkEx*)pElememtCur->m_lDataSinkID;
						if (pSink != NULL && !::IsBadReadPtr(pSink, sizeof(long)))
						{
							pSink->HSDataSourceSink_OnCommNotify(pszDataBuff);
							bRet = TRUE;
							break;
						}
					}
					if ( lSinkID == -1 )
					{
						IDataSourceSinkEx* pSink = (IDataSourceSinkEx*)pElememtCur->m_lDataSinkID;
						if (pSink != NULL && !::IsBadReadPtr(pSink, sizeof(long)))
						{
							pSink->HSDataSourceSink_OnCommNotify(pszDataBuff);
							bRet = TRUE;
						}
					}
					
				}
			}
		}
		catch(...)
		{

		}
		lock.Unlock();
	}
	return bRet;
}
BOOL CDataSinkConnectManager::SendSinkData(long lSinkID, void* pszDataBuff, int nDataLen)
{
	BOOL bRet = FALSE;	
	DataHead* pHead = (DataHead*)pszDataBuff;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		try
		{
			CDataSourceSinkToConnectID* pElememt = NULL;
			for (std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
			{
				CDataSourceSinkToConnectID* pElememtCur = (CDataSourceSinkToConnectID*)*it;
				if (pElememtCur != NULL && pElememtCur->m_lDataSinkID == lSinkID)
				{
					IDataSourceSink* pSink = (IDataSourceSink*)pElememtCur->m_lDataSinkID;			
					TRACE("\r\n send data to sink  type is %x , sinkID is %d", pHead->m_nType, lSinkID);
					if (pSink != NULL && !::IsBadReadPtr(pSink, sizeof(long)))
					{
						pSink->HSDataSourceSink_OnRecvData(pszDataBuff, nDataLen);
						bRet = TRUE;
						break;
					}
				}
			}
		}
		catch(...)
		{
		
		}
		lock.Unlock();
	}
	return bRet;
}

long CDataSinkConnectManager::SendSinkData(long lConnectHandle, long lPackID, void* pszDataBuff, int nDataLen)
{
	long lSinkRet = 0;
	CDataSourceSinkToConnectID* pData = NULL;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		try
		{
			CDataSourceSinkToConnectID* pElememt = NULL;
			for (std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
			{
				CDataSourceSinkToConnectID* pElememtCur = (CDataSourceSinkToConnectID*)*it;
				if (pElememtCur != NULL && pElememtCur->m_lConnectHandle == lConnectHandle)
				{
					if (lPackID == 0)
					{
						pData = pElememtCur;
						break;
					}
					else
					{
						if (pElememtCur->m_lPackID == lPackID)
						{
							pData = pElememtCur;
							break;
						}
					}
				}
			}
			if (pData != NULL)
			{
				IDataSourceSink* pSink = (IDataSourceSink*)pData->m_lDataSinkID;
				if (pSink != NULL && !::IsBadReadPtr(pSink, sizeof(long)))
				{
					pSink->HSDataSourceSink_OnRecvData(pszDataBuff, nDataLen);
					lSinkRet = pData->m_lDataSinkID;
				}
			}
		}
		catch(...)
		{
		}
		lock.Unlock();
	}
	return lSinkRet;
}

BOOL CDataSinkConnectManager::SendAllInitFinishInfo()
{
	BOOL bRet = FALSE;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		try
		{
			CDataSourceSinkToConnectID* pElememt = NULL;
			for (std::list<CDataSourceSinkToConnectID*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
			{
				CDataSourceSinkToConnectID* pElememtCur = (CDataSourceSinkToConnectID*)*it;
				if (pElememtCur != NULL )
				{
					IDataSourceSink* pSink = (IDataSourceSink*)pElememtCur->m_lDataSinkID;
					if (pSink != NULL && pElememtCur->m_lDataSinkID != TEMP_DATASOURCE_ID && !::IsBadReadPtr(pSink, sizeof(long)))
					{
						DataHead dataHead;
						dataHead.m_nType = RT_INITIALINFO;

						pSink->HSDataSourceSink_OnRecvData(&dataHead, sizeof(DataHead));
						bRet = TRUE;
//						break;
					}
				}
			}
		}
		catch(...)
		{

		}
		lock.Unlock();
	}
	return bRet;
}