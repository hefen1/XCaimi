#include "StdAfx.h"
#include <process.h>
#include "ConnectorImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CConnector::IsRThreadStart() const
{
	return ( NULL != m_hReceiveThread );
}
bool CConnector::IsSThreadStart() const
{
	return ( NULL != m_hSendThread );
}
bool CConnector::IsCBThreadStart() const
{
	return ( NULL != m_hCallBackThread );
}

bool CConnector::CreateReceiveThread()
{
	if (!IsRThreadStart())
	{
		if (IsOpen())
		{
			HANDLE hThread;
			UINT uiThreadId = 0;
			hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
				0,	// stack
				ReceiveThreadProc,	// Thread proc
				this,	// Thread param
				CREATE_SUSPENDED,	// creation mode
				&uiThreadId);	// Thread ID

			if ( NULL != hThread)
			{
				//SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
				ResumeThread( hThread );
				m_hReceiveThread = hThread;
#ifdef _DEBUG
				TRACE1("receive thread ID:%0x\n",uiThreadId);
#endif 
				return true;
			}
			OnEvent(EVT_CREATETHREADERR);
			return false;
		}
		OnEvent(EVT_NOTCONNECTED);
		return false;
	}
	OnEvent(EVT_THREADEXIST);
	return false;
}
void CConnector::StopReceiveThread()
{
	if (IsRThreadStart())
	{
#ifdef _DEBUG
		SYSTEMTIME time;
		GetSystemTime(&time);
		CString str;
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("停止接收线程！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
		if(WaitForSingleObject(m_hReceiveThread,100L) == WAIT_TIMEOUT)
			TerminateThread(m_hReceiveThread, 1L);
		CloseHandle(m_hReceiveThread);
		m_hReceiveThread = NULL;
#ifdef _DEBUG
		GetSystemTime(&time);
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("停止接收线程完成！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
	}
}
bool CConnector::CreateSendThread()
{
	if (!IsSThreadStart())
	{
		if (IsOpen())
		{
			HANDLE hThread;
			UINT uiThreadId = 0;
			hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
				0,	// stack
				SendThreadProc,	// Thread proc
				this,	// Thread param
				CREATE_SUSPENDED,	// creation mode
				&uiThreadId);	// Thread ID

			if ( NULL != hThread)
			{
				//SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
				ResumeThread( hThread );
				m_hSendThread = hThread;
#ifdef _DEBUG
				TRACE1("send thread ID:%0x\n",uiThreadId);
#endif  
				return true;
			}
			OnEvent(EVT_CREATETHREADERR);
			return false;
		}
		OnEvent(EVT_NOTCONNECTED);
		return false;
	}
	OnEvent(EVT_THREADEXIST);
	return false;
}
void CConnector::StopSendThread()
{
	if (IsSThreadStart())
	{
#ifdef _DEBUG
		SYSTEMTIME time;
		GetSystemTime(&time);
		CString str;
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("停止发送线程！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
		if(WaitForSingleObject(m_hSendThread,100L) == WAIT_TIMEOUT)
			TerminateThread(m_hSendThread, 1L);
		CloseHandle(m_hSendThread);
		m_hSendThread = NULL;
#ifdef _DEBUG
		GetSystemTime(&time);
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("停止发送线程完成！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
	}
}
bool CConnector::CreateCallBackThread()
{
	if (!IsCBThreadStart())
	{
		if (IsOpen())
		{
			HANDLE hThread;
			UINT uiThreadId = 0;
			hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
				0,	// stack
				CallBackThreadProc,	// Thread proc
				this,	// Thread param
				CREATE_SUSPENDED,	// creation mode
				&uiThreadId);	// Thread ID
			if ( NULL != hThread)
			{
				//SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
				ResumeThread( hThread );
				m_hCallBackThread = hThread;
#ifdef _DEBUG
				TRACE1("callback thread ID:%0x\n",uiThreadId);
#endif 
				return true;
			}
			OnEvent(EVT_CREATETHREADERR);
			return false;
		}
		OnEvent(EVT_NOTCONNECTED);
		return false;
	}
	OnEvent(EVT_THREADEXIST);
	return false;
}
void CConnector::StopCallBackThread()
{
	if (IsCBThreadStart())
	{
#ifdef _DEBUG
		SYSTEMTIME time;
		GetSystemTime(&time);
		CString str;
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("停止回调线程！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
		WaitCallBackThreadDone();
		if(WaitForSingleObject(m_hCallBackThread,100L) == WAIT_TIMEOUT)
			TerminateThread(m_hCallBackThread, 1L);
		CloseHandle(m_hCallBackThread);
		m_hCallBackThread = NULL;
#ifdef _DEBUG
		GetSystemTime(&time);
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("停止回调线程完成！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
	}
}
bool CConnector::StartThreads()
{
	return (ResetEvent(m_hEventQuit) && CreateSendThread() && CreateReceiveThread() && CreateCallBackThread());
}
void CConnector::StopThreads()
{
	SetEvent(m_hEventQuit);
	StopSendThread();
	StopReceiveThread();
	StopCallBackThread();
}
int CConnector::WaitCallBackThreadDone()
{
	CSingleLock lock(&m_cCritical_Async);
	lock.Lock();
	while (!m_listReceiveDataAsync.IsEmpty())
	{
		lock.Unlock();
		if(WaitForSingleObject(m_hCallBackThread,100L) == WAIT_OBJECT_0)
			return 0;
		lock.Lock();
	}
	lock.Unlock();
	return 0;
}
UINT WINAPI CConnector::ReceiveThreadProc(LPVOID pParam) 
{
	CConnector* pThis = reinterpret_cast<CConnector*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->RunReceive();

	return 1L;
}
UINT WINAPI CConnector::SendThreadProc(LPVOID pParam)
{
	CConnector* pThis = reinterpret_cast<CConnector*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->RunSend();

	return 1L;
}
UINT WINAPI CConnector::CallBackThreadProc(LPVOID pParam)
{
	CConnector* pThis = reinterpret_cast<CConnector*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->RunCallBack();

	return 1L;
}