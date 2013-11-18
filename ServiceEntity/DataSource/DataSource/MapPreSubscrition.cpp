//////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "MapPreSubscrition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 
//////////////////////////////////////////////////////////////////////////
CMapPreSubscrition::CMapPreSubscrition()
{

}

CMapPreSubscrition::~CMapPreSubscrition()
{

}

//////////////////////////////////////////////////////////////////////////
//
void CMapPreSubscrition::SetAt(CodeInfo* pInfo, BOOL bCodeAlreadySet, BOOL bCheckExist)
{
	if (bCheckExist)
	{
		//检查本地是否已经存在该代码
		for (std::list<CPreSubscritionCodeInfo*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
		{
			CPreSubscritionCodeInfo* pCur = (CPreSubscritionCodeInfo*)*it;
			if (pCur != NULL && memcmp(&pCur->m_codeinfo, pInfo, sizeof(CodeInfo)) == 0)
			{
				pCur->m_bAlreadySet = bCodeAlreadySet;
				return;
			}
		}
	}


	CPreSubscritionCodeInfo* pNewElement = new CPreSubscritionCodeInfo;
	memcpy(&pNewElement->m_codeinfo, pInfo, sizeof(CodeInfo));
	pNewElement->m_bAlreadySet = bCodeAlreadySet;
	m_listData.push_back(pNewElement);
}

BOOL CMapPreSubscrition::RemoveKey(CodeInfo* pInfo)
{
	BOOL bFind = FALSE;
	CPreSubscritionCodeInfo* pRemoveKey = NULL;
	for (std::list<CPreSubscritionCodeInfo*>::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
	{
		CPreSubscritionCodeInfo* pCur = (CPreSubscritionCodeInfo*)*it;
		if (pCur != NULL && memcmp(&pCur->m_codeinfo, pInfo, sizeof(CodeInfo)) == 0)
		{
			pRemoveKey = pCur;
			m_listData.erase(it);
			bFind = TRUE;
			break;
		}
	}
	if (pRemoveKey != NULL)
	{
		delete pRemoveKey;
		pRemoveKey = NULL;
	}
	return bFind;
}

void CMapPreSubscrition::RemoveAll()
{
	while(m_listData.size() > 0)
	{
		CPreSubscritionCodeInfo* pCur = (CPreSubscritionCodeInfo*)m_listData.front();
		m_listData.pop_front();
		if (pCur != NULL)
		{
			delete pCur;
			pCur = NULL;
		}
	}
}

int CMapPreSubscrition::GetSize()
{
	return m_listData.size();
}

BOOL CMapPreSubscrition::CopyCodeInfo(CodeInfo* pInfoCopyed, int nSize)
{
	ASSERT(nSize == m_listData.size());
	if (pInfoCopyed != NULL)
	{
		CodeInfo* pInfoNew = pInfoCopyed;
		for (std::list<CPreSubscritionCodeInfo* >::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
		{
			CPreSubscritionCodeInfo* pInfoCur = (CPreSubscritionCodeInfo*)*it;
			memcpy(pInfoNew, &pInfoCur->m_codeinfo, sizeof(CodeInfo));
			pInfoNew ++;
		}
	}
	return FALSE;
}

int	CMapPreSubscrition::GetUnSetSubList(std::list<CodeInfo* >&list)
{
	if (m_listData.size() > 0)
	{
		for (std::list<CPreSubscritionCodeInfo* >::iterator it = m_listData.begin(); it != m_listData.end(); it ++)
		{
			CPreSubscritionCodeInfo* pInfoCur = (CPreSubscritionCodeInfo*)*it;
			if (pInfoCur != NULL && !pInfoCur->m_bAlreadySet)
			{
				list.push_back(&pInfoCur->m_codeinfo);
			}
		}
		return list.size();
	}
	return 0;
}