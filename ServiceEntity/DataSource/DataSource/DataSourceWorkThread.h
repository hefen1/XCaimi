#pragma once
#include <list>
#define WM_THREAD_DEAL		WM_USER + 2


// CDataSourceWorkThread
class CDataSourceImpl;
class CDataSourceWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CDataSourceWorkThread)
protected:
	CMutex				m_mutex;
	BOOL				m_bIsBusy;
public:
	CDataSourceImpl*	m_pDataSourceImpl;
	BOOL				IsThreadBusy();
protected:
public:
	CDataSourceWorkThread();           // protected constructor used by dynamic creation
	virtual ~CDataSourceWorkThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnThreadDeal(WPARAM wParam, LPARAM lParam);
};


