//////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "DSWriteFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////
CDSWriteFile::CDSWriteFile()
{
	m_strSettingPath = _T("");
	m_hZipFileDll = NULL;
}

CDSWriteFile::~CDSWriteFile()
{
	if (m_hZipFileDll != NULL)
	{
		FreeLibrary(m_hZipFileDll);
		m_hZipFileDll = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDSWriteFile::InitWriteFileEnv()
{
	TCHAR tszFilePath[_MAX_PATH] = {0};
	DWORD dwSize = ::GetModuleFileName(NULL, tszFilePath, _MAX_PATH);

	CString strExeFilePath = tszFilePath;
	if (dwSize == _MAX_PATH)
	{
		TCHAR tszFilePathEx[1024] = {0};
		dwSize = ::GetModuleFileName(NULL, tszFilePath, 1024);
		strExeFilePath = tszFilePathEx;
	}
	CString strRoot = strExeFilePath.Left(strExeFilePath.ReverseFind('\\'));
	m_strSettingPath = strRoot + _T("\\Setting\\");
	CreateDirectory(m_strSettingPath, NULL);
	m_strBlockPath = strRoot + _T("\\Block\\");
	CreateDirectory(m_strBlockPath, NULL);
	m_strInitPath = strRoot + _T("\\InitDat\\");
	CreateDirectory(m_strInitPath, NULL);

	m_hZipFileDll = LoadLibrary(strRoot + "\\zipFileDll.dll");
	if (m_hZipFileDll != NULL)
	{
		m_pfnUnZipFile = (PFN_UNZIPFILE)GetProcAddress(m_hZipFileDll, "UnZipFile");
	}
	return FALSE;
}

CString	CDSWriteFile::GetSettingFilePath()
{
	return m_strSettingPath;
}

BOOL CDSWriteFile::WriteAnsLoginData(AnsLogin* pAnsLogin, CString& pHtmlPage, CStringArray& pAy)
{
	if( pAnsLogin == NULL || pAnsLogin->m_nError < 0 )
		return FALSE;
	if( pAnsLogin->m_nSize <= 0 )
	{
		return TRUE;
	}
	/* 是否为行情， 否则为资讯 */
	BOOL IsHQSrv = (pAnsLogin->m_dhHead.m_nType == RT_LOGIN);
	if(IsHQSrv)
	{
	
	}

	CString strTreeFileName;
	strTreeFileName  = m_strSettingPath;
	if(IsHQSrv)
	{
		strTreeFileName += INFO_TREE_INI_FILE;
	}
	else
	{
		strTreeFileName += BASE_INFO_TREE_INI_FILE;
	}
	CString strPathFileName;
	strPathFileName  = m_strSettingPath;
	if(IsHQSrv)
	{
		strPathFileName += INFO_PATH_INI_FILE;
	}
	else
	{
		strPathFileName += SECRET_TREE_INI_FILE;
	}

	try
	{
		CFile::Remove(strTreeFileName);
	}
	catch (CFileException* pEx)
	{
		TRACE(_T("File %20s cannot be removed\n"), strTreeFileName);
		pEx->Delete();
	}

	try
	{
		CFile::Remove(strPathFileName);
	}
	catch (CFileException* pEx)
	{
		TRACE(_T("File %20s cannot be removed\n"), strTreeFileName);
		pEx->Delete();
	}

	//配置文件，由配置管理接管
	return FALSE;
}

BOOL CDSWriteFile::WriteTempFile(CString strFileName, char* pszData, int nSize)
{
	if (!strFileName.IsEmpty())
	{
		CFile fp;
		BOOL bOpen = fp.Open( strFileName,
			CFile::typeBinary | CFile::shareDenyNone | 
			CFile::modeCreate | CFile::modeReadWrite );
		if( !bOpen ) 
		{
			return FALSE;
		}

		fp.Write(pszData, nSize);
		fp.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL CDSWriteFile::WriteAnsSysBlockData(char* pszData, int nSize)
{
	CString strTempBlockFile = m_strSettingPath + SYS_BLOCK_FILE;
	if(WriteTempFile(strTempBlockFile, pszData, nSize)) 
	{
		if (m_pfnUnZipFile != NULL && nSize > 0)
		{
			if(m_pfnUnZipFile(strTempBlockFile, m_strBlockPath) > 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDSWriteFile::WriteAnsFinanceData(char* pszData, int nSize)
{
	CString strTempBlockFile = m_strSettingPath + CURRENT_FINANCE_FILE;
	if(WriteTempFile(strTempBlockFile, pszData, nSize)) 
	{

	}
	return FALSE;
}

BOOL CDSWriteFile::WriteAnsExrightData(char* pszData, int nSize)
{
	CString strTempBlockFile = m_strSettingPath + EX_RIGHT_FILE;
	if(WriteTempFile(strTempBlockFile, pszData, nSize)) 
	{

	}
	return FALSE;
}

BOOL CDSWriteFile::WriteComBouseData(BourseInfo* pInfo)
{
	if (pInfo != NULL)
	{
		CString strFileName;
		strFileName.Format(_T("%s%i.int"), m_strInitPath, pInfo->m_nMarketType);
		CFile fileBouse;
		if (fileBouse.Open(strFileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate | CFile::typeBinary))
		{
			int	nVersion = DSVERSION_CODELIST;
			fileBouse.Write(&nVersion, sizeof(nVersion));
			fileBouse.Write(pInfo, sizeof(BourseInfo));
			fileBouse.Close();
		}
	}
	return FALSE;
}

BOOL CDSWriteFile::ReadComBouseData(CHsBouseManager* pManager)
{
	if (pManager == NULL)
	{
		return FALSE;
	}
	CString strFilePath;
	strFilePath.Format(_T("%s*.int"), m_strInitPath);

	BOOL bRet = FALSE;
	CFileFind find;
	BOOL bContinue = find.FindFile(strFilePath);
	while(bContinue)
	{
		bContinue   = find.FindNextFile();
		CString strFileName = find.GetFileTitle();

		if( find.IsDots() || find.IsDirectory() )
			continue;
	
		HSMarketDataType cBourse = atol(strFileName);
		if( cBourse == 0 )
			continue;

		CString strCurFilePath = find.GetFilePath();
		CFile fileBouse;
		if(fileBouse.Open(strCurFilePath, CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary))
		{
			int nVersion = 0;
			int nReadSize = fileBouse.Read(&nVersion, sizeof(int));
			if (nReadSize == sizeof(int) && nVersion == DSVERSION_CODELIST)
			{
				BourseInfo* pBouseInfoNew = new BourseInfo;
				memset(pBouseInfoNew, 0, sizeof(BourseInfo));

				nReadSize = fileBouse.Read(pBouseInfoNew, sizeof(BourseInfo));
				if (nReadSize == sizeof(BourseInfo))
				{
					pManager->PushBourse(pBouseInfoNew);
				}
			}
			fileBouse.Close();
		}
		bRet = TRUE;
	}
	return bRet;
}

BOOL CDSWriteFile::StartWriteStockData()
{
	if (m_fileStock.m_hFile == INVALID_HANDLE_VALUE)
	{
		CString strFileName;
		strFileName.Format(_T("%sstocks.ext"), m_strInitPath);
		BOOL bOpen = m_fileStock.Open(strFileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate | CFile::typeBinary);
		if (bOpen)
		{
			int nVersion = DSVERSION_CODELIST;
			m_fileStock.Write(&nVersion, sizeof(nVersion));
		}
		return bOpen;
	}
	return FALSE;
}

BOOL CDSWriteFile::WriteStocksData(StockUserInfo* pStock)
{
	if (pStock != NULL)
	{
		BOOL bInitFile = TRUE;
		if (m_fileStock.m_hFile == INVALID_HANDLE_VALUE)
		{	
			bInitFile = StartWriteStockData();
		}
		if (bInitFile)
		{
			m_fileStock.Write(pStock, sizeof(StockUserInfo));
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDSWriteFile::EndWriteStockData()
{
	if (m_fileStock.m_hFile != INVALID_HANDLE_VALUE)
		m_fileStock.Close();

	return TRUE;
}

BOOL CDSWriteFile::ReadStocksData(CStockManager* pManager)
{
	if (pManager == NULL)
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	CString strFileName;
	strFileName.Format(_T("%sstocks.ext"), m_strInitPath);
	CFile fileStock;
	if(fileStock.Open(strFileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary))
	{
		int nVersion = 0;
		int nReadSize = fileStock.Read(&nVersion, sizeof(int));
		if (nReadSize == sizeof(int) && nVersion == DSVERSION_CODELIST)
		{
			while (nReadSize != 0)
			{
				StockUserInfo* pStockUserNew = new StockUserInfo;
				memset(pStockUserNew, 0, sizeof(StockUserInfo));
				nReadSize = fileStock.Read(pStockUserNew, sizeof(StockUserInfo));
				if (nReadSize == sizeof(StockUserInfo))
				{
					CHSDSStockInfoIntact* pStockNew = new CHSDSStockInfoIntact;
					memset(pStockNew, 0, sizeof(CHSDSStockInfoIntact));
					memcpy(&pStockNew->m_stockInfo, pStockUserNew, sizeof(StockUserInfo));
					pManager->PushStock(pStockNew);
				}
				delete pStockUserNew;
			}

		}

		fileStock.Close();
		bRet = TRUE;
	}
	
	return bRet;
}

BOOL CDSWriteFile::WriteRequestFile(CString strFileName, void* pData, int nSize)
{
	BOOL bRet = FALSE;
	CString strPath;
	strPath.Format("%s%s", m_strSettingPath, strFileName);

	try
	{
		CFile file;
		if( file.Open(strPath, CFile::modeCreate | CFile::modeWrite) )
		{
			file.Write(pData, nSize);
			file.Close();
			bRet = TRUE;
		}
	}
	catch (CFileException* pEx)
	{
		pEx->Delete();
		bRet = FALSE;
	}
	catch(...)
	{
		bRet = FALSE;
	}
	return bRet;
}
BOOL CDSWriteFile::SetStocksFinanceData(CMapStringToPtr& mapFinanceData)
{
	BOOL bRet = FALSE;
	CString strFileName;
	strFileName.Format(_T("%s%s"), m_strSettingPath, CURRENT_FINANCE_FILE);
	CFile fileFinance;
	if(fileFinance.Open(strFileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary))
	{
		int nStockSize = 0;
		int nReadSize = fileFinance.Read(&nStockSize, sizeof(int));
		nReadSize = fileFinance.Read(&nStockSize, sizeof(int));
		while (nStockSize-- > 0)
		{
			int nOneDataSize = sizeof(FxjCaiWu) - 1;
			FxjCaiWu financeNew;
			memset(&financeNew, 0, nOneDataSize);
			nReadSize = fileFinance.Read(&financeNew, nOneDataSize);
			CString strCurCode = CString(financeNew.cCode, sizeof(financeNew.cCode));
			CurrentFinanceData* financeAdd = new CurrentFinanceData;
			memset(financeAdd, 0, sizeof(CurrentFinanceData));
			financeAdd->m_nDate = 1;
			memcpy(&financeAdd->m_fFinanceData, financeNew.fCwdata, sizeof(financeNew.fCwdata));
			memcpy(&financeAdd->m_ciStockCode.m_cCode, financeNew.cCode, sizeof(financeNew.cCode));
			mapFinanceData.SetAt(strCurCode, financeAdd);
		}
		fileFinance.Close();
		bRet = TRUE;
	}

	return bRet;
}
BOOL CDSWriteFile::ReadStocksFinanceData(CString strCodeComp, CArray<CurrentFinanceData>& ayFinanceData, int nTotal)
{
	if (strCodeComp.IsEmpty())
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	CString strFileName;
	strFileName.Format(_T("%s%s"), m_strSettingPath, CURRENT_FINANCE_FILE);
	CFile fileFinance;
	if(fileFinance.Open(strFileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary))
	{
		int nStockSize = 0;
		int nReadSize = fileFinance.Read(&nStockSize, sizeof(int));
		nReadSize = fileFinance.Read(&nStockSize, sizeof(int));
		while (nStockSize-- > 0)
		{
			int nOneDataSize = sizeof(FxjCaiWu) - 1;
			FxjCaiWu financeNew;
			memset(&financeNew, 0, nOneDataSize);
			nReadSize = fileFinance.Read(&financeNew, nOneDataSize);
			CString strCurCode = CString(financeNew.cCode, sizeof(financeNew.cCode));
			if(strCodeComp.Find(strCurCode) >= 0)
			{
				CurrentFinanceData financeAdd;
				memset(&financeAdd, 0, sizeof(CurrentFinanceData));
				financeAdd.m_nDate = 1;
				memcpy(&financeAdd.m_fFinanceData, financeNew.fCwdata, sizeof(financeNew.fCwdata));
				memcpy(&financeAdd.m_ciStockCode.m_cCode, financeNew.cCode, sizeof(financeNew.cCode));
				ayFinanceData.Add(financeAdd);
			}
			if (ayFinanceData.GetSize() >= nTotal)	//查完所有，结束
			{
				break;
			}
		}
		fileFinance.Close();
		bRet = TRUE;
	}

	return bRet;
}

BOOL CDSWriteFile::ReadStocksExrightData(CString strCodeComp, CArray<CurrentExRightData>& ayExrightData, int nTotal)
{
	if (strCodeComp.IsEmpty())
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	CString strFileName;
	strFileName.Format(_T("%s%s"), m_strSettingPath, EX_RIGHT_FILE);
	CFile fp;
	CFileException pError;
	if( fp.Open(strFileName,
		CFile::shareDenyNone|CFile::modeReadWrite|CFile::typeBinary,&pError) )
	{
		int nLen = fp.GetLength();
		if( nLen <= 0)
			return 0;

		char* pData = new char[nLen];
		fp.Read(pData,sizeof(char)*nLen );
		fp.Close();

		FxjChuQuan*         pFxj;
		HSExRightItem* pFxjDataItem;
	
		int   nCountPos = 8+4;
		char* ppData = (char*)&pData[nCountPos];

		CString strName;
		CTime   date;
		CString strDate;
		while( nCountPos < nLen && *(int*)ppData != -1 ) //20090620 YJT 调整条件顺序，防止内存读越界
		{
			pFxj = (FxjChuQuan*)ppData;
			nCountPos += (sizeof(FxjChuQuan)-sizeof(char));
			ppData = (char*)&pFxj->cNext[0];
			strName = (char*)&pFxj->cCode[2];
			CString strMarket = (char*)&pFxj->cCode[0];

			CArray<HSExRightItem*> ayCurExright;
			for( ; nCountPos < nLen && *(int*)ppData != -1 ; ) //20090620 YJT 调整条件顺序，防止内存读越界
			{
				pFxjDataItem = (HSExRightItem*)ppData;
				ppData += sizeof(HSExRightItem);
				nCountPos += sizeof(HSExRightItem);

				ayCurExright.Add(pFxjDataItem);
			}
			ppData += sizeof(int);
			nCountPos += sizeof(int);

			if (strCodeComp.Find(strName) >= 0)
			{
				int nItemSize = ayCurExright.GetSize();
				int nExrightDataLen = sizeof(CodeInfo) + nItemSize * sizeof(HSExRightItem) + sizeof(long) + 1;
				CurrentExRightData exrightData;
				exrightData.Init(nExrightDataLen);
				CodeInfo codeSet;
				strncpy(codeSet.m_cCode, strName, min(sizeof(codeSet.m_cCode), strName.GetLength()));
				exrightData.SetCode(&codeSet);
				exrightData.SetCount(nItemSize);
				for(int i = 0; i < nItemSize; i ++)
				{
					HSExRightItem* pItem = ayCurExright.GetAt(i);					
					exrightData.SetExRightDataItem(pItem, i);					
				}
				ayExrightData.Add(exrightData);
				if (ayExrightData.GetSize() >= nTotal)
				{
					bRet = TRUE;
					break;
				}
			}
		}
		delete[] pData;
	}
	return bRet;
}