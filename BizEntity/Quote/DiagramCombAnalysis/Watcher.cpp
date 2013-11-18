#include "StdAfx.h"
#include "Watcher.h"
#include ".\CombComm\CombManagerInterface.h"
#include "WinnerApplication.h"
//////////////////////////////////////////////////////////////////////////
//CWatcher
//////////////////////////////////////////////////////////////////////////

CWatcher::CWatcher(void)
{
	m_pWatcherInfo = NULL;
}

CWatcher::~CWatcher(void)
{
	SaveLastIndicatriData();
	CString strKey;
	IIndicatrixObj *pIndicatrixObj = NULL;
	for(POSITION pos = m_IndicatrixMap.GetStartPosition(); pos  != NULL;)
	{
		m_IndicatrixMap.GetNextAssoc(pos,strKey,(CObject*&)pIndicatrixObj);
		if(pIndicatrixObj)
		{
			pIndicatrixObj->Release();
		}
	}
}

void CWatcher::SaveLastIndicatriData()
{
	CString strKey;
	IIndicatrixObj *pIndicatrixObj = NULL;
	for(POSITION pos = m_IndicatrixMap.GetStartPosition(); pos  != NULL;)
	{
		m_IndicatrixMap.GetNextAssoc(pos,strKey,(CObject*&)pIndicatrixObj);
		if(pIndicatrixObj)
		{
			CString strFile = this->GetName() + _T("_") + pIndicatrixObj->GetIndicatrixName();
			pIndicatrixObj->Save(strFile);
		}
	}
}

void CWatcher::LoadLastIndicatriData()
{
	CString strKey;
	IIndicatrixObj *pIndicatrixObj = NULL;
	for(POSITION pos = m_IndicatrixMap.GetStartPosition(); pos  != NULL;)
	{
		m_IndicatrixMap.GetNextAssoc(pos,strKey,(CObject*&)pIndicatrixObj);
		if(pIndicatrixObj)
		{
			CString strFile = this->GetName() + _T("_") + pIndicatrixObj->GetIndicatrixName();
			pIndicatrixObj->Load(strFile);
		}
	}
}

CString CWatcher::GetName()
{
	if(m_pWatcherInfo)
		return m_pWatcherInfo->GetName();
	else
		return _T("");
}

IWatcherParams* CWatcher::GetParams()
{
	if(m_pWatcherInfo)
		return m_pWatcherInfo->GetParams();
	else
		return FALSE;
}

void CWatcher::SetWatcherInfo(IWatcherInfo* pInfo)
{
	m_pWatcherInfo = pInfo;
}


void CWatcher::AddIndicatrixObj(CString strIndicatrixName,IIndicatrixObj* pObj)
{
	m_IndicatrixMap.SetAt(strIndicatrixName,(CObject*&)pObj);
}

BOOL CWatcher::DoCalculateWork()
{
	CString strKey;
	IIndicatrixObj *pIndicatrixObj = NULL;
	CTime now = CTime::GetCurrentTime();
	for(POSITION pos = m_IndicatrixMap.GetStartPosition(); pos  != NULL;)
	{
		m_IndicatrixMap.GetNextAssoc(pos,strKey,(CObject*&)pIndicatrixObj);
		if(pIndicatrixObj)
			pIndicatrixObj->DoCalculate(now);	
	}
	return TRUE;
}

double CWatcher::GetLastIndicatrixObjValue(CString strIndicatrixName)
{
	IIndicatrixObj *pIndicatrixObj = NULL;
	if( m_IndicatrixMap.Lookup(strIndicatrixName,(CObject*&)pIndicatrixObj) )
	{
		return pIndicatrixObj->GetLastValue();
	}
	else
	{
		return 0;
	}
}

IIndicatrixObj* CWatcher::GeIndicatrixOb(CString strIndicatrixName)
{
	IIndicatrixObj *pIndicatrixObj = NULL;
	if( m_IndicatrixMap.Lookup(strIndicatrixName,(CObject*&)pIndicatrixObj) )
	{
		return pIndicatrixObj;
	}
	else
	{
		return 0;
	}
}

void CWatcher::UpdateIndicatrixObjs()
{
	CString strKey;
	IIndicatrixObj *pIndicatrixObj = NULL;
	IWatcherParams* pWatcherParams= GetParams();
	for(POSITION pos = m_IndicatrixMap.GetStartPosition(); pos  != NULL;)
	{
		m_IndicatrixMap.GetNextAssoc(pos,strKey,(CObject*&)pIndicatrixObj);
		if(pIndicatrixObj)
		{
			CString strExp = pIndicatrixObj->GetExpression();
			pIndicatrixObj->BuildExpressionObj(strExp,pWatcherParams);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//CWatcherSet
//////////////////////////////////////////////////////////////////////////

CWatcherSet::CWatcherSet()
{
	m_pWatcherInfoMg = NULL;
}

CWatcherSet::~CWatcherSet()
{
	
}

void CWatcherSet::OnCombCacheDataUpdate()
{
	DoCalculateWork();
}

void CWatcherSet::InitWatcherSet(CString strWatcherSetName)
{
	CSingleLock singleLock(&m_critSection,TRUE);

	m_pWatcherInfoMg = (IWatcherInfoManager*)CWinnerApplication::GetObject(OJB_WATCHERINFOMANAGER);
	if(m_pWatcherInfoMg)
	{
		m_pWatcherInfoMg->LoadWatchers(strWatcherSetName);
		for( int i = 0; i < m_pWatcherInfoMg->GetWatcherInfoCount(); i++ )
		{
			IWatcherInfo* pWatcherInfo = m_pWatcherInfoMg->GetWatcherInfo(i);
			CWatcher* pWatcher = new CWatcher(); 
			pWatcher->SetWatcherInfo(pWatcherInfo);
			m_ayWatcher.Add(pWatcher);
		}
	}	
}

void CWatcherSet::Clear()
{
	for( int i =0; i < this->m_ayWatcher.GetCount(); i ++)
	{
		CWatcher* pWatcher = (CWatcher*)m_ayWatcher.GetAt(i);
		if(pWatcher)
			delete pWatcher;
	}
	m_ayWatcher.RemoveAll();

	if(m_pWatcherInfoMg)
	{
		m_pWatcherInfoMg->Release();
		m_pWatcherInfoMg = NULL;
	}
}

int CWatcherSet::GetCount()
{
	return m_ayWatcher.GetCount();
}

CWatcher* CWatcherSet::GetWatcher(int nIndex)
{
	CSingleLock singleLock(&m_critSection,TRUE);

	if(nIndex >=0 && nIndex <= m_ayWatcher.GetCount())
		return m_ayWatcher[nIndex];
	else
		return NULL;
}

void CWatcherSet::AddWatcher(CWatcher* pWatcher)
{
	CSingleLock singleLock(&m_critSection,TRUE);

	m_ayWatcher.Add(pWatcher);
}

void CWatcherSet::RemoveWatcher(CWatcher* pWatcher)
{
	CSingleLock singleLock(&m_critSection,TRUE);

	for( int i =0; i < this->m_ayWatcher.GetCount(); i ++)
	{
		CWatcher* pTemp = (CWatcher*)m_ayWatcher.GetAt(i);
		if(pTemp == pWatcher)
		{	
			m_ayWatcher.RemoveAt(i);
		}
	}

	if(pWatcher)
		delete pWatcher;
}

void CWatcherSet::ModifyWatcher(CWatcher* pWatcher)
{
	CSingleLock singleLock(&m_critSection,TRUE);
	pWatcher->UpdateIndicatrixObjs();
}


BOOL CWatcherSet::DoCalculateWork()
{
	CSingleLock singleLock(&m_critSection,TRUE);

	for( int i =0; i < this->m_ayWatcher.GetCount(); i ++)
	{
		CWatcher* pWatcher = (CWatcher*)m_ayWatcher.GetAt(i);
		if(pWatcher)
			pWatcher->DoCalculateWork();
	}
	return TRUE;
}