//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AutoRequest.h"
#include "DataSourceImpl.h"
#include "DSWriteFile.h"
#include "ConfigManager.h"
#include "GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
CAutoRequest::CAutoRequest()
{
	m_pDataSourceImpl = NULL;
	m_pConfigManager = NULL;
}

CAutoRequest::~CAutoRequest()
{

}

//////////////////////////////////////////////////////////////////////////
void CAutoRequest::SetDataSourceImpl(CDataSourceImpl* pImpl)
{
	m_pDataSourceImpl = pImpl;
}

CConfigManager*	CAutoRequest::GetConfigManager()
{
	if (m_pConfigManager == NULL)
	{
		if (m_pDataSourceImpl != NULL)
		{
			m_pConfigManager = m_pDataSourceImpl->GetConfigManager();
		}
	}
	return m_pConfigManager;
}

BOOL CAutoRequest::OnLoginSuccess(Server_type svrType)
{
	//RequestConfFile
	if (svrType == CEV_Connect_HQ)
	{
		if (m_pDataSourceImpl != NULL)
		{
			CDSWriteFile* pDsWriteFile = m_pDataSourceImpl->GetDSWriterFile();
			if (pDsWriteFile == NULL)
			{
				return FALSE;
			}
			CString strSettingFilePath = pDsWriteFile->GetSettingFilePath();
			if(strSettingFilePath.IsEmpty())
			{
				return FALSE;
			}

//			RequestConfFile(strSettingFilePath + DYN_NEWS_INI_FILE);
// 			RequestIncrementData(RT_EXRIGHT_DATA);
// 			RequestIncrementData(RT_CURRENTFINANCEDATA);
// 			RequestIncrementData(RT_BLOCK_DATA);
// 			RequestIncrementData(RT_USER_BLOCK_DATA);
		}
	}
	return TRUE;
}

BOOL CAutoRequest::ReqServerInitInfo(Server_type svrType)
{
	IHsQuoteSys* pQSysConf = (IHsQuoteSys*)CWinnerApplication::GetObject(OBJ_QUOTESYS);
	CString strMarket;
	CStringArray ayRet;
	if( svrType == CEV_Connect_HQ )
	{
		strMarket = pQSysConf->GetQuoteServerInitMarket();
		CGeneralHelper::AnalyseStr(strMarket,"|",ayRet);
	}
	else if(svrType == CEV_Connect_QH)
	{
		strMarket = pQSysConf->GetFutureServerInitMarket();
		CGeneralHelper::AnalyseStr(strMarket,"|",ayRet);
	}

	CArray<int,int> ayInitMarket;
	for (int i = 0 ; i < ayRet.GetCount(); i++)
	{
		CString strTemp = ayRet.GetAt(i);
		int nMarket = 0;
		sscanf(strTemp,"%x",&nMarket);
		if(nMarket > 0)
			ayInitMarket.Add(nMarket);
	}

	int nSize = ayInitMarket.GetCount();
	int nServer = sizeof(ReqInitSrv) + nSize * sizeof(ServerCompare);
	nServer = nServer / sizeof(CodeInfo) + ((nServer % sizeof(CodeInfo))?1:0);

	int lRetLen = sizeof(AskData) - sizeof(CodeInfo);
	lRetLen += nServer * sizeof(CodeInfo);

	char* pBuffer = new char[lRetLen];
	AskData* pakData = (AskData*)pBuffer;
	memset(pakData, 0, lRetLen);					
	pakData->m_nType  = RT_INITIALINFO;
	pakData->m_nIndex = 0;
	pakData->m_nSize  = nServer;
	pakData->m_lKey   = (long)this;
	pakData->m_cOperator = svrType;

	// 填充代码验证
	ReqInitSrv* pReqInitSrv = (ReqInitSrv*)pakData->m_pCode;
	pReqInitSrv->m_nAlignment = 0;
	ServerCompare* pServerCompare  = pReqInitSrv->m_sServerCompare;
	pReqInitSrv->m_nSrvCompareSize = 0;

	for( unsigned int i = 0; i < ayInitMarket.GetCount(); i++ )
	{
		pServerCompare[i].m_cBourse = ayInitMarket.GetAt(i);
		pServerCompare[i].m_dwCRC = 0;
		pReqInitSrv->m_nSrvCompareSize++;
	}

	if (m_pDataSourceImpl != NULL)
	{
		m_pDataSourceImpl->HSDataSource_RequestAsyncData(TEMP_DATASOURCE_ID, (char*)pakData, lRetLen, 0);
	}

	delete[] pBuffer;
	pakData = NULL;
	return TRUE;
}

BOOL CAutoRequest::RequestConfFile(CString strFileName, BOOL bForceUpdate)
{
	AskData* pakData = NULL;

	int nCodeCount = sizeof(ReqTextData) / sizeof(CodeInfo) + 1;
	int nAskLen = sizeof(AskData) + nCodeCount * sizeof(CodeInfo);

	pakData = (AskData*)new char[nAskLen];
	memset(pakData, 0, nAskLen);

	pakData->m_nIndex = 0;
	pakData->m_nSize  = nCodeCount;
	pakData->m_nType  = RT_FILEREQUEST;
	pakData->m_lKey   = 0;

	ReqTextData* pDataReq = (ReqTextData*)pakData->m_pCode;

	// 读取配置
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath( strFileName, drive, dir, fname, ext );
	CString strPath = drive;
	strPath += dir;

	strFileName  = fname;
	strFileName += ext; 

	strncpy(pDataReq->m_sMarkData.m_cFilePath,strFileName,
		sizeof(pDataReq->m_sMarkData.m_cFilePath));

	if( bForceUpdate == 0 )
	{
		pDataReq->m_sMarkData.m_lCheckCRC = 1;
	}

	if (m_pDataSourceImpl != NULL)
	{
		m_pDataSourceImpl->HSDataSource_RequestAsyncData(TEMP_DATASOURCE_ID, (char*)pakData, nAskLen, 0);
	}	
	delete[] pakData;
	pakData = NULL;

	return TRUE;
}

BOOL CAutoRequest::RequestIncrementData(long lType)
{
	int nSize = sizeof(ReqCurrentFinanceData) / sizeof(CodeInfo);
	if( sizeof(ReqCurrentFinanceData) % sizeof(CodeInfo) )
	{
		nSize++;
	}

	int nAskLen = sizeof(AskData) + sizeof(CodeInfo) * (nSize - 1); 
	AskData* pskData = (AskData*) new char[nAskLen];
	memset(pskData, 0, nAskLen);

	pskData->m_nType  = (unsigned short)lType;
	pskData->m_nIndex = 0;
	pskData->m_nSize  = nSize;
	pskData->m_cOperator = 0; 

	ReqCurrentFinanceData* pData = (ReqCurrentFinanceData*)pskData->m_pCode;

	CConfigManager* pManager = GetConfigManager();
	if (pManager != NULL)
	{
		pData->m_lLastDate = pManager->GetInitDateByType(GetSectionName(ConfigType_byRequest, lType));
	}

	if (m_pDataSourceImpl != NULL)
	{
		m_pDataSourceImpl->HSDataSource_RequestAsyncData(TEMP_DATASOURCE_ID, (char*)pskData, nAskLen, 0);
	}	
	delete[] pskData;
	pskData = NULL;

	return FALSE;
}

BOOL CAutoRequest::AutoRequestServerCalc(char cSvrType)
{
	if (m_pDataSourceImpl != NULL)
	{
		CStockManager* pManager = m_pDataSourceImpl->GetStockManager();
		if (pManager != NULL)
		{
			CArray<CodeInfo*> arCodeInfo;
			int nCodeCount = pManager->GetStockCodeInfoList(arCodeInfo);
		
			int lRetLen    = sizeof(AskData) + sizeof(CodeInfo)*(nCodeCount - 1);
			AskData* pAskData = (AskData*)(new char[lRetLen]);
			if( pAskData == NULL )
			{
				return FALSE;
			}
			memset(pAskData, 0, lRetLen);
			pAskData->m_nType  = RT_SEVER_CALCULATE;
			pAskData->m_nIndex = 0;
			pAskData->m_nSize  = nCodeCount;
			pAskData->m_cOperator = cSvrType; 
			CodeInfo* pcInfo = pAskData->m_pCode;
			for (int i = 0; i < nCodeCount; i ++)
			{
				CodeInfo* pCodeCopy = arCodeInfo.GetAt(i);
				if (pCodeCopy != NULL)
				{
					memcpy(&pcInfo[i], pCodeCopy, sizeof(CodeInfo));
				}
			}
			if (m_pDataSourceImpl != NULL)
			{
				m_pDataSourceImpl->HSDataSource_RequestAsyncData(TEMP_DATASOURCE_ID, (char*)pAskData, lRetLen, 0);
			}

			delete[] pAskData;
			pAskData = NULL;
			arCodeInfo.RemoveAll();
			return TRUE;
		}
	}
	return FALSE;
}

CString	CAutoRequest::GetStatus(DWORD dwStatus)
{
	CString strRet = _T("");
	switch(dwStatus)
	{
	case InitStatus_login:
		strRet = _T("服务器校验成功...");
		break;
	case InitStatus_init:
		strRet = _T("开始数据初始化...");
		break;
	case InitStatus_exright:
		strRet = _T("处理除权数据...");
		break;
	case InitStatus_config:
		strRet = _T("下载服务器配置...");
		break;
	case InitStatus_currentfinancedata:
		strRet = _T("处理财务数据...");
		break;
	case InitStatus_blockdata:
		{
			strRet = _T("处理系统板块数据...");
			break;
		}
	case InitStatus_userblockdata:
		strRet = _T("处理用户板块数据...");
		break;
	case InitStatus_servercalc:
		strRet = _T("计算股票涨跌停");
		break;
	case InitStatus_succ:
		//strRet = _T("完成初始化");
	default:
		break;
	}
	CConfigManager* pManager = GetConfigManager();
	if (pManager != NULL)
	{
		CTime tm = CTime::GetCurrentTime();
		CString str = tm.Format("%Y%m%d");
		pManager->SetDownloadDate(GetSectionName(ConfigType_bystatus, dwStatus), str);
	}


	return strRet;
}

CString	CAutoRequest::GetSectionName(int nType, DWORD dwStatus)
{
	CString strRet = _T("");
	if (nType == ConfigType_bystatus)
	{
		switch(dwStatus)
		{
		case InitStatus_exright:
			strRet = _T("ExRightDate");
			break;
		case InitStatus_currentfinancedata:
			strRet = _T("CurrentFinanceDate");
			break;
		case InitStatus_blockdata:
			{
				strRet = _T("SysBlockDate");
				break;
			}
		case InitStatus_userblockdata:
			strRet = _T("UserBlockDate");
			break;
		default:
			break;
		}
	}
	else if (ConfigType_byRequest == nType)
	{
		switch(dwStatus)
		{
		case RT_EXRIGHT_DATA:
			strRet = _T("ExRightDate");
			break;
		case RT_CURRENTFINANCEDATA:
			strRet = _T("CurrentFinanceDate");
			break;
		case RT_BLOCK_DATA:
			{
				strRet = _T("SysBlockDate");
				break;
			}
		case RT_USER_BLOCK_DATA:
			strRet = _T("UserBlockDate");
			break;
		default:
			break;
		}
	}
	return strRet;

}

BOOL CAutoRequest::ReqServerInfo()
{
	BOOL bRet = FALSE;			


	int lRetLen = sizeof(AskData);
	AskData* pskData = (AskData*)new char[lRetLen];
	memset(pskData, 0, lRetLen);				
	pskData->m_nType = RT_SERVERINFO;
//	pskData->m_cOperator = CEV_Connect_HQ_;
	
	if (m_pDataSourceImpl != NULL)
	{
		m_pDataSourceImpl->HSDataSource_RequestAsyncData(TEMP_DATASOURCE_ID, (char*)pskData, lRetLen, 0);
	}	
	delete[] pskData;
	pskData = NULL;

	return bRet;
}