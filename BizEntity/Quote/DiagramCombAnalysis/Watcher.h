#pragma once
#include ".\CombComm\CombManagerInterface.h"
#include ".\CombComm\IndicatrixObj.h"
#include ".\CombComm\CombCache.h"
#include <afxmt.h>

class CWatcher
{
public:
	CWatcher(void);
	~CWatcher(void);
	void SetWatcherInfo(IWatcherInfo* pInfo);
	CString GetName();
	IWatcherParams* GetParams();
	void AddIndicatrixObj(CString strIndicatrixName,IIndicatrixObj* pObj);
	BOOL DoCalculateWork();
	double GetLastIndicatrixObjValue(CString strIndicatrixName);
	IIndicatrixObj* GeIndicatrixOb(CString strIndicatrixName);
	void UpdateIndicatrixObjs();
	void SaveLastIndicatriData();
	void LoadLastIndicatriData();
	IWatcherInfo* GetWatcherInfo() { return m_pWatcherInfo; }
private:
	IWatcherInfo*         m_pWatcherInfo;
	CMapStringToOb    m_IndicatrixMap;              //ָ�������:ָ������==>CIndicatrixObj�������==>���������;�̿ڻ���==>��������
};

typedef CTypedPtrArray<CPtrArray,CWatcher*> WatcherArray;
class CWatcherSet: public ICombCacheNotifier
{
public:
	CWatcherSet();
	~CWatcherSet();
	//ʵ��
	virtual void OnCombCacheDataUpdate();

	void InitWatcherSet(CString strWatcherSetName);
	void Clear();
	int GetCount();
	CWatcher* GetWatcher(int nIndex);
	void AddWatcher(CWatcher* pWatcher);
	void RemoveWatcher(CWatcher* pWatcher);
	void ModifyWatcher(CWatcher* pWatcher);

	BOOL DoCalculateWork();
private:
	CCriticalSection               m_critSection;
	WatcherArray                  m_ayWatcher;
	IWatcherInfoManager*    m_pWatcherInfoMg;
};
