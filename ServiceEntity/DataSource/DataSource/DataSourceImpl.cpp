#include "stdafx.h"
#include "DataSourceImpl.h"
#include "HSDisk.h"
#include "DataCache.h"
#include <process.h>
#include "..\..\..\Common\PackEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

#define IDT_DATASUBSCRIPTION_TIMEOUT	5		//非主推实时数据超时时间
#define IDT_THREAD_EXIT					10000
HANDLE	g_hEventQuit = NULL;
HANDLE	g_hEventQueue = NULL;
long    g_nIndex=0;
extern void QuickSortAscend2(struct KQuickSort* pQSData);
//////////////////////////////////////////////////////////////////////////
//
extern CDataSourceImpl*	ds_pi_DataSourceImpl;
BOOL WINAPI	sFn_OnPacketReceive(const char* pszBuffer,UINT uLen)
{

	PacketInfo *pInfo = (PacketInfo *)pszBuffer;
	if (pInfo->m_lDataTotal != pInfo->m_lDataTransmited)
		return FALSE;

	DataHead* pAsk = (DataHead*)pInfo->m_pszData;
	if (ds_pi_DataSourceImpl == NULL)
	{
		return FALSE;
	}
	return ds_pi_DataSourceImpl->OnReceivePacket(pszBuffer, uLen);
}

BOOL WINAPI sFn_OnConnectNotify(ConnectNotify* pNotify)
{

	if (ds_pi_DataSourceImpl == NULL)
	{
		return FALSE;
	}
	return ds_pi_DataSourceImpl->OnNetConnectNotify(pNotify);
}

//////////////////////////////////////////////////////////////////////////
//
CDataSourceImpl::CDataSourceImpl()
{
	m_bQuit = FALSE;
	m_pTradeCore = NULL;
	m_pChannelManager = (IChannelManager*)CWinnerApplication::GetObject(OBJ_CHANNELMANAGER);
	m_pChannels = (IChannels*)CWinnerApplication::GetObject(OBJ_CHANNELS);

	m_pfnOnPacketReceive = sFn_OnPacketReceive;
	m_pfnOnConnectNotify = sFn_OnConnectNotify;

	//m_lConnectHandle = 0;
	m_lNotifySink = -1;
	if(m_pChannelManager != NULL)
	{
		m_pChannelManager->RegisterNotify(m_pfnOnConnectNotify);
	}
	m_nServerDateSpan = 0;
	m_nServerTimeSpan = 0;
	m_lServerTime = 0;
	m_lCurTime = 0;
	m_bInitFinished = FALSE;
	m_bReConnect = FALSE;
	m_autoRequest.SetDataSourceImpl(this);
	m_bFirstFlag = TRUE;
	m_bInitInfo = FALSE;
	m_bIniting = FALSE;
	m_pWndDataRecv = NULL;
	m_pWndProgressDlg = NULL;
	g_hEventQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hEventQueue = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CDataSourceImpl::~CDataSourceImpl()
{
	FreeAllUnit();
}

CDSWriteFile* CDataSourceImpl::GetDSWriterFile()
{
	return &m_dsWriteFile;
}

CConfigManager* CDataSourceImpl::GetConfigManager()
{
	return &m_configManager;
}

CStockManager* CDataSourceImpl::GetStockManager()
{
	return &m_stockManager;
}

CHsBouseManager* CDataSourceImpl::GetBouseManager()
{
	return &m_bourseManager;
}

BOOL CDataSourceImpl::InitEnv()
{
	if (m_bFirstFlag)
	{
		m_pTradeCore = (ITradeCoreObj *)CWinnerApplication::GetObject(OBJ_TRADECORE);

		m_dsWriteFile.InitWriteFileEnv();
		m_bourseManager.SetDataSourceImpl(this);
		m_stockManager.SetDataSourceImpl(this);
  		if(m_pWndDataRecv == NULL)
  		{
  			m_pWndDataRecv = new CWndRecvData(this);
  		}
  		if (m_pWndDataRecv != NULL )
  		{
  			if (m_pWndDataRecv->m_hWnd == NULL || !::IsWindow(m_pWndDataRecv->m_hWnd))
  			{
  				m_pWndDataRecv->Create(NULL, _T("recvdatawnd"), WS_OVERLAPPED | WS_EX_TOOLWINDOW, CRect(0, 0, 0, 0), NULL, 0);
  			}
  		}
		m_bFirstFlag = FALSE;
	}	
	return FALSE;
}

BOOL CDataSourceImpl::FreeAllUnit()
{
   	if (m_pWndDataRecv != NULL && m_pWndDataRecv->m_hWnd != NULL && ::IsWindow(m_pWndDataRecv->m_hWnd))
   	{
   		m_pWndDataRecv->PostMessage(WM_CLOSE);
   	}
	if ( m_pWndProgressDlg )
	{
		m_pWndProgressDlg->DestroyWindow();
		delete m_pWndProgressDlg;
		m_pWndProgressDlg = NULL;
	}
// 	if (m_pChannelManager != NULL && m_lConnectHandle > 0)
// 	{
// 		m_lConnectHandle = 0;
// 	}
	m_bFirstFlag = TRUE;

	m_stockManager.RemoveAllStocks();
	m_bourseManager.RemoveAllBourse();
	m_subscriptionManager.RemoveAllSubscriptions();
	m_bourseManager.RemoveAllBourse();
	m_datasinkconnectManager.RemoveAllElement();
	return TRUE;
}


long CDataSourceImpl::HSDataSource_DataSouceInit(IDataSourceSink* pSink, const char* pszProfile)
{
	if (pSink == NULL)
	{
		return 0;
	}
	if ( pszProfile != NULL )
		m_lNotifySink = (long)pSink;
	InitEnv();
	return (long)pSink;
}

BOOL CDataSourceImpl::HSDataSource_DataSourceUnInit(long lDataSourceID)
{
	m_subscriptionManager.RemoveSink(lDataSourceID);
	long lConnecthandle = m_datasinkconnectManager.RemoveSink(lDataSourceID);
	if (lConnecthandle > 0)
	{
		CDataSourceSinkToConnectID* pSinkConnect = m_datasinkconnectManager.GetDataSinkID(lConnecthandle);
		if (pSinkConnect == NULL && m_pChannels != NULL)		//使用完成
		{
			m_pChannels->UnRegisterProcessor(lConnecthandle, m_pfnOnPacketReceive);
		}
	}
	

	if (m_datasinkconnectManager.GetSize() <= 0 )
	{
//		FreeAllUnit();
	}
	return FALSE;
}


BOOL CDataSourceImpl::HSDataSource_RequestAsyncData(long lDataSourceID, char* pAskData, int nAskLen, int nTimeOut, 
													UINT uSendOptions,  long lConnectHandle, long lPackID)
{
	if(lConnectHandle == 0 )
		lConnectHandle = this->DecideConnectByAskData((AskData*)pAskData);
	if (lConnectHandle == 0)
		return FALSE;
	SendSpecialSink(pAskData, nAskLen);

	if(uSendOptions & e_DataSouceSend_JYData)
	{
		if(m_datasinkconnectManager.AddUpdateNew(lDataSourceID, lConnectHandle, lPackID, uSendOptions))	//有新的连接
		{
			if (m_pChannels != NULL)
			{
				m_pChannels->RegisterProcessor(lConnectHandle, m_pfnOnPacketReceive);
			}
		}
		if (m_pChannels != NULL)
		{
			m_pChannels->AsyncSend(lConnectHandle, pAskData, nAskLen, nTimeOut);
		}
		return TRUE;
	}
	//数据下载
	if ( uSendOptions & e_DataSourceSend_HQDataDownLoad )
	{
		if ( m_datasinkconnectManager.AddUpdateNew(lDataSourceID, lConnectHandle,0, uSendOptions) )
		{
			if (m_pChannels != NULL)
			{
				m_pChannels->RegisterProcessor(lConnectHandle, m_pfnOnPacketReceive);
			}
		}
		if (m_pChannels != NULL)
		{
			m_pChannels->AsyncSend(lConnectHandle, pAskData, nAskLen, nTimeOut);
		}
		return TRUE;
	}
	//行情数据
	m_datasinkconnectManager.AddUpdateNew(lDataSourceID, lConnectHandle);
	if(m_pChannels != NULL)
	{
		if (pAskData != NULL && nAskLen > 0)
		{
			UINT uPackID = m_pChannels->GetPacketID(lConnectHandle);
			int nSize = sizeof(CDataSourceReqestResponceData);// + nAskLen;
			CDataSourceReqestResponceData* pReuest= (CDataSourceReqestResponceData*)new char[nSize];
			memset(pReuest, 0, nSize);
			pReuest->m_lDataSinkID = lDataSourceID;
			pReuest->m_nAskDataLen = nAskLen;
			pReuest->m_uStatus = eDataSourceRequestResponceStauts_notprocess;
			pReuest->m_uOption = uSendOptions;
			pReuest->m_nTimeOut = nTimeOut;
 			char* pDataBuffer = new char[nAskLen + 1];
 			memset(pDataBuffer, 0, nAskLen + 1);
 			memcpy(pDataBuffer, pAskData, nAskLen);
 			AskData* pAsk = (AskData*)pDataBuffer;
			pReuest->m_pRequestResponceData = pDataBuffer;

 			BOOL bRet = CtrolRequest(pReuest);
 			delete[] pDataBuffer;
 			delete[] pReuest;
			return bRet;
		}
		else		//相当于交易端的清空-- 模拟状况，当交易端订阅600000后，输入框中输入“000”，这时需要把原订阅取消
		{
			m_subscriptionManager.RemoveSink(lDataSourceID);
		}
	}
	return FALSE;
}

void* CDataSourceImpl::HSDataSource_RequestSyncData(long lDataSourceID, char* pAskData, int nAskLen, int nTimeOut, 
													int& nLen, UINT uSendOptions, long lConnectHandle, long lPackID)
{
	if (m_pChannels == NULL )
	{
		return NULL;
	}
	if (lConnectHandle == 0)
	{
		lConnectHandle = DecideConnectByAskData((AskData*)pAskData);
	}
	if (lConnectHandle == 0)
	{
		return 0;
	}
	if (pAskData == NULL || IsBadReadPtr(pAskData,1))
		return NULL;
	SendSpecialSink(pAskData, nAskLen);
	char* pszRetBuffer = NULL;
	UINT uLen = 0;
	AskData* pAsk = (AskData*)pAskData;
	pAsk->m_lKey = m_pChannels->GetPacketID(lConnectHandle);
	int nRetVal = m_pChannels->SendRcv(lConnectHandle, pAskData, nAskLen, nTimeOut, pszRetBuffer, uLen);
	if (nRetVal == 0)
	{
		nLen = uLen;
		return pszRetBuffer;
	}
	else
	{
		uLen = 0;//m_pChannels->GetLastEorror(lConnectHandle);
		return NULL;
	}
	return NULL;
}

BOOL CDataSourceImpl::HSDataSource_SyncDataRefComplete(long lDataSourceID, void* pRefData, int nRefDataLen, long lConnectHandle)
{
	long lConnectFreeMem = lConnectHandle;
	if (lConnectFreeMem == 0)
	{
		//lConnectFreeMem = m_lConnectHandle;
	}
	if (m_pChannels != NULL && lConnectFreeMem != 0)
	{
		m_pChannels->FreeMemory(lConnectFreeMem, (char*)pRefData);
		return TRUE;
	}
	return FALSE;
}

CString	CDataSourceImpl::HSDataSource_GetLastError(long lConnectHandle)
{
	long lConnectGetError = lConnectHandle;
	if (lConnectGetError == 0)
	{
		//lConnectGetError = m_lConnectHandle;
	}
	if (m_pChannels != NULL && lConnectGetError != 0)
	{
		return m_pChannels->GetLastEorror(lConnectGetError);
	}
	return _T("");
}

void    CDataSourceImpl::HSDataSource_SetDefaultConnectHandle(IN long lConnectHandle)
{
	//m_lConnectHandle = lConnectHandle;
}
long CDataSourceImpl::HSDataSource_ServerDataInit(long lConnectHandle, PFN_INITSTATUS_NOTIFY fn_Notify, CWnd* pWnd)
{
	InitEnv();
	m_pfnInitStatusNotify = fn_Notify;
	m_pNotifyWnd = pWnd;
	if (lConnectHandle > 0)
	{
		if (m_pChannels != NULL)
		{
			m_pChannels->RegisterProcessor(lConnectHandle, m_pfnOnPacketReceive);
		}	
		Server_type svrType = m_pChannels->GetServerTypeByHandle(lConnectHandle);
		m_autoRequest.OnLoginSuccess(svrType);
		InitMarketStocksInfo();
		m_autoRequest.ReqServerInitInfo(svrType);
		if (m_pfnInitStatusNotify != NULL)
		{
			CDataSourceInitDataStauts initstatus;
			memset(&initstatus, 0, sizeof(CDataSourceInitDataStauts));
			strcpy(initstatus.m_szMsg, "正在初始化...");
			m_pfnInitStatusNotify(&initstatus, pWnd);
		}
	}
	m_datasinkconnectManager.AddUpdateNew(TEMP_DATASOURCE_ID, lConnectHandle);

	return 0;
}	

BOOL CDataSourceImpl::HSDataSource_ReConnectNotify( long lConnectHandle)
{
	//断线重连
	if ( lConnectHandle > 0 )
	{
// 		m_subscriptionManager.RemoveAllSubscriptions();

		if (m_pChannels != NULL)
		{
			m_pChannels->RegisterProcessor(lConnectHandle, m_pfnOnPacketReceive);
		}
		CommNotify notify;
		notify.m_uType = eDataSource_ReConnect;
		notify.m_svrType = m_pChannels->GetServerTypeByHandle(lConnectHandle);
		m_datasinkconnectManager.NotifyCommStatus(&notify, -1);
		return TRUE;
	}
	return FALSE;
}

BOOL CDataSourceImpl::HSDataSourceEx_GetStockUserInfo(CodeInfo* pInfo, StockUserInfo& stockInfo)
{
	CHSDSStockInfoIntact* pInfoIntact = m_stockManager.GetStock(pInfo);
	if (pInfoIntact != NULL)
	{
		stockInfo.Copy(&pInfoIntact->m_stockInfo);
		return TRUE;
	}
	return FALSE;
}

BOOL CDataSourceImpl::HSDataSourceEx_GetServerDataTime(long &lDate, long &lTime)
{
/*	CTime tmcur = CTime::GetCurrentTime();

	CTime tmRetData = tmcur - CTimeSpan(m_nServerDateSpan, 0, 0, 0);
	int dwTimeSpan = m_nServerTimeSpan;
	int dwHour = dwTimeSpan / 10000;
	int dwMin = (dwTimeSpan % 10000) / 100;
	int dwSec = (dwTimeSpan % 100);
	CTime tmRetTime = tmcur - CTimeSpan(0, dwHour, dwMin, dwSec);
	lDate = atol(tmRetData.Format("%Y%m%d"));
	lTime = atol(tmRetTime.Format("%H%M%S"));*/

	lDate = m_lServerTime;
	return TRUE;
}

int	CDataSourceImpl::HSDataSourceEx_GetMarketStocksInfo(unsigned short nMarket, CArray<StockUserInfo>& stockArray)
{	
	return m_stockManager.GetMarketStockList((HSMarketDataType*)&nMarket, 1, stockArray);
}

int	CDataSourceImpl::HSDataSourceEx_GetSystemMarketInfo(unsigned short* pMarket, int nMarketCount, CArray<StockType>& blockArray)
{
	return m_bourseManager.GetSystemMarketInfo(pMarket, nMarketCount, blockArray);
}

BOOL CDataSourceImpl::HSDataSourceEx_GetStockUserInfo(char* pszCode, unsigned short nMarket, StockUserInfo& stockInfo, unsigned short nMarketException)
{
	CHSDSStockInfoIntact* pInfoIntact = m_stockManager.GetStock(pszCode, nMarket, nMarketException);
	if (pInfoIntact != NULL)
	{
		stockInfo.Copy(&pInfoIntact->m_stockInfo);
		return TRUE;
	}
	return FALSE;
}

int	CDataSourceImpl::HSDataSourceEx_GetStocksUserInfo(IN CArray<CodeInfo>& arInfo, IN OUT CArray<StockUserInfo>& stockInfo, BOOL bCompareOnlyMainMarket)
{
	return m_stockManager.GetStocksList(&arInfo, stockInfo, bCompareOnlyMainMarket);
}

int	CDataSourceImpl::HSDataSourceEx_GetStocksFinanceData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentFinanceData>& ayFinanceData)
{
	return m_stockManager.GetStocksFinanceData(&arInfo, ayFinanceData);
}

int	CDataSourceImpl::HSDataSourceEx_GetStocksExrightData(IN CArray<CodeInfo>& arInfo, IN OUT CArray<CurrentExRightData>& ayExrightData)
{
	return m_stockManager.GetStocksExrighData(&arInfo, ayExrightData);
}

BOOL CDataSourceImpl::OnReceivePacket(const char* pszBuffer,UINT uLen)
{
	PacketInfo *pInfo = (PacketInfo *)pszBuffer;
	if (pInfo->m_lDataTotal != pInfo->m_lDataTransmited)
		return FALSE;
	SendSpecialSink(pszBuffer, uLen);
	BOOL bRet = FALSE;
	CDataSourceReqestResponceData* pData = new CDataSourceReqestResponceData;
	if (pData != NULL)
	{
		pData->m_lDataSinkID = pInfo->m_lHandle;
		pData->m_nAskDataLen = pInfo->m_lDataTotal;
		pData->m_uStatus = eDataSourceRequestResponceStauts_recvnotPorcess;
		PacketInfo* pDataRecv = (PacketInfo*)new char[sizeof(PacketInfo)]; //&pData->m_szRequestResponceData;
		memset(pDataRecv, 0, sizeof(PacketInfo));

		memcpy(pDataRecv, pInfo, sizeof(PacketInfo));
		char* pDataBuffer = new char[pInfo->m_lDataTotal];
		memset(pDataBuffer, 0, pInfo->m_lDataTotal);
		memcpy(pDataBuffer, pInfo->m_pszData, pInfo->m_lDataTotal);
		pDataRecv->m_pszData = pDataBuffer;
		pData->m_pRequestResponceData = (char*)pDataRecv;
		bRet = CtrolResponce(pData);
		delete[] pDataBuffer;
		delete[] pDataRecv;
	}
	
	delete pData;

	return bRet;
}

BOOL CDataSourceImpl::OnNetConnectNotify(ConnectNotify* pNotify)
{
	return FALSE;
}

LRESULT	CDataSourceImpl::OnAsyncNotify(WPARAM wParam, LPARAM lParam)
{
	CDataSourceReqestResponceData* pData = (CDataSourceReqestResponceData*)lParam;
	if (pData != NULL && !::IsBadReadPtr(pData, sizeof(CDataSourceReqestResponceData)))
	{
		//if (m_pChannels != NULL && m_lConnectHandle > 0 && pData->m_uStatus == eDataSourceRequestResponceStauts_processing)
		if (m_pChannels != NULL && pData->m_uStatus == eDataSourceRequestResponceStauts_processing)
		{
			AskData* pAsk = (AskData*)pData->m_pRequestResponceData;
			long lConnectHandle = DecideConnectByAskData(pAsk);
			m_pChannels->AsyncSend(lConnectHandle, pData->m_pRequestResponceData, pData->m_nAskDataLen);
		}
		else if (pData->m_uStatus == eDataSourceRequestResponceStauts_recvPorcess || pData->m_uStatus == eDataSourceRequestResponceStauts_partreaddisk)
		{
			PacketInfo* pInfo = (PacketInfo*)pData->m_pRequestResponceData;
			m_datasinkconnectManager.SendSinkData(pData->m_lDataSinkID, (void*)pInfo->m_pszData, pInfo->m_lDataTotal);
		}
		else if ( pData->m_uStatus == eDataSourceRequestResponceStauts_allreaddisk)
		{
			PacketInfo* pInfo = (PacketInfo*)pData->m_pRequestResponceData;
			m_datasinkconnectManager.SendSinkData(pData->m_lDataSinkID, (void*)pInfo->m_pszData, pInfo->m_lDataTotal);
			if (pData && pData->m_pRequestResponceData && !IsBadReadPtr(pData->m_pRequestResponceData,1)) //读文件时 清除包内容缓存
			{
				delete[] pData->m_pRequestResponceData;
			}
		}		
		else if (pData->m_uStatus == eDataSourceRequestResponceStauts_loacal)
		{
			ProceLocalDeal(pData);
		}
		else if(eDataSourceRequestResponceStauts_jypacknofiy == pData->m_uStatus)
		{			
			m_datasinkconnectManager.NotifySinkData(pData->m_lDataSinkID, pData->m_pRequestResponceData, pData->m_nAskDataLen);
		}
		return 1;
	}
	return 0;
}

LRESULT	CDataSourceImpl::OnStatusNotify(WPARAM wParam, LPARAM lParam)
{
	if (m_pfnInitStatusNotify != NULL)
	{
		CString strMsg = m_autoRequest.GetStatus(lParam);
		if (strMsg.IsEmpty())
		{
			m_pfnInitStatusNotify(NULL, m_pNotifyWnd);
			m_datasinkconnectManager.SendAllInitFinishInfo();
			return 1;
		}

		CDataSourceInitDataStauts initstatus;
		memset(&initstatus, 0, sizeof(CDataSourceInitDataStauts));
		strncpy(initstatus.m_szMsg, strMsg, min(sizeof(initstatus.m_szMsg), strMsg.GetLength()));
		m_pfnInitStatusNotify(&initstatus, m_pNotifyWnd);
		return 1;
	}
	return 0;
}

BOOL CDataSourceImpl::CtrolRequest(CDataSourceReqestResponceData* pData)
{
	if (pData == NULL)
		return FALSE;
	BOOL bReadDisk = FALSE;
	if (pData->m_uOption & e_DataSouceSend_ReadDisk )
		bReadDisk = TRUE;
	AskData* pAsk = (AskData*)pData->m_pRequestResponceData;
	if (pAsk == NULL)
	{
		return FALSE;
	}

	if (pAsk->m_nType == RT_STOCKINFOLIST)		//不需要到服务器
	{
		while (!m_bInitFinished)
		{
			Sleep(50);
		}
		CDataSourceReqestResponceData dataSend;
		memcpy(&dataSend, pData, sizeof(CDataSourceReqestResponceData));
		dataSend.m_uStatus = eDataSourceRequestResponceStauts_loacal;

		AskData* pAsk = (AskData*)pData->m_pRequestResponceData;
		int		nReplyLen = 0;
		char*	pszHead = NULL;

		if (pAsk != NULL && pAsk->m_nType == RT_STOCKINFOLIST)
		{
			ReqStockInfoList* pReqStockInfoList = (ReqStockInfoList*)pAsk->m_pCode;
			if (pReqStockInfoList != NULL)
			{
				int nMarketCount = pReqStockInfoList->m_nSize;

				CArray<StockUserInfo> ayStock;
				int nCount = m_stockManager.GetMarketStockList(pReqStockInfoList->m_nMarketType, nMarketCount, ayStock);

				if (nCount >= 0)
				{
					nReplyLen = nCount * sizeof(StockUserInfo) + sizeof(AnsStockInfoList);

					pszHead = new char[nReplyLen];
					memset(pszHead, 0, nReplyLen); 
					AnsStockInfoList* pAnsStockList = (AnsStockInfoList*)pszHead;
					pAnsStockList->m_nSize = nCount;
					pAnsStockList->m_dhHead.m_nType = RT_STOCKINFOLIST;
					StockUserInfo* pInfo = (StockUserInfo*)pAnsStockList->m_pnowData;

					for (int i=0; i<ayStock.GetCount(); i++)
					{
						StockUserInfo* pStockCur = &ayStock.GetAt(i);
						if (pStockCur != NULL)
						{
							memcpy(pInfo, pStockCur, sizeof(StockUserInfo));
						}
						pInfo ++;
					}
				}
			}
		}
		dataSend.m_pRequestResponceData = pszHead;
		dataSend.m_nAskDataLen = nReplyLen;

		if (m_pWndDataRecv != NULL && m_pWndDataRecv->m_hWnd != NULL && ::IsWindow(m_pWndDataRecv->m_hWnd))
				m_pWndDataRecv->SendMessage(WM_THREAD_ASYNC, 0, (LPARAM)&dataSend);
		pData->m_uStatus = eDataSourceRequestResponceStauts_sended;
		if (pszHead != NULL)
		{
			delete []pszHead;
			pszHead = NULL;
		}
	}
	else
	{
		//生成请求索引
		InterlockedIncrement(&g_nIndex);
		if ( pAsk->m_nType != RT_TECHDATA_EX)
			pAsk->m_lKey = g_nIndex;
		char* pRequestData = NULL;
		int nSendLen = 0;
		BOOL bDeleteFlag = FALSE;
		BOOL bNeedRequest = FALSE;

		if (pAsk->m_nType == RT_REPORTSORT || pAsk->m_nType == RT_REPORTSORT_EXT 
			|| pAsk->m_nType == RT_TECHDATA_EX || pAsk->m_nType == RT_HISTREND )
		{
			pRequestData = pData->m_pRequestResponceData;
			nSendLen = pData->m_nAskDataLen;
			bNeedRequest = m_subscriptionManager.UpdateOneSubscription(pAsk->m_nType, pData->m_lDataSinkID, pAsk);
		}
		else
		{
			bDeleteFlag = TRUE;
			int nNewAskCount = 0;
			pRequestData = m_subscriptionManager.GetRequestPack(pData, nNewAskCount);
			nSendLen = sizeof(AskData) + sizeof(CodeInfo) * (nNewAskCount - 1);
			if (nNewAskCount > 0)
			{
				bNeedRequest = TRUE;
			}
		}
		//通讯库已支持多线程，不需要发送到主线程进行了
		long lConnectHandle = DecideConnectByAskData(pAsk);
		if (m_pChannels != NULL && lConnectHandle > 0 && /*pData->m_uStatus == eDataSourceRequestResponceStauts_processing &&*/ bNeedRequest)
		{
			m_pChannels->AsyncSend(lConnectHandle, pRequestData, nSendLen);
		}
		pData->m_uStatus = eDataSourceRequestResponceStauts_sended;
		if (pRequestData != NULL && bDeleteFlag)
		{
			delete []pRequestData;
			pRequestData = NULL;
		}
		if (bReadDisk)
		{
			if ( pAsk->m_nType == RT_TECHDATA_EX )
			{
				ReqDayData* pReqdata = (ReqDayData*)(pAsk->m_pCode);
				long lSize;
				BOOL bSendSvr = FALSE;
				DWORD time = GetTickCount();
				void* pCacheData = m_DataCache.MakeDayData(pAsk,lSize,bSendSvr );
				DWORD nowtime = GetTickCount();
				TRACE1("\r\n get tech tiem is %d", nowtime-time);
				if (pCacheData && !IsBadReadPtr(pCacheData,1))
				{		
					PacketInfo DataRecv;
					memset(&DataRecv, 0, sizeof(PacketInfo));
					DataRecv.m_pszData = (const char*)pCacheData;
					DataRecv.m_lDataTotal = lSize;				
					DataProcess_DiskDataEx(&DataRecv);				
					delete[] pCacheData;
				}								
			}		
			else
			{
 				long lSize;
 				void* pCacheData = NULL;
				pCacheData = m_DataCache.ReadFile(pAsk,lSize);
				//分时 tick
 				if (pCacheData && !IsBadReadPtr(pCacheData,1))
 				{
					PacketInfo DataRecv;
					memset(&DataRecv, 0, sizeof(PacketInfo));
					DataRecv.m_pszData = (const char*)pCacheData;
					DataRecv.m_lDataTotal = lSize;
					//发送到外部
					DataProcess_DiskDataEx(&DataRecv);				
					delete[] pCacheData;
 				}
			}
		}
	}
	return TRUE;
}

BOOL CDataSourceImpl::CtrolResponce(CDataSourceReqestResponceData* pData)
{

	if (pData != NULL )
	{
		PacketInfo* pRecv = (PacketInfo*)pData->m_pRequestResponceData;
		if (pRecv != NULL)
		{
			DataHead* pHead = (DataHead*)pRecv->m_pszData;
			
			if (pHead != NULL && !::IsBadReadPtr(pHead, sizeof(DataHead)))
			{
				int nReqType = pHead->m_nType;
				ProcessRecvData(pData);
			}
		}
		pData->m_uStatus = eDataSourceRequestResponceStauts_complete;
	}

	return TRUE;
}

BOOL CDataSourceImpl::SendDataToSinks(UINT uAskType, CodeInfo* pInfo, char* pszDatabuff, int nBufferLen,UINT uStatus)
{
	CDataSourceSubscription* pDataSubscription = m_subscriptionManager.GetSubscription(uAskType, pInfo);
	if (pDataSubscription != NULL && uAskType != RT_LOGIN)
	{
		CDataSourceReqestResponceData dataSend;
		dataSend.m_nAskDataLen = nBufferLen;
		dataSend.m_pRequestResponceData = pszDatabuff;
		dataSend.m_uStatus = eDataSourceRequestResponceStauts_recvPorcess;
		if ( uStatus != -1)
			dataSend.m_uStatus = uStatus;
		if (pDataSubscription->m_listDataSinkID.size() > 0)
		{
			for (std::list<long>::iterator it = pDataSubscription->m_listDataSinkID.begin(); it != pDataSubscription->m_listDataSinkID.end(); it ++)
			{
				long lDataSourceSink = (long)*it;			
				dataSend.m_lDataSinkID = lDataSourceSink;
				if (m_pWndDataRecv != NULL && m_pWndDataRecv->m_hWnd != NULL && ::IsWindow(m_pWndDataRecv->m_hWnd))
					m_pWndDataRecv->SendMessage(WM_THREAD_ASYNC, 0, (LPARAM)&dataSend);
				
			}
			if ( uStatus != eDataSourceRequestResponceStauts_allreaddisk && uStatus == eDataSourceRequestResponceStauts_partreaddisk)
				pDataSubscription->m_dwTickResponce = GetTickCount();
		}
		dataSend.m_uStatus = eDataSourceRequestResponceStauts_complete;
	}

	return FALSE;
}

BOOL CDataSourceImpl::ProcessRecvData(CDataSourceReqestResponceData* pData)
{
	if (pData != NULL )
	{
		PacketInfo* pRecv = (PacketInfo*)pData->m_pRequestResponceData;
		if (pRecv != NULL)
		{
			CDataSourceSinkToConnectID* pSinkConnect = m_datasinkconnectManager.GetDataSinkID(pRecv->m_lHandle);
			if (pSinkConnect != NULL && pSinkConnect->m_uSinkType & e_DataSouceSend_JYData)
			{
				long lPackID = 0;
				if (pRecv->m_pszData == NULL )
				{
					lPackID = pRecv->m_lDataTotal;	//如果数据超时，传输数量为packID
				}
				else
				{
					CPacker packer;
					if(packer.ParseData((char*)pRecv->m_pszData, pRecv->m_lDataTotal))
					{
						lPackID = packer.GetHeader(phSenderID);
					}
				}

				//交易包先到交易模块执行解包，再通知到主线程做同步
				long lSinkID = m_datasinkconnectManager.SendSinkData(pRecv->m_lHandle, lPackID, (void*)pRecv->m_pszData, pRecv->m_lDataTotal);
				if(lSinkID > 0)
				{
					CDataSourceReqestResponceData dataSend;
					dataSend.m_nAskDataLen = 0;
					dataSend.m_pRequestResponceData = NULL;
					dataSend.m_uStatus = eDataSourceRequestResponceStauts_jypacknofiy;
					dataSend.m_lDataSinkID = lSinkID;
					if (m_pWndDataRecv != NULL && m_pWndDataRecv->m_hWnd != NULL && ::IsWindow(m_pWndDataRecv->m_hWnd))
						m_pWndDataRecv->SendMessage(WM_THREAD_ASYNC, 0, (LPARAM)&dataSend);
				}
			}
			//数据下载
			else if ( pSinkConnect != NULL && pSinkConnect->m_uSinkType & e_DataSourceSend_HQDataDownLoad)
			{
				m_datasinkconnectManager.SendSinkData(pRecv->m_lHandle, 0, (void*)pRecv->m_pszData, pRecv->m_lDataTotal);
			}
			else	//行情数据
			{
				DataHead* pHead = (DataHead*)pRecv->m_pszData;
				CReplyDataManger replayManager;
				TRACE("处理接收数据, 请求类型%x\n", pHead->m_nType);
				int nSize = DataProcessing(pRecv, replayManager);
				if ( pHead->m_nType != RT_TECHDATA_EX)
				{
					if (nSize >= 0)
					{					
						replayManager.ReplyToSink(m_pWndDataRecv->m_hWnd);				
						m_DataCache.WriteFile(pRecv, &pHead->m_nPrivateKey.m_pCode);
						replayManager.RemoveAllElement();	
					}
					else	//未处理
					{
						SendDataToSinks(pHead->m_nType, NULL, pData->m_pRequestResponceData, pData->m_nAskDataLen);
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL CDataSourceImpl::ProceLocalDeal(CDataSourceReqestResponceData* pData)
{
	IDataSourceSink* pSinkCur = (IDataSourceSink* )pData->m_lDataSinkID;
	if (pSinkCur != NULL && pData->m_lDataSinkID != TEMP_DATASOURCE_ID)
	{			
		m_datasinkconnectManager.SendSinkData(pData->m_lDataSinkID, pData->m_pRequestResponceData, pData->m_nAskDataLen);
	}
	return TRUE;
}


void* CDataSourceImpl::HSDataSource_GetDayData( IN void* pAsk, IN int nMask /*= 0*/,IN int nExRight /*= 0*/ )
{
	CHSDisk disk;
	return disk.GetDayData(pAsk, nMask, nExRight);
}

long CDataSourceImpl::DecideConnectByAskData(AskData* pAsk)
{
	if(pAsk == NULL)
		return 0;
	IAccountMng *pMng = m_pTradeCore->GetAccountMng();
	if(pMng == NULL)
		return 0;
	EAccountType nType;
	switch(pAsk->m_cOperator)
	{
	case CEV_Connect_QH://期货
		nType = atFuture;
		break;
	case CEV_Connect_LEVEL2://level
		nType = atLevel2;
		break;
	default://股票
		nType = atLevel1;
	}
	IAccount *pAcc = pMng->GetCurrentAccount(nType);
	ASSERT(pAcc != NULL);
	return pAcc->GetConnectHandle();
}

BOOL CDataSourceImpl::HSDataSource_WriteDayData( CodeInfo* info,int period,StockCompDayDataEx* pNewData,int nSize )
{
	CHSDisk disk;
	return disk.WriteDayDataNew(info, period, pNewData,nSize);
}

BOOL CDataSourceImpl::SendSpecialSink(const char* pszDataBuff, long lDatalen, long lSink )
{
	BOOL bRet = FALSE; 
	if ( lSink == -1 && m_lNotifySink == -1 )
		return FALSE;
	if (lSink == -1 )
	{
		lSink = m_lNotifySink;
	}
	IDataSourceSink* pSink = (IDataSourceSink*)lSink;
	if (pSink != NULL && !::IsBadReadPtr(pSink, sizeof(long)))
	{
		pSink->HSDataSourceSink_OnRecvData((void*)pszDataBuff, lDatalen);
		bRet = TRUE;
	}
	return bRet;
}

int CDataSourceImpl::HSDataSource_FindStockFromKey( StockUserInfo* pStock, const CString& strKey,int& nUnique, int& nStart,int& nEnd, CString &szMarch,int& nBetter,const int nCurIndex )
{
	if ( m_bIniting )
		return m_stockManager.FindStockFromKey(pStock,strKey,nUnique, nStart, nEnd, szMarch, nBetter, nCurIndex);
	return -1;
}

CString CDataSourceImpl::HSDataSource_GetStockPyjc( StockUserInfo* pStock )
{
	if ( m_bIniting )
		return m_stockManager.GetStockPyjc(pStock);
	return "";
}