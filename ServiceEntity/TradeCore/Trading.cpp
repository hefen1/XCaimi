/*******************************************************
  源程序名称:Trading.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易请求类实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "Trading.h"
#include "WinnerApplication.h"
#include "mmsystem.h"
#include "TradeCore.h"
#include "TradeFunc.h"
#pragma comment(lib,"winmm")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern ITradeCoreObj* pTradeCoreObj;

CTrading::CTrading()
{
	m_iAction = -1;
	m_Account = NULL;
	m_NVList = new CMapStringToString;
	m_ArDataList = new CMapStringToString;
	m_Packer = new CPacker;
	m_Ar1Packet = new CAr1Packet;
	m_sLastError = "";
	m_DataSource = (IDataSource*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	m_DataSourceHandle = m_DataSource->HSDataSource_DataSouceInit(this, NULL);
	m_TradingAsyncCallBack = NULL;
	m_nPackID = 0;
	m_Successful = TRUE;
	m_timerId = 0;
	m_pChannels = (IChannels*)CWinnerApplication::GetObject(OBJ_CHANNELS);
	m_lConnectHandle = 0;
	m_DataList = NULL;
	m_DataList = NULL;
	m_bAsyncCallBackFlag = TRUE;
}


void CTrading::Release()
{
	delete this;
}


CTrading::~CTrading()
{
	/*if (!IsBadReadPtr(m_DataSource, sizeof(int)))
	{
		m_DataSource->HSDataSource_DataSourceUnInit(m_DataSourceHandle);
	}	*/
	//Lock();
	if (m_timerId)
	{
		timeKillEvent(m_timerId);
	}	
	m_DataSource->HSDataSource_DataSourceUnInit(m_DataSourceHandle);
	//UnLock();
	delete m_NVList;
	delete m_ArDataList;
	delete m_Packer;
	delete m_Ar1Packet;
	if (m_DataList && !m_bAsyncCallBackFlag)
	{
		 m_DataList->Release();
	}
}
void CTrading::SetAction(const int iAction)
{
	m_iAction = iAction;
}

void CTrading::SetAccount(IAccount* pAccount)
{
	m_Account = pAccount;
	m_lConnectHandle = m_Account->GetConnectHandle();
}

BOOL CTrading::GetSuccessful()
{
	return m_Successful;
}

void CTrading::Clear()
{
	m_NVList->RemoveAll();
	m_ArDataList->RemoveAll();
	delete m_Packer;
	m_Packer = new CPacker;
	m_Ar1Packet->FreePacker();
	m_Ar1Packet->FreeUnPacker();
}

CString CTrading::GetRequestNVData(const CString sName)
{
	/*CString sTmp = "";
	m_NVList->Lookup(sName,sTmp);
	return sTmp;*/
	return GetBinStr(sName);
}

void CTrading::SetRequestNVData(const CString sName, CString sValue)
{
	//
	m_NVList->SetAt(sName, sValue);
}

CString CTrading::GetRequestARData(const CString sName)
{
	CString sTmp = "";
	m_ArDataList->Lookup(sName,sTmp);
	return sTmp;
}

void CTrading::SetRequestARData(const CString sName, CString sValue)
{
	m_ArDataList->SetAt(sName, sValue);
}

char* CTrading::GetBinData(const CString sName, int& iDataLen)
{
	return m_Packer->GetData(sName, iDataLen);
}

void CTrading::SetBinData(const CString scName, char* lpData, int iDataLen)
{
	m_Packer->SetData(ptBin, scName, lpData, iDataLen);
}

CString CTrading::GetBinStr(const CString sName)
{
	int iDataLen;
	CString sTmp = "";
	char* pTmpBuf = GetBinData(sName, iDataLen);
	if (pTmpBuf)
	{
		char* pBuf = new char[iDataLen + 1];
		memcpy(pBuf, pTmpBuf, iDataLen);
		memcpy(pBuf + iDataLen, "\0", 1);
		sTmp = pBuf;
		delete[] pBuf;
		pBuf = NULL;
		delete[] pTmpBuf;
		pTmpBuf = NULL;
	}
	return sTmp;
}

BOOL CTrading::TradingSync(const int iTimeOut /* = TRADE_TIMEOUT */)
{
	if (0 == m_lConnectHandle)
	{
		m_sLastError = _T("连接未建立！");
		return FALSE;
	}
	BOOL bTmp = FALSE;
	int iLen = 0;
	char* pBuf = TradePack(iLen);
	char* pRevBuf = NULL;
	int nRevLen = 0;
	//发包
	pRevBuf = (char*)m_DataSource->HSDataSource_RequestSyncData(m_DataSourceHandle, pBuf, iLen, iTimeOut*1000, nRevLen, e_DataSouceSend_JYData, m_lConnectHandle);


	//int iSend = m_pChannels->SendRcv(m_lConnectHandle,pBuf, iLen, iTimeOut*1000, pRevBuf, nRevLen);
	delete[] pBuf;
	pBuf = NULL;

	if ((pRevBuf) && (nRevLen > 0))
	{
		bTmp = ParseData(pRevBuf, nRevLen);
		m_DataSource->HSDataSource_SyncDataRefComplete(m_DataSourceHandle, pRevBuf, nRevLen, m_lConnectHandle);
	}
	else
	{
		m_sLastError = m_pChannels->GetLastEorror(m_lConnectHandle);
	}	
	
	return bTmp;
}

BOOL CTrading::TradingAsync(ITradingAsyncCallBack * lpTradingAsyncCallBack, const int iTimeOut /* = -1 */)
{
	if (0 == m_lConnectHandle)
	{
		m_sLastError = _T("连接未建立！");
		return FALSE;
	}
	m_Account->AddTradingAsync(this);
	m_TradingAsyncCallBack = lpTradingAsyncCallBack;
	BOOL bTmp = FALSE;
	int iLen = 0;
	char* pBuf = TradePack(iLen);
	int iTmp = -1;
	if (iTimeOut != -1)
	{
		iTmp = iTimeOut * 1000;
	}
	//发包
    m_DataSource->HSDataSource_RequestAsyncData(m_DataSourceHandle, pBuf, iLen, iTmp, e_DataSouceSend_JYData, m_lConnectHandle, m_nPackID);
	delete[] pBuf;
	pBuf = NULL;
	return bTmp;
}

IDataList* CTrading::GetDataList()
{
	return m_DataList;
}

char* CTrading::TradePack(int& iPackLen)
{
	CString sTmp = "";
	//打报头
	m_Packer->SetHeader(phAction, m_iAction);
	if (m_Account)
	{
		m_Packer->SetHeader(phCellIndex, m_Account->GetServerID());
		m_Packer->SetHeader(phOnlineID, m_Account->GetOnlineID());
		m_Packer->SetHeader(phOnlineToken, m_Account->GetOnlineToken());
		m_Packer->SetHeader(phClientAddr, m_Account->GetClientAddr());
	}
	else
	{
		m_Packer->SetHeader(phCellIndex, 0);
		m_Packer->SetHeader(phOnlineID, 0);
		m_Packer->SetHeader(phOnlineToken, 0);
	}
	m_Packer->SetHeader(phReturnCode, 0);
	m_nPackID = m_pChannels->GetPacketID(m_lConnectHandle);
	m_Packer->SetHeader(phSenderID, m_nPackID);
	//m_Packer->SetData(ptNV,S_NV_ReqSerialNo, "12345678", 8);
	
	/*if (m_Account)
	{
		sTmp = "1";
		m_Packer->SetData(ptNV, "version", (LPTSTR)(LPCTSTR)sTmp, sTmp.GetLength());
		sTmp = m_Account->GetClientID();
		m_Packer->SetData(ptNV, "client_id", (LPTSTR)(LPCTSTR)sTmp, sTmp.GetLength());
		sTmp = m_Account->GetAccountName();
		m_Packer->SetData(ptNV, "fund_account", (LPTSTR)(LPCTSTR)sTmp, sTmp.GetLength());
		sTmp = m_Account->GetPassword();
		m_Packer->SetData(ptNV, "password", (LPTSTR)(LPCTSTR)sTmp, sTmp.GetLength());
		sTmp = "7";
		m_Packer->SetData(ptNV, "op_entrust_way", (LPTSTR)(LPCTSTR)sTmp, sTmp.GetLength());
		sTmp = "1";
		m_Packer->SetData(ptNV, "entrust_safety", (LPTSTR)(LPCTSTR)sTmp, sTmp.GetLength());
	}*/
	

	//打NV字段
	POSITION ipos = m_NVList->GetStartPosition();
	while(ipos)
	{
		CString sKey = "";
		CString sValue = "";
		m_NVList->GetNextAssoc(ipos,sKey,sValue);
		m_Packer->SetData(ptNV,sKey,(LPTSTR)(LPCTSTR)sValue, sValue.GetLength());
	}	
	if (m_Account)
	{
		//打AR包
		CAr1Packer* pAr1Packer = m_Ar1Packet->GetAr1Packer();
		AR1PacketHeader* pAR1PacketHeader = pAr1Packer->GetHeader();
		pAR1PacketHeader->HeaderVersion = 1;
		CString sBranch = m_Account->GetBranchNo();
		if (sBranch.IsEmpty())
		{
			sBranch = "0";
		}
		pAR1PacketHeader->Branch = atoi(sBranch);
		pAR1PacketHeader->FunctionNo = m_iAction;
		pAR1PacketHeader->SenderID = 0;
		pAR1PacketHeader->PacketType = 0;
		pAR1PacketHeader->AddressLevel = 0;
		INodeTreeData* pReqParamList = m_Account->GetTradeConfigObj()->GetReqParamList(m_iAction);
		if (pReqParamList)
		{
			int nCount = pReqParamList->GetCount();
			for (int i = 0; i < nCount; i++)
			{
				INodeData* pItem = pReqParamList->GetItems(i);
				if (pItem)
				{
					CString sName = pItem->Name();
					CString sValue = pItem->Value();
					//m_ArDataList->SetAt(sName, sValue);
					pAr1Packer->AddData(sName, sValue);
					pItem->Release();
				}
				else
					break;
			}
			pReqParamList->Release();
		}
		CMapStringToString* pArSession = m_Account->GetArSession();
		if (pArSession->GetCount() > 0)
		{
			POSITION ipos = pArSession->GetStartPosition();
			while (ipos)
			{
				CString sKey = "";
				CString sValue = "";
				pArSession->GetNextAssoc(ipos,sKey,sValue);
				pAr1Packer->AddData(sKey, sValue);
			}
		}

		m_ArDataList->SetAt("client_id", m_Account->GetClientID());
		m_ArDataList->SetAt("fund_account", m_Account->GetAccountName());
		m_ArDataList->SetAt("password", m_Account->GetPassword());
		//m_ArDataList->SetAt("op_entrust_way", "7");
		//m_ArDataList->SetAt("request_num", "2000");
		if(m_ArDataList->GetCount() > 0)
		{
			POSITION ipos = m_ArDataList->GetStartPosition();
			while(ipos)
			{
				CString sKey = "";
				CString sValue = "";
				m_ArDataList->GetNextAssoc(ipos,sKey,sValue);
				pAr1Packer->AddData(sKey, sValue);
			}

			int iBufLen;
			char* pArBuf = pAr1Packer->GetPackBuf(iBufLen);

			//把AR包写入投资赢家包
			CString sArNVData = S_BIN_DATA;
			m_Packer->SetData(ptNV, S_BIN_REQ, S_BIN_DATA, sArNVData.GetLength());
			m_Packer->SetData(ptBin,S_BIN_DATA, pArBuf, iBufLen);
			delete[] pArBuf;
			pArBuf = NULL;
	    }		
	}
	char* pBuf = m_Packer->GetPackData(iPackLen);
	ITradeTcpView* pTcpView = pTradeCoreObj->GetTradeTcpView();
	if (pTcpView)
	{
		char* pCallBack = new char[iPackLen];
		memcpy(pCallBack, pBuf, iPackLen);
		pTcpView->CallBackData(pCallBack, iPackLen, m_lConnectHandle, m_iAction, 0);
	}
	return pBuf;
}

BOOL CTrading::ParseData(char* lpBuf, const UINT iBufLen)
{
	BOOL bTmp =  FALSE;
	try
	{
		ITradeTcpView* pTcpView = pTradeCoreObj->GetTradeTcpView();
		if (pTcpView)
		{
			char* pCallBack = new char[iBufLen];
			memcpy(pCallBack, lpBuf, iBufLen);
			pTcpView->CallBackData(pCallBack, iBufLen, m_lConnectHandle, m_iAction, 1);
		}
		if (m_Packer->ParseData(lpBuf, iBufLen)) 
		{			
			if (m_Account)
			{
				m_Account->SetOnlineID(m_Packer->GetHeader(phOnlineID));
				m_Account->SetOnlineToken(m_Packer->GetHeader(phOnlineToken));
				//m_Account->SetClientAddr(m_Packer->GetHeader(phClientAddr));				
			}		

			int iReturnCode = m_Packer->GetHeader(phReturnCode);
			if (0 == iReturnCode)
			{
				int iDataLen = 0;		
				CString sName = GetBinStr(S_BIN_ANS);
				char* pAr1Buf = NULL;
				if (sName.IsEmpty())
				{
					pAr1Buf = m_Packer->GetData(S_BIN_DATA_ANS, iDataLen);
				}
				else
					pAr1Buf = m_Packer->GetData(sName.MakeLower(), iDataLen);

				if (iDataLen > 0)
				{
					CAr1UnPacker* pAr1UnPacker = m_Ar1Packet->GetAr1UnPacker(pAr1Buf, iDataLen);

					DWORD nBegin = GetTickCount();
					DWORD nEnd = 0;
					TRACE1(_T("Trading Fill Datalist Begin：%d,\n"), nBegin);

					//发包成功解析返回包信息
					m_DataList = NewDataList();	
					if (m_DataList != NULL)
					{
						while(!pAr1UnPacker->isEof())
						{
							IDataItem* pDataItem = m_DataList->NewItem();
							CMapStringToString* lpAr1Item = pAr1UnPacker->GetItem();
							if (lpAr1Item)
							{
								POSITION ipos = lpAr1Item->GetStartPosition();
								while(ipos)
								{
									CString sKey = "";
									CString sValue = "";
									lpAr1Item->GetNextAssoc(ipos,sKey,sValue);
									pDataItem->AddData(sKey, sValue);
								}
								m_DataList->AddItem(pDataItem);
							}							
							pAr1UnPacker->Next();
						}

					}
					delete[] pAr1Buf;
					pAr1Buf = NULL;

					nEnd = GetTickCount();
					TRACE2(_T("Trading Fill Datalist End：%d, UseTime：%d\n"), nEnd, nEnd - nBegin);

				}	
				bTmp = TRUE;
			}
			else
			{
				int iDataLen = 0;
				char* szTmp = m_Packer->GetData("errormessage", iDataLen);
				char* szError = new char[iDataLen + 1];
				memcpy(szError, szTmp, iDataLen);
				delete[] szTmp;
				szTmp = NULL;
				memcpy(szError + iDataLen, "\0", 1);
				m_sLastError = szError;
				delete[] szError;
				szError = NULL;
			}
		}
		else
			m_sLastError = m_Packer->GetLastError();

	}
	catch (char* e)
	{
		m_sLastError = e;
	}
	catch (...)
	{
		m_sLastError = _T("服务器异常");
	}
	return bTmp;
}


void CTrading::SetConnectHandle(long nConnectHandle)
{
	m_lConnectHandle = nConnectHandle;
}

CString CTrading::GetLastError()
{
	if (m_sLastError.IsEmpty())
	{
		m_sLastError = "未知的原因请求失败！";
	}
	return m_sLastError;
}

IDataList* CTrading::NewDataList()
{
	CDataList* pDataList = new CDataList;
	return pDataList;
}

BOOL CTrading::HSDataSourceSink_OnCommNotify(void* pData)
{
	return TRUE;
}

BOOL CTrading::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{	

	DWORD nBegin = MyGetTickCount();
	TRACE1(_T("AsyncCall Begin：%d\n"), nBegin);

	m_bAsyncCallBackFlag = FALSE;
	char* pRevBuf = (char*)pData;
	if ((pRevBuf) && (nLen > 0))
	{
	    m_Successful = ParseData(pRevBuf, nLen);

		DWORD nEnd = MyGetTickCount();
		TRACE3(_T("ThreadId：%d, AsyncCall ParseData end：%d UseTime： %d\n"), GetCurrentThreadId(), nEnd, nEnd - nBegin);
	}
	else
	{
		m_Successful = FALSE;
		m_sLastError = _T("请求超时!");
	}	
	/*if (m_TradingAsyncCallBack)
	{
		m_TradingAsyncCallBack->TradingAsyncCallBack(this, m_iAction);
	}*/	
	//pTradeCoreObj->ThreadSync(this);	
	return TRUE;
}

BOOL CTrading::HSDataSourceSinkEx_RecvDataNotify()
{
	if (m_TradingAsyncCallBack)
	{
		m_TradingAsyncCallBack->TradingAsyncCallBack(this, m_iAction);
	}
	m_bAsyncCallBackFlag = TRUE;
	if(0 == m_timerId)
		m_timerId = timeSetEvent(100,100,&TradingTimerCallBack,(DWORD)this,TIME_PERIODIC);	
	return TRUE;
}

void CTrading::OnMMTimer()
{
	if (TIMERR_NOERROR == timeKillEvent(m_timerId))
	{
		m_Account->RemoveTradingAsync(this);
		delete this;
	}	
}

int CTrading::GetHeader(EWholePacketHeade eValue)
{
	return m_Packer->GetHeader(eValue);
}

BOOL CTrading::HaveError(CString &sMsg)
{
	sMsg = "";
	if (m_DataList)
	{
		if (m_DataList->GetCount() > 0)
		{
			IDataItem* pItem = m_DataList->GetItems(0);
			if (0 != pItem->GetErrorNo())
			{
				sMsg = pItem->GetErrorInfo();
				return TRUE;
			}
			else
				return FALSE;

		}
		else return FALSE;
	}
	else
	  return TRUE;
}

BOOL CTrading::HaveData()
{
	CString sMsg = "";
	return !HaveError(sMsg);
}

void CTrading::FreeDataList()
{
	if (m_DataList)
	{
		m_DataList->Release();
	}
}

void CTrading::CallBack()
{
	if (m_TradingAsyncCallBack)
	{
		m_TradingAsyncCallBack->TradingAsyncCallBack(this, m_iAction);
	}
}
