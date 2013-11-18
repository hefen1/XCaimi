//////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "ReplyDataManger.h"
#include "RequestResponceDataManager.h"
#include "WndRecvData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//////////////////////////////////////////////////////////////////////////
//
CReplyDataManger::CReplyDataManger()
{
	m_pszHead = NULL;
	m_nHeadLen = 0;
}

CReplyDataManger::~CReplyDataManger()
{

}

BOOL CReplyDataManger::AddElementHead(char* pszHead, int nHeadLen)
{
	m_pszHead = pszHead;
	m_nHeadLen = nHeadLen;
	return TRUE;
}

BOOL CReplyDataManger::AddOneElement(long lSinkID, char* pData, int nDataLen,BOOL bRemove)
{
	BOOL bFind = FALSE;
	CSinksReply* pSinkReply = NULL;
	if(m_listSinkReply.size() > 0)
	{
		for (std::list<CSinksReply*>::iterator it = m_listSinkReply.begin(); it != m_listSinkReply.end(); it ++)
		{
			CSinksReply* pCur = (CSinksReply*)*it;
			if (pCur != NULL && pCur->m_lSinkID == lSinkID)
			{
				pSinkReply = pCur;
				pSinkReply->m_lTotalLen += nDataLen;
				bFind = TRUE;
				break;
			}
		}
	}
	if (pSinkReply == NULL)
	{
		pSinkReply = new CSinksReply();
		pSinkReply->m_lTotalLen = nDataLen;
		pSinkReply->m_lSinkID = lSinkID;
	}
	CReplyDataElement* pElement = new CReplyDataElement;
	pElement->m_pszData = pData;
	pElement->m_nSize = nDataLen;
	pElement->m_bRemoveData = bRemove;
	pSinkReply->m_listData.push_back(pElement);

	if (!bFind)
	{
		m_listSinkReply.push_back(pSinkReply);
	}
	return TRUE;
}

void CReplyDataManger::RemoveAllElement()
{
	while(m_listSinkReply.size() > 0)
	{
		CSinksReply* pReply = m_listSinkReply.front();
		m_listSinkReply.pop_front();
		if (pReply != NULL)
		{
			while(pReply->m_listData.size() > 0)
			{
				CReplyDataElement* pElement = pReply->m_listData.front();
				pReply->m_listData.pop_front();
				if (pElement != NULL)
				{
					delete pElement;
					pElement = NULL;
				}
			}
			delete pReply;
			pReply = NULL;
		}
	}
}

int	CReplyDataManger::GetSize()
{
	return m_listSinkReply.size();
}

int	CReplyDataManger::ReplyToSink(HWND hWndAsync)
{
	if (m_listSinkReply.size() > 0)
	{
		for (std::list<CSinksReply*>::iterator it = m_listSinkReply.begin(); it != m_listSinkReply.end(); it ++)
		{
			CSinksReply* pReply = (CSinksReply*)*it;
			if (pReply != NULL && pReply->m_lTotalLen > 0)
			{
				int nTotalLen = pReply->m_lTotalLen + m_nHeadLen;
				char* pszNewChar = new char[nTotalLen];
				memset(pszNewChar, 0, nTotalLen);
				memcpy(pszNewChar, m_pszHead, m_nHeadLen);

				char* pCopy = pszNewChar + m_nHeadLen ;
				if (pReply->m_listData.size() > 0)
				{
					for (std::list<CReplyDataElement*>::iterator itEle = pReply->m_listData.begin(); itEle != pReply->m_listData.end(); itEle ++)
					{
						CReplyDataElement* pEleCur = (CReplyDataElement*)*itEle;
						if (pEleCur != NULL && pEleCur->m_nSize > 0)
						{
							memcpy(pCopy, pEleCur->m_pszData, pEleCur->m_nSize);
							pCopy += pEleCur->m_nSize;
						}
					}
				}

				PacketInfo info;
				info.m_lDataTotal = nTotalLen;
				info.m_lDataTransmited = nTotalLen;
				info.m_pszData = pszNewChar;

				CDataSourceReqestResponceData dataSend;
				dataSend.m_nAskDataLen = nTotalLen;
				dataSend.m_pRequestResponceData = (char*)&info;
				dataSend.m_lDataSinkID = pReply->m_lSinkID;
				dataSend.m_uStatus = eDataSourceRequestResponceStauts_recvPorcess;
				if (hWndAsync != NULL && ::IsWindow(hWndAsync))
				{
					::SendMessage(hWndAsync, WM_THREAD_ASYNC, (WPARAM)0, (LPARAM)&dataSend);
				}

				delete[] pszNewChar;
				pszNewChar = NULL;
			}
		}
	}
	return 0;
}