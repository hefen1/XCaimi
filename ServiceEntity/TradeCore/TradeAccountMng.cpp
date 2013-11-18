/*******************************************************
  源程序名称:TradeAccountMng.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  账号管理器类实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "TradeAccountMng.h"
#include "BusinessLogic.h"
#include "WinnerApplication.h"
#include "TradeCore.h"
#include "QueryFundIntf.h"
//#include "PackEntry.h"
#include "TradeLoginIntf.h"
#include "..\..\HelperLib\GeneralHelper.h"
#include "TradeAction.h"
#include "hsds_comudata.h" 
#include "TradeDataCore.h"
#include "hs_interface.h"
#include "as_packet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// 声明AR包动态库中的函数
typedef IKnown* (FUNCTION_CALL_MODE *GETPACKSERVICEINSTANCE)(IAppContext *);
#define ARDLL  _T("fsc_pack.dll")
//
extern ITradeCoreObj* pTradeCoreObj;

BOOL WINAPI sFn_OnConnectNotify(ConnectNotify* pszConnectNotify)
{
	pTradeCoreObj->GetAccountMng()->OnNetConnectNotify(pszConnectNotify);
	return TRUE;
}

IChannelManager* CAccount::m_pChannelManager = NULL;
IChannels* CAccount::m_pChannels = NULL;
IDataSourceEx* CAccount::m_pDataSource = NULL;
CAccount::CAccount()
{
  m_iAccountID = 0;
  m_CellNo = "";
  m_sBranchNo = "";
  m_iServerID = 0;
  m_iOnlineID = -1;
  m_iOnlineToken = -1;
  m_iClientAddr = -1;
  m_bIsLogined = FALSE;
  m_sLoginAccountType = "1";
  m_sLoginMarketType = "0";
  m_sLoginAccount = "";
  m_sPassword = "";
  m_sClientID = "";
  m_sAccountName = "";
  m_sUserName = "";
  m_bIsLocked = FALSE;
  m_sLastLoginInfo = "";
  m_sLoginFailCause = "";
  m_BusinessLogic = NULL;
  m_NVSession = new CMapStringToString;
  m_ArSession = new CMapStringToString;
  m_pChannelManager = NULL;
  m_pChannels = NULL;
  m_lConnectHandle = 0;
  m_bConnected = FALSE;
  m_pfnOnConnectNotify = sFn_OnConnectNotify;
  m_sJrName = "";
  m_sClientIP = "";
  m_pTradeConfigObj = (ITradeConfigObj*)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
  m_nAutoLoginNum = 0;
  m_TradeDataCore = NULL;
  /*INodeTreeData* pDefaultList = m_pTradeConfigObj->GetDefaultReqDataList();
  for (int i = 0; i < pDefaultList->GetCount(); i++)
  {
	  INodeData* pItem = pDefaultList->GetItems(i);
	  if(pItem)
	  {
		  CString sKey = pItem->Name();
		  CString sValue = pItem->Value();
		  m_ArSession->SetAt(sKey, sValue);
		  pItem->Release();
	  }	  
  }
  pDefaultList->Release();*/

  if (m_pChannelManager == NULL)
	  m_pChannelManager = (IChannelManager*)CWinnerApplication::GetObject(OBJ_CHANNELMANAGER);
  if (m_pChannels == NULL)
	  m_pChannels = (IChannels*)CWinnerApplication::GetObject(OBJ_CHANNELS);
  if (m_pDataSource == NULL)
	  m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
  
  m_pParamList = NULL;
}

CAccount::~CAccount()
{
	m_bIsLogined = FALSE;
	if (m_lConnectHandle)
	{
		m_pChannelManager->DestroyConnect(m_lConnectHandle);
	}	
	if (m_BusinessLogic)
	{
		m_BusinessLogic->Release();
	}	
	delete m_NVSession;
	delete m_ArSession;
	if (m_TradeDataCore)
	{
		m_TradeDataCore->Release();
	}
	if (m_pParamList)
	{
		m_pParamList->Release();
	}
}

void CAccount::Release()
{
	delete this;
}

void CAccount::FreeCache()
{
	for (int i = 0; i < m_TradingAsyncList.GetCount(); i++)
	{
		ITrading* pAsync = m_TradingAsyncList.GetAt(i);
		pAsync->Release();
	}
	m_TradingAsyncList.RemoveAll();

}

int CAccount::GetID()
{
	return m_iAccountID;
}

CString CAccount::GetCellNo()
{
	return m_CellNo;
}

void CAccount::SetCellNo(const CString Value)
{
	if (m_CellNo != Value)
	{
		m_CellNo = Value;
		ICellList* pCellList = m_pTradeConfigObj->GetCellList();
		INodeTreeData* pDefaultList = pCellList->GetDefaultReqDataList(Value);
		for (int i = 0; i < pDefaultList->GetCount(); i++)
		{
			INodeData* pItem = pDefaultList->GetItems(i);
			if(pItem)
			{
				CString sKey = pItem->Name();
				CString sValue = pItem->Value();
				m_ArSession->SetAt(sKey, sValue);
				pItem->Release();
			}	  
		}
		pDefaultList->Release();
		pCellList->Release();
	}
}

CString CAccount::GetBranchNo()
{
	return m_sBranchNo; 
}
void CAccount::SetBranchNo(const CString Value)
{
   //m_sBranchNo.format("%s", lpValue);
	m_sBranchNo = Value;
}

int CAccount::GetServerID()
{
	return m_iServerID;
}
void CAccount::SetServerID(const int Value)
{
	m_iServerID = Value;
}
int CAccount::GetOnlineID()
{
	return m_iOnlineID;
}
void CAccount::SetOnlineID(const int Value)
{
	if (m_iOnlineID != Value)
	{
		m_iOnlineID = Value;
	}	
}

int CAccount::GetOnlineToken()
{
	return m_iOnlineToken;
}
void CAccount::SetOnlineToken(const int Value)
{
	if (m_iOnlineToken != Value)
	{
		m_iOnlineToken = Value;
	}	
}

int CAccount::GetClientAddr()
{
	return m_iClientAddr;
}
void CAccount::SetClientAddr(const int Value)
{
	if (m_iClientAddr != Value)
	{
		m_iClientAddr = Value;
		IN_ADDR addTmp;
		//int iIP = ntohl(m_iClientAddr);
		int iIP = m_iClientAddr;
		memcpy(&addTmp, &iIP, 4);
		m_sClientIP = inet_ntoa(addTmp);
	}	
}

BOOL CAccount::GetIsLogined()
{
	return m_bIsLogined;
}
void CAccount::SetIsLogined(const BOOL Value)
{
	m_bIsLogined = Value;
}

CString CAccount::GetLoginAccountType()
{
	return m_sLoginAccountType;
}
void CAccount::SetLoginAccountType(const CString Value)
{
	m_sLoginAccountType = Value;
}

CString CAccount::GetLoginMarketType()
{
	return m_sLoginMarketType;
}

void CAccount::SetLoginMarketType(const CString Value)
{
	m_sLoginMarketType = Value;
}

CString CAccount::GetLoginAccount()
{
	return m_sLoginAccount;
}
void CAccount::SetLoginAccount(const CString Value)
{
	m_sLoginAccount = Value;
}

CString CAccount::GetPassword()
{
	return m_sPassword;
}
void CAccount::SetPassword(const CString Value)
{
	m_sPassword = Value;
}

CString CAccount::GetClientID()
{
	return m_sClientID;
}

void CAccount::SetClientID(const CString Value)
{
	m_sClientID = Value;
}

CString CAccount::GetAccountName()
{
	return m_sAccountName;
}
void CAccount::SetAccountName(const CString Value)
{
	m_sAccountName = Value;
}


CString CAccount::GetUserName()
{
	return m_sUserName;
}
void CAccount::SetUserName(const CString Value)
{
	m_sUserName = Value;
}

BOOL CAccount::GetIsLocked()
{
	return m_bIsLocked;
}
void CAccount::SetIsLocked(const BOOL Value)
{
	m_bIsLocked = Value;
}

CString CAccount::GetLastLoginInfo()
{
	return m_sLastLoginInfo;
}
void CAccount::SetLastLoginInfo(const CString Value)
{
	m_sLastLoginInfo = Value;
}

CString CAccount::GetLoginFailCause()
{
	return m_sLoginFailCause;
}
void CAccount::SetLoginFailCause(const CString Value)
{
	m_sLoginFailCause = Value;
}



IBusinessLogic* CAccount::GetBusinessLogic()
{
	if (NULL == m_BusinessLogic)
	{
		m_BusinessLogic = new CBusinessLogic;
		m_BusinessLogic->SetAccount(this);
	}
	return m_BusinessLogic;
}

ITradeDataCore* CAccount::GetTradeDataCore()
{
	if (NULL == m_TradeDataCore)
	{
		m_TradeDataCore = new CTradeDataCore;
	}
	return m_TradeDataCore;
}


CMapStringToString* CAccount::GetNVSession()
{
	return m_NVSession;
}

CMapStringToString* CAccount::GetArSession()
{
	return m_ArSession;
}

BOOL CAccount::Login(CString &sMsg, BOOL bReConnect)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_bIsLogined = FALSE;
	switch (this->m_nAccountType)
	{
	case atTrade:
		return LoginTrade(sMsg);
	case atLevel1:
		return LoginLevel1(sMsg, bReConnect);
	case atLevel2:
		return LoginLevel2(sMsg, bReConnect);
	case atFuture:
		return LoginFuture(sMsg,bReConnect);
	default:
		return FALSE;
	}
}

BOOL CAccount::LoginTrade(CString &sMsg)
{
	if ((NULL == m_pChannelManager)||(NULL == m_pChannels))
	{
		sMsg = _T("获取连接通道失败！");
		return FALSE;
	}
	if (0 == m_lConnectHandle)
	{
		
		TradeCert tcInfo;
		tcInfo.m_Type = SSL;
		strncpy_s(tcInfo.m_ca, _T("hsca"), sizeof(tcInfo.m_ca));
		CString sCertFile =  CGeneralHelper::GetAppPath() + _T("Cert\\cert.pem");
		strncpy_s(tcInfo.m_cert, sCertFile, sizeof(tcInfo.m_cert));
		CString sCertKey =  CGeneralHelper::GetAppPath() + _T("Cert\\key.pem");
		strncpy_s(tcInfo.m_key, sCertKey, sizeof(tcInfo.m_key));
		strncpy_s(tcInfo.m_pwd, _T("111111"), sizeof(tcInfo.m_pwd));
		m_lConnectHandle = m_pChannelManager->CreateConnector(CEV_Connect_JY, _T(""), &tcInfo);
		if(0 == m_lConnectHandle)
		{
			//CPackEntry PackEntry;
			//CString sError = PackEntry.GetLastError();
			m_bConnected = FALSE;
			sMsg = _T("建立连接失败！");
			return FALSE;
		}

		/*CommInfo server;
		strncpy_s(server.m_strServerID,  _T("hundsun"), sizeof(server.m_strServerID));
		strncpy_s(server.m_strServerIP, _T("60.191.25.162"), sizeof(server.m_strServerIP));
		//strncpy_s(server.m_strServerIP, _T("127.0.0.1"), sizeof(server.m_strServerIP));
		server.m_nPort = 7777;
		server.m_cType = CEV_Connect_JY;
		server.m_cIsDefault = 0;

		if(m_pChannelManager != NULL)
		{
			m_pChannelManager->SetSites(&server, 1, NULL);
			m_pChannelManager->RegisterNotify(m_pfnOnConnectNotify);
			TradeCert tcInfo;
			tcInfo.m_Type = SSL;
			strncpy_s(tcInfo.m_ca, _T("hsca"), sizeof(tcInfo.m_ca));
			CString sCertFile =  CGeneralHelper::GetAppPath() + _T("Cert\\cert.pem");
			strncpy_s(tcInfo.m_cert, sCertFile, sizeof(tcInfo.m_cert));
			CString sCertKey =  CGeneralHelper::GetAppPath() + _T("Cert\\key.pem");
			strncpy_s(tcInfo.m_key, sCertKey, sizeof(tcInfo.m_key));
			strncpy_s(tcInfo.m_pwd, _T("111111"), sizeof(tcInfo.m_pwd));
			m_lConnectHandle = m_pChannelManager->CreateConnector(CEV_Connect_JY, _T("hundsun"), &tcInfo);
			if(0 == m_lConnectHandle)
			{
				//CPackEntry PackEntry;
				//CString sError = PackEntry.GetLastError();
				sMsg = _T("建立连接失败！");
				return FALSE;
		    }
		}*/
	}
	m_bConnected = TRUE;
	CString sMacAdd = m_pChannelManager->GetMacAdd(m_lConnectHandle);
	m_ArSession->SetAt("macadd", sMacAdd);
	IReqLogin* pIntf = dynamic_cast<IReqLogin*>(pTradeCoreObj->NewTradingByAction(TRDADE_LOGIN));
	pIntf->SetAccount(this);
	pIntf->SetOpBranchNo(m_sBranchNo);
	pIntf->SetInputContent(m_sLoginAccountType);
	pIntf->SetContentType(m_sLoginMarketType);
	pIntf->SetAccountContent(m_sLoginAccount);
	pIntf->SetOpStation(m_sClientIP);
	//pIntf->SetConnectHandle(m_lConnectHandle);
	//pIntf->SetChannels(m_pChannels);
	if (pIntf->TradingSync())
	{		
		ILoginList* pList = pIntf->GetDataList();
		if (pList)
		{
			ILoginItem* pItem = pList->GetItems(0);
			if (pItem)
			{
				int iErrorNo = pItem->GetErrorNo();
				if (0 == iErrorNo)
				{					
					CString sFundAccount = pItem->GetFundAccount();
					IAccount* pAccount = pTradeCoreObj->GetAccountMng()->GetAccountByNameAndCellNo(sFundAccount, m_CellNo, m_nAccountType);
					if(pAccount && pAccount != this)
					{
						m_bIsLogined = FALSE;
						m_pChannelManager->DestroyConnect(m_lConnectHandle);
						m_lConnectHandle = 0;
						m_bConnected = FALSE;
						sMsg = _T("此账号已经登陆！");
					}
					else
					{
						m_sBranchNo = pItem->GetBranchNo();
						m_sClientID = pItem->GetClientID();
						m_sAccountName = sFundAccount;
						m_sUserName = pItem->GetClientName();
						m_ArSession->SetAt("op_station", m_sClientIP);
						m_ArSession->SetAt("op_branch_no", m_sBranchNo);
						m_ArSession->SetAt("branch_no", m_sBranchNo);
						m_bIsLogined = TRUE;
						sMsg = _T("登陆成功！");
					}
					
				}
				else
				{
					sMsg = pItem->GetErrorInfo();
					m_pChannelManager->DestroyConnect(m_lConnectHandle);
					m_lConnectHandle = 0;
					m_bIsLogined = FALSE;
					m_bConnected = FALSE;
				}

			}
			pIntf->FreeDataList();
		}		
	}
	else
	{
		sMsg = pIntf->GetLastError();
		m_bIsLogined = FALSE;
	}
	pIntf->Release();
	return m_bIsLogined;
}

BOOL CAccount::LoginLevel1(CString &sMsg,  BOOL bReConnect)
{
	if ((NULL == m_pChannelManager)||(NULL == m_pChannels))
	{
		sMsg = _T("获取连接对象失败！");
		return FALSE;
	}
	
	if (m_lConnectHandle == 0)
	{
		sMsg = _T("连接失败！");
		m_bConnected = FALSE;
		return FALSE;
	}
// 	BOOL bNeedInit(FALSE);
// 	if (m_lConnectHandle == 0)
// 	{// 连接断开
// 		m_lConnectHandle = m_pChannelManager->CreateConnector(CEV_Connect_HQ, "");
// 		if (m_lConnectHandle == 0)
// 		{
// 			sMsg = _T("连接站点失败");
// 			return FALSE;
// 		}
// 		bNeedInit = TRUE;
// 	}
	
	if (bReConnect)
	{// 断线重连
		long lConnectHandle = m_pChannelManager->CreateConnector(CEV_Connect_HQ, "");
		if (lConnectHandle == 0)
		{
			sMsg = _T("连接站点失败");
			m_bConnected = FALSE;
			return FALSE;
		}
		m_pChannelManager->DestroyConnect(m_lConnectHandle); // 销毁之前的连接
		m_lConnectHandle = lConnectHandle;
		//bNeedInit = TRUE;
	}
	m_bConnected = TRUE;

	int nCodeCount = sizeof(ReqLogin) / sizeof(CodeInfo) + ((sizeof(ReqLogin) % sizeof(CodeInfo))?1:0);
	int lRetLen    = sizeof(AskData) + sizeof(CodeInfo)*(nCodeCount - 1);

	AskData* pAskData = (AskData*)(new char[lRetLen]);
	if( pAskData == NULL )
	{
		return FALSE;
	}
	memset(pAskData, 0, lRetLen);

	pAskData->m_nType = RT_LOGIN;
	pAskData->m_nSize = nCodeCount;
	ReqLogin* pLogin  = (ReqLogin*)pAskData->m_pCode;
	pAskData->m_nOption = Login_Option_Password;
	pAskData->m_lKey = LOGIN_KEY;  // 客户端内部版本

	strncpy(pLogin->m_szPWD, "1", min(sizeof(pLogin->m_szPWD), 1));
	strncpy(pLogin->m_szUser,"", sizeof(pLogin->m_szUser));

	char* szBuf = NULL;
	UINT nLen = 0;
	if ( m_pChannels->SendRcv(m_lConnectHandle, (char*)pAskData, lRetLen,5000,szBuf,nLen) != -1 )
	{
		if(szBuf)
		{
			AnsLogin* pAnsLogin = (AnsLogin*)szBuf;
			CString strFileName = CGeneralHelper::GetAppPath() + _T("setting\\infopath.ini");
			CFile fp;
			if( fp.Open( strFileName,CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate | CFile::typeBinary ) )
			{
				fp.Write(pAnsLogin->m_szRet,pAnsLogin->m_nSize);
				fp.Close();
			}	
			m_pChannels->FreeMemory(m_lConnectHandle, szBuf);
			szBuf = NULL;
		}
		m_bIsLogined = TRUE;

		//取服务器信息
		int nAskSvrInfoLen = sizeof(AskData);
		AskData* pAskSvrInfo = (AskData*)new char[nAskSvrInfoLen];
		memset(pAskSvrInfo, 0, nAskSvrInfoLen);				
		pAskSvrInfo->m_nType = RT_SERVERINFO;
		pAskSvrInfo->m_lKey = 1/*m_pChannels->GetPacketID(this->GetConnectHandle())*/;
		if ( m_pChannels->SendRcv(this->GetConnectHandle(),(char*)pAskSvrInfo, nAskSvrInfoLen,5000,szBuf,nLen) != -1 )
		{
			if(szBuf)
			{
				AnsServerInfo* pServerInfo = (AnsServerInfo*)szBuf;
				this->SetJrName(pServerInfo->m_pName);
				m_pChannels->FreeMemory(this->GetConnectHandle(),szBuf);
			}
		}
		delete[] pAskSvrInfo;
	}
	else
	{
		m_bIsLogined = FALSE;
	}

	delete []pAskData;
	pAskData = NULL;

	if (bReConnect)
	{// 数据引擎重新初始化
		m_pDataSource->HSDataSource_ReConnectNotify(m_lConnectHandle);
	}
	return m_bIsLogined;
}

BOOL CAccount::LoginLevel2(CString &sMsg, BOOL bReConnect)
{
	if ((NULL == m_pChannelManager)||(NULL == m_pChannels))
	{
		sMsg = _T("获取连接对象失败！");
		return FALSE;
	}

	if (m_lConnectHandle == 0)
	{
		sMsg = _T("连接失败！");
		m_bConnected = FALSE;
		return FALSE;
	}

	if (bReConnect)
	{// 断线重连
		long lConnectHandle = m_pChannelManager->CreateConnector(CEV_Connect_LEVEL2, "");
		if (lConnectHandle == 0)
		{
			sMsg = _T("连接站点失败");
			m_bConnected = FALSE;
			return FALSE;
		}
		m_pChannelManager->DestroyConnect(m_lConnectHandle); // 销毁之前的连接
		m_lConnectHandle = lConnectHandle;
	}
	m_bConnected = TRUE;

	CString strPath = CGeneralHelper::GetAppPath() + ARDLL;
	HMODULE hmodule = ::LoadLibrary(strPath);
	if (hmodule)
	{
		GETPACKSERVICEINSTANCE GetPackServiceInstance = (GETPACKSERVICEINSTANCE)GetProcAddress(hmodule, "getPackServiceInstance");
		IPackService *pPackService = (IPackService*)GetPackServiceInstance(NULL);
		if(pPackService == NULL)
		{
			TRACE0("\ngetPackServiceInstance failed...\n");
			return FALSE;
		}

		IPacker *pAr2Packer = pPackService->GetPacker(0x20);
		if(pAr2Packer == NULL)
		{
			TRACE0("GetPacker failed...\n");
			return FALSE;
		}

		pAr2Packer->BeginPack();
		pAr2Packer->AddField("c_login_type");
		pAr2Packer->AddField("vc_user");
		pAr2Packer->AddField("vc_user_pw");

		pAr2Packer->AddValue("1");
		pAr2Packer->AddValue(this->m_sLoginAccount);
		pAr2Packer->AddValue(this->m_sPassword);

		pAr2Packer->EndPack();

		unsigned int nArPackLen = pAr2Packer->PackLen();
		char* arDataBuffer = (char*)pAr2Packer->PackBuf();	
		//
		long lRetLen = sizeof(AskData) - sizeof(CodeInfo) + nArPackLen;
		AskData* pAskData = (AskData*)(new char[lRetLen]);
		memset(pAskData, 0, lRetLen);

		pAskData->m_nIndex = m_pChannels->GetPacketID(this->GetConnectHandle());
		pAskData->m_nType = RT_LOGIN_LEVEL;
		pAskData->m_nSize = nArPackLen;
		pAskData->m_nOption = Login_Option_Password;
		pAskData->m_lKey = LEVEL2LOGIN_KEY/*SendClientVerInfoToSrv*/;  // 客户端内部版本
		//copy AR2数据包
		memcpy(pAskData->m_pCode,arDataBuffer,nArPackLen);
		pPackService->FreePacker( pAr2Packer);
		char *pRcv;
		UINT nRcv;
		int timeout = 5000;
		if(m_pChannels->SendRcv(m_lConnectHandle, (char*)pAskData,lRetLen,timeout,pRcv,nRcv) != -1)
		{
			BOOL bLogin(TRUE);
			if (pRcv)
			{
				DataHead* pHead = (DataHead*)pRcv;
				AnsLogin* pAnsLogin = (AnsLogin*)pHead;
				if (pHead->m_nType == RT_DISCONNLEVEL2)
				{
					unsigned int iLen = pAnsLogin->m_nSize;
					IUnPacker *pUnPacker = NULL;
					if( iLen <= 1000 ) //应答包过长视为错误包
						pUnPacker = pPackService->GetUnPacker(pAnsLogin->m_szRet, iLen);
					else
						pUnPacker = NULL;
					
					if (pUnPacker)
					{
						int nOpenRet =pUnPacker->Open(pAnsLogin->m_szRet, iLen);
						while (!pUnPacker->isEOF())
						{
							int nColIndex = pUnPacker->findColIndex("error_info");
							if(  nColIndex >= 0 )
							{
								int nErrorNO = pUnPacker->getInt("error_no");
								CString strErrorInfo = pUnPacker->getString("error_info");
								if(nErrorNO != 0 || strErrorInfo != "" )
								{
									CString strTemp;
									strTemp.Format("[level2登录失败]错误号:%d，错误信息:%s",nErrorNO,strErrorInfo.GetBuffer());
									sMsg = strTemp;
								}
							}
							pUnPacker->next();
							continue;
						}
						pPackService->FreeUnPacker(pUnPacker);
					}
					bLogin = FALSE;
					m_pChannels->FreeMemory(m_lConnectHandle, pRcv);
					delete[] pAskData;
				}
				else
				{
					m_pChannels->FreeMemory(m_lConnectHandle, pRcv);
					delete[] pAskData;

					//取服务器信息
					int nAskSvrInfoLen = sizeof(AskData);
					AskData* pAskSvrInfo = (AskData*)new char[nAskSvrInfoLen];
					memset(pAskSvrInfo, 0, nAskSvrInfoLen);				
					pAskSvrInfo->m_nType = RT_SERVERINFO;
					pAskSvrInfo->m_lKey = 1/*m_pChannels->GetPacketID(this->GetConnectHandle())*/;
					sMsg = _T("Level2获取服务器信息错误");
					if ( m_pChannels->SendRcv(m_lConnectHandle,(char*)pAskSvrInfo, nAskSvrInfoLen,timeout,pRcv,nRcv) != -1 )
					{
						if(pRcv)
						{
							AnsServerInfo* pServerInfo = (AnsServerInfo*)pRcv;
							this->SetJrName(pServerInfo->m_pName);
							m_pChannels->FreeMemory(m_lConnectHandle, pRcv);
							sMsg = _T("Level2获取服务器信息成功");
						}
					}
					delete[] pAskSvrInfo;
				}
			}
			m_bIsLogined = bLogin;
			return bLogin;
		}
		sMsg = _T("Level2连接错误");
		pPackService->Release();
		return FALSE;
	}	
	sMsg = _T("Level2打登录包失败");
	return FALSE;
}


BOOL CAccount::LoginFuture(CString &sMsg,  BOOL bReConnect)
{
	if ((NULL == m_pChannelManager)||(NULL == m_pChannels))
	{
		sMsg = _T("获取连接对象失败！");
		return FALSE;
	}

	if (m_lConnectHandle == 0)
	{
		sMsg = _T("连接失败！");
		m_bConnected = FALSE;
		return FALSE;
	}

	if (bReConnect)
	{// 断线重连
		long lConnectHandle = m_pChannelManager->CreateConnector(CEV_Connect_QH, "");
		if (lConnectHandle == 0)
		{
			sMsg = _T("连接站点失败");
			m_bConnected = FALSE;
			return FALSE;
		}
		m_pChannelManager->DestroyConnect(m_lConnectHandle); // 销毁之前的连接
		m_lConnectHandle = lConnectHandle;
	}
	m_bConnected = TRUE;

	int nCodeCount = sizeof(ReqLogin) / sizeof(CodeInfo) + ((sizeof(ReqLogin) % sizeof(CodeInfo))?1:0);
	int lRetLen    = sizeof(AskData) + sizeof(CodeInfo)*(nCodeCount - 1);

	AskData* pAskData = (AskData*)(new char[lRetLen]);
	if( pAskData == NULL )
	{
		return FALSE;
	}
	memset(pAskData, 0, lRetLen);

	pAskData->m_nType = RT_LOGIN_FUTURES;
	pAskData->m_nSize = nCodeCount;
	ReqLogin* pLogin  = (ReqLogin*)pAskData->m_pCode;
	pAskData->m_nOption = Login_Option_NotCheck;
	pAskData->m_lKey = LOGIN_KEY;  // 客户端内部版本

	strncpy(pLogin->m_szPWD, "1", min(sizeof(pLogin->m_szPWD), 1));
	strncpy(pLogin->m_szUser,"", sizeof(pLogin->m_szUser));

	char* szBuf = NULL;
	UINT nLen = 0;
	if ( m_pChannels->SendRcv(m_lConnectHandle, (char*)pAskData, lRetLen,5000,szBuf,nLen) != -1 )
	{
		if(szBuf)
		{
// 			AnsLogin* pAnsLogin = (AnsLogin*)szBuf;
// 			CString strFileName = CGeneralHelper::GetAppPath() + _T("setting\\infopath.ini");
// 			CFile fp;
// 			if( fp.Open( strFileName,CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate | CFile::typeBinary ) )
// 			{
// 				fp.Write(pAnsLogin->m_szRet,pAnsLogin->m_nSize);
// 				fp.Close();
// 			}	
			m_pChannels->FreeMemory(m_lConnectHandle, szBuf);
			szBuf = NULL;
		}
		m_bIsLogined = TRUE;

		//取服务器信息
		int nAskSvrInfoLen = sizeof(AskData);
		AskData* pAskSvrInfo = (AskData*)new char[nAskSvrInfoLen];
		memset(pAskSvrInfo, 0, nAskSvrInfoLen);				
		pAskSvrInfo->m_nType = RT_SERVERINFO;
		pAskSvrInfo->m_lKey = 1/*m_pChannels->GetPacketID(this->GetConnectHandle())*/;
		if ( m_pChannels->SendRcv(this->GetConnectHandle(),(char*)pAskSvrInfo, nAskSvrInfoLen,5000,szBuf,nLen) != -1 )
		{
			if(szBuf)
			{
				AnsServerInfo* pServerInfo = (AnsServerInfo*)szBuf;
				this->SetJrName(pServerInfo->m_pName);
				m_pChannels->FreeMemory(this->GetConnectHandle(),szBuf);
			}
		}
		delete[] pAskSvrInfo;
	}
	else
	{
		m_bIsLogined = FALSE;
	}

	delete []pAskData;
	pAskData = NULL;

	if (bReConnect)
	{// 数据引擎重新初始化
			m_pDataSource->HSDataSource_ReConnectNotify(m_lConnectHandle);
	}
	return m_bIsLogined;
}


long CAccount::GetConnectHandle()
{
	return m_lConnectHandle;
}

void CAccount::SetConnectHandle(const long lValue)
{
	m_lConnectHandle = lValue;
	m_bConnected = m_lConnectHandle?TRUE:FALSE;
}

BOOL CAccount::IsConnected()
{
	return m_bConnected;
}

CString CAccount::GetJrName()
{
	return m_sJrName;
}

void CAccount::SetJrName(const CString Value)
{
	m_sJrName = Value;
}

//ITradeMarketList* CAccount::GetTradeMarketList()
//{
//	return m_pTradeConfigObj->GetTradeMarketList(m_CellNo, m_sAccountName);
//}

ITradeConfigObj* CAccount::GetTradeConfigObj()
{
	return m_pTradeConfigObj;
}

void CAccount::AddTradingAsync(ITrading* pTrading)
{
	m_TradingAsyncList.Add(pTrading);
}

void CAccount::RemoveTradingAsync(ITrading* pTrading)
{
	for (int i = 0; i < m_TradingAsyncList.GetCount(); i++)
	{
		ITrading* pAsync = m_TradingAsyncList.GetAt(i);
		if (pAsync == pTrading)
		{
			m_TradingAsyncList.RemoveAt(i);
			break;
		}
	}
}

EAccountType CAccount::GetAccountType()
{
	return m_nAccountType;
}

void CAccount::SetAccountType(EAccountType nType)
{
	m_nAccountType = nType;
}

BOOL CAccount::AutoLogin()
{
	CString sMsg;
	m_nAutoLoginNum++;
	if (m_nAutoLoginNum > 5)//需要读取配置
	{
		return FALSE;
	}	
	BOOL bReturn = Login(sMsg);
	m_bIsLogined = TRUE;
	return bReturn;
}

BOOL CAccount::AutoQuoteLogin()
{
	CString sMsg;
// 	m_nAutoLoginNum++;
// 	if (m_nAutoLoginNum > 5) 
// 	{
// 		return FALSE;
// 	}
	BOOL bReturn = Login(sMsg, TRUE);
	m_bIsLogined = TRUE;
	return bReturn;
}

ICCommParamList* CAccount::GetAccountParamList()
{
	if (!m_pParamList)
	{
		m_pParamList = m_pTradeConfigObj->GetAccountParamList(m_CellNo, m_sAccountName);
	}
	return m_pParamList;
}

//////////////////////////////////////////////////////////////////////////


CAccountMng::CAccountMng()
{
	m_iMaxID = 0;
	//m_pCurrentAccount = NULL;
	m_pfnOnConnectNotify = sFn_OnConnectNotify;
	m_pChannelManager = (IChannelManager*)CWinnerApplication::GetObject(OBJ_CHANNELMANAGER);
	if(m_pChannelManager)
		m_pChannelManager->RegisterNotify(m_pfnOnConnectNotify);
}

CAccountMng::~CAccountMng()
{
	if (m_pChannelManager)
	{
		m_pChannelManager->UnregisterNotify(m_pfnOnConnectNotify);
	}
		
}

void CAccountMng::Release()
{
	delete this;
}

void CAccountMng::FreeCacheAll()
{	
	POSITION ipos = m_AccountTypeMap.GetStartPosition();
	while(ipos)
	{
		WORD nKey = 0;
		CAccountList* tmpList = NULL;
		m_AccountTypeMap.GetNextAssoc(ipos, nKey, tmpList);
		if (tmpList)
		{
			for (int i = 0; i < (tmpList->GetCount()); ++i)
			{
				IAccount* pAccount = tmpList->GetAt(i);
				pAccount->FreeCache();
				pAccount->Release();
				pAccount = NULL;
			}
			tmpList->RemoveAll();
			delete tmpList;
		}
	}
	m_AccountTypeMap.RemoveAll();	
}

void CAccountMng::FreeCache(EAccountType nType)
{
	CAccountList* tmpList = NULL;
	//m_AccountTypeMap.GetNextAssoc(ipos, nKey, tmpList);
	m_AccountTypeMap.Lookup((WORD)nType, tmpList);
	if (tmpList)
	{
		for (int i = 0; i < (tmpList->GetCount()); ++i)
		{
			IAccount* pAccount = tmpList->GetAt(i);
			pAccount->FreeCache();
			pAccount->Release();
			pAccount = NULL;
		}
		tmpList->RemoveAll();
		delete tmpList;
	}
	m_AccountTypeMap.RemoveKey((WORD)nType);
	m_CurrentAccountMap.RemoveKey((WORD)nType);
	CallAccountChange(NULL, acChange);
}



IAccount* CAccountMng::GetAccountByID(const int iID, EAccountType nType)
{	
	CAccountList* tmpList = NULL;
	m_AccountTypeMap.Lookup(WORD(nType), tmpList);
	IAccount* pTmp = NULL;
	if (tmpList)
	{
		for(int i = 0; i < tmpList->GetCount(); ++i)
		{
			IAccount* pAccount = tmpList->GetAt(i);
			if (pAccount->GetID() == iID )
			{
				pTmp = pAccount;
				break;
			}
		}
	}	
	return pTmp;
}
IAccount* CAccountMng::GetAccount(const int iIndex, EAccountType nType)
{
	CAccountList* tmpList = NULL;
	m_AccountTypeMap.Lookup(WORD(nType), tmpList);
	if (tmpList)
	{
		if (iIndex >= tmpList->GetCount())
			return NULL;
		else
		{
			IAccount* pAccount = tmpList->GetAt(iIndex) ;	 
			return pAccount;
		}
	}
	else
		return NULL;
}

IAccount* CAccountMng::NewAccount(EAccountType nType)
{
	CAccount* pAccount = new CAccount;
	m_iMaxID += 1;
	pAccount->m_iAccountID = m_iMaxID;
	pAccount->SetAccountType(nType);
	return pAccount;
}

void CAccountMng::Add(IAccount * lpValue)
{
	CAccountList* tmpList = NULL;
	WORD nType = (WORD)lpValue->GetAccountType();
	m_AccountTypeMap.Lookup(nType, tmpList);
	if (!tmpList)
	{
		tmpList = new CAccountList;
		m_AccountTypeMap.SetAt(nType, tmpList);
	}
	tmpList->Add(lpValue);
	CallAccountChange(lpValue, acAdd);
}
int CAccountMng::GetCount(EAccountType nType)
{
	CAccountList* tmpList = NULL;
	m_AccountTypeMap.Lookup(WORD(nType), tmpList);
	if (tmpList)
	{
		return tmpList->GetCount();
	}
	else
		return 0;
}

void CAccountMng::DeleteAccount(IAccount * lpValue)
{
	CallAccountChange(lpValue, acDel);
	CAccountList* tmpList = NULL;
	EAccountType nType = lpValue->GetAccountType();
	m_AccountTypeMap.Lookup(WORD(nType), tmpList);
	if (tmpList)
	{
		for(int i = 0; i < tmpList->GetCount(); ++i)
		{
			IAccount* pAccount = tmpList->GetAt(i) ;
			if (lpValue == pAccount)
			{
				tmpList->RemoveAt(i);
				pAccount->FreeCache();
				pAccount->Release();
				pAccount = NULL;
				lpValue = NULL;
				break;
			}
		}  
		if (tmpList->GetCount() > 0)
		{
			SetCurrentAccount(tmpList->GetAt(0));
		}
		else
		{
			m_CurrentAccountMap.RemoveKey((WORD)nType);
			CallAccountChange(NULL, acChange);
		}
	}
	
}
//获取当前账号
IAccount* CAccountMng::GetCurrentAccount(EAccountType nType)
{
	IAccount* pAccount = NULL;
	m_CurrentAccountMap.Lookup(WORD(nType), pAccount);	
	return pAccount;
}
//设置当前账号
void CAccountMng::SetCurrentAccount(IAccount * lpValue)
{
	//m_pCurrentAccount = lpValue;
	if (lpValue)
	{
		IAccount* pAccount = GetCurrentAccount(lpValue->GetAccountType());
		if (pAccount != lpValue)
		{
			m_CurrentAccountMap.SetAt(WORD(lpValue->GetAccountType()), lpValue);
			CallAccountChange(lpValue, acChange);
		}		
	}
}

BOOL CAccountMng::OnNetConnectNotify(ConnectNotify* pNotify)
{
	long lConnectHandle = 0;

	if(pNotify != NULL)
	{
		POSITION ipos = m_AccountTypeMap.GetStartPosition();
		while(ipos)
		{
			WORD nKey = 0;
			CAccountList* tmpList = NULL;
			m_AccountTypeMap.GetNextAssoc(ipos, nKey, tmpList);
			if (tmpList)
			{
				for (int i = 0; i < tmpList->GetCount(); i++)
				{
					IAccount* pAccount = tmpList->GetAt(i);
					if (pAccount->GetIsLogined())
					{
						if (pAccount->GetConnectHandle() == pNotify->m_lHandle)
						{
							if(pNotify->m_result == Drop)//断线，自动重连
							{
								if(pAccount->GetIsLogined())//必须是已经登陆过的帐户
								{
									BOOL bReturn = FALSE;
									switch (pAccount->GetAccountType())
									{
									case atTrade:
										{		
											pAccount->SetConnectHandle(0);
											bReturn = pAccount->AutoLogin();
											break;
										}
									case atLevel1:
									case atLevel2:
									case atFuture:
										{
											bReturn = pAccount->AutoQuoteLogin();
											break;
										}
									default: bReturn = FALSE;
									}

									//if (!bReturn)//连接不成功则通知
									{
										//CallAccountStatus(pAccount);
										pTradeCoreObj->AccountStatus(pAccount);
									}				
								}
								
							}
							else
								pTradeCoreObj->AccountStatus(pAccount);								
							
						}
					}				
				}
			}
		}
	}
	return TRUE;
}

void CAccountMng::RegAccountChange(IAccountChange* lpValue)
{
	m_AccountChangeList.Add(lpValue);
}

void CAccountMng::UnRegAccountChange(IAccountChange* lpValue)
{
	for (int i = 0; i < m_AccountChangeList.GetCount(); i++)
	{
		if (lpValue == m_AccountChangeList.GetAt(i))
		{
			m_AccountChangeList.RemoveAt(i);
			break;
		}		 
	}	
}

void CAccountMng::CallAccountChange(IAccount* lpValue, EAccountChange nChangeType)
{
	for (int i = 0; i < m_AccountChangeList.GetCount(); i++)
	{
		m_AccountChangeList.GetAt(i)->OnAccountChange(lpValue, nChangeType);
	}	
}

void CAccountMng::CallAccountStatus(IAccount* lpValue)
{
	for (int i = 0; i < m_AccountChangeList.GetCount(); i++)
	{
		m_AccountChangeList.GetAt(i)->OnAccountStatus(lpValue);
	}	
}

int CAccountMng::GetCurrentAccountCount()
{
	return m_CurrentAccountMap.GetCount();
}

IAccount* CAccountMng::GetCurrentAccountByIndex(int iIndex)
{
	if (iIndex > (m_CurrentAccountMap.GetCount() - 1))
    {
		return NULL;
    }
	else
	{
		WORD nKey = 0;
		IAccount* pAccount = NULL;
		int i = 0;
		POSITION ipos = m_CurrentAccountMap.GetStartPosition();
		while(ipos)
		{
			m_CurrentAccountMap.GetNextAssoc(ipos, nKey, pAccount);
			if(i == iIndex)
			{
				break;
			}
			else
				i++;
		}
		return pAccount;
	}
}

IAccount* CAccountMng::GetAccountByNameAndCellNo(CString sAccountName, CString sCellNo, EAccountType nType)
{
	for (int i = 0; i < GetCount(nType); i++)
	{
		IAccount* pTmp = GetAccount(i, nType);
		if ((pTmp->GetAccountName() == sAccountName) && (pTmp->GetCellNo() == sCellNo))
		{
			return pTmp;
		}
	}
	return NULL;
}