#include "StdAfx.h"
#include "TrendFile.h"
#include "GeneralHelper.h"
#include "WinnerApplication.h"
#include "DataSourceImpl.h"
#include "MapViewFile.h"
extern char* GetMarketName(int cBourse,char* strRet,int nLen);
extern CDataSourceImpl* ds_pi_DataSourceImpl;
extern void CreatePath(CString);
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 
void GetTrendName(CTrendFile::TrendDataType TrenType,CodeInfo* pCode, char* strRet, int nLen)
{
	char strMarket[_MAX_PATH];
	memset(strRet,0,sizeof(strRet));
	GetMarketName(pCode->m_cCodeType,strRet,sizeof(strMarket));
	switch (TrenType)
	{
	case CTrendFile::HisTrace:
		{
			CString strPath;
			strPath.Format("histrend\\%s.ten", pCode->GetCode());
			strncpy(strRet,strPath.GetBuffer(),nLen);	
			break;
		}
	case CTrendFile::NewTrace:
		{
			CString strPath;
			strPath.Format("tend\\%s.fen", strRet);
			strncpy(strRet,strPath.GetBuffer(),nLen);	
			break;
		}
	case CTrendFile::Tick:
		{
			CString strPath;
			strPath.Format("tick\\%s.tik", pCode->GetCode());
			strncpy(strRet,strPath.GetBuffer(),nLen);	
			break;
		}
	default:
		break;
	}
}
CTrendFile::CTrendFile(void)
{
	m_pFile = NULL;
	m_strBuffer = NULL;
}

CTrendFile::~CTrendFile(void)
{
	if ( m_strBuffer && !IsBadReadPtr(m_strBuffer,1))
		delete[] m_strBuffer;
	m_strBuffer = NULL;
	if ( m_pFile && !IsBadReadPtr(m_pFile,1) )
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}
}

BOOL CTrendFile::Open(CodeInfo* pCode,TrendDataType TrenType)
{
	if (pCode)
	{
		m_nTrendType = TrenType;
		//取文件路径
		CString strDayBasePath =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::DATA_PATH);
		char strRet[_MAX_PATH];
		memset(strRet,0,sizeof(strRet));
		GetMarketName(pCode->m_cCodeType,strRet,sizeof(strRet));

		char strFile[_MAX_PATH];
		memset(strFile,0,sizeof(strFile));
		GetTrendName(TrenType,pCode,strFile,sizeof(strFile));
		
		CString filename;
		filename.Format("%s%s\\%s", strDayBasePath, strRet,strFile);
		CSingleLock lock(&m_mutex);
		if ( lock.Lock())
		{
			try
			{
				if ( MakeFile(filename) )
				{
					if (m_pFile == NULL)
					{
						m_pFile   = new CFile;
					}
					if(	!m_pFile->Open(filename,  ( CFile::modeCreate |CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate|CFile::typeBinary)))
					{
						delete m_pFile;
						m_pFile = NULL;		
						lock.Unlock();
						return FALSE;
					}
				}	
				switch (TrenType)
				{
				case HisTrace:   //历史分时
					{
						m_Head.m_lVersion = 2011;
						m_Head.m_nIndexSize= 250; //默认存放250天
						m_Head.m_lCount  = 0;
						m_Head.m_nDataSize = sizeof(StockCompHistoryData);   //
						break;
					}
				case NewTrace:    //分时走势
					{
						m_Head.m_lVersion = 2011;
						m_Head.m_nIndexSize= 0; //对应市场的代码数
						m_Head.m_lCount  = 0;
						m_Head.m_nDataSize = sizeof(PriceVolItem_Ext);   //
						break;
					}
				case Tick:        //分笔
					{
						m_Head.m_lVersion = 2011;
						m_Head.m_nIndexSize= 1; //对应市场的代码数
						m_Head.m_lCount  = 0;
						m_Head.m_nDataSize = sizeof(StockTick);   //
						break;
					}
				}

			}
			catch(...)
			{
				lock.Unlock();
				return FALSE;
			}
			lock.Unlock();
		}
		
		
		//InitIndex();
		return TRUE;
	}
	return FALSE;
}

BOOL CTrendFile::InitIndex()
{
	if(m_pFile == NULL)
	{
		return FALSE;		
	}
	CSingleLock lock(&m_mutex);
	if ( lock.Lock())
	{
		TRY
		{
			if (m_strBuffer)
				delete[] m_strBuffer;
			m_pFile->SeekToBegin();
			CTrendFileHead tagHead;
			memset(&tagHead,0,sizeof(tagHead));
			// 读取通用文件头
			m_pFile->SeekToBegin();
			m_pFile->Read(&tagHead, sizeof(CTrendFileHead));
			if (tagHead.m_lVersion >0 || tagHead.m_nIndexSize >0)
				memcpy(&m_Head, &tagHead, sizeof(CTrendFileHead));
			long nSize = m_Head.m_nIndexSize*sizeof(CTrendDataIndex);
			m_strBuffer = new char[nSize];
			memset(m_strBuffer,0,nSize);
			m_pFile->Seek(sizeof(CTrendFileHead), CFile::begin);
			m_pFile->Read(m_strBuffer,nSize);
			lock.Unlock();
			return TRUE;
		}
		CATCH(CFileException, pEx)
		{	
			pEx->Delete();
		}
		AND_CATCH(CMemoryException, pEx)
		{
			pEx->Delete();
		}
		END_CATCH;
		lock.Unlock();
	}
	
	return FALSE;
}

BOOL CTrendFile::InitIndex(HSMarketDataType nMarket)
{
	if(m_pFile == NULL)
	{
		return FALSE;		
	}
	CSingleLock lock(&m_mutex);
	if ( lock.Lock())
	{
		TRY
		{
			if (m_strBuffer)
				delete[] m_strBuffer;
			m_pFile->SeekToBegin();
			CTrendFileHead tagHead;
			memset(&tagHead,0,sizeof(CTrendFileHead));
			// 读取通用文件头
			m_pFile->SeekToBegin();
			m_pFile->Read(&tagHead, sizeof(CTrendFileHead));
			if (tagHead.m_lVersion >0 && tagHead.m_nIndexSize >0 && tagHead.m_nDataSize >0)
				memcpy(&m_Head, &tagHead, sizeof(CTrendFileHead));
			CArray<StockType> blockArray;
			unsigned short nCodeType = nMarket;
			if ( m_Head.m_nIndexSize == 0 )
			{
				if ( ds_pi_DataSourceImpl->HSDataSourceEx_GetSystemMarketInfo(&nCodeType, 1,blockArray) >0)
					m_Head.m_nIndexSize = blockArray.GetAt(0).m_nTotal;
				else
					m_Head.m_nIndexSize = DEFAULTTRENINDEXCOUNT;
			}
			if ( m_Head.m_nIndexSize <= 0)
				m_Head.m_nIndexSize = 2000;

			long nSize = m_Head.m_nIndexSize*sizeof(CTrendDataIndex);
			m_strBuffer = new char[nSize];
			memset(m_strBuffer,0,nSize);
			m_pFile->Seek(sizeof(CTrendFileHead), CFile::begin);
			m_pFile->Read(m_strBuffer,nSize);
			lock.Unlock();
			return TRUE;
		}
		CATCH(CFileException, pEx)
		{	
			pEx->Delete();
		}
		AND_CATCH(CMemoryException, pEx)
		{
			pEx->Delete();
		}
		END_CATCH;
		lock.Unlock();
	}
	
	return FALSE;
}

char* CTrendFile::Read(long lDate, long& nSize,CodeInfo* pCode)
{
	if ( m_pFile ==NULL || m_strBuffer == NULL)
	{
		return NULL;
	}
	CSingleLock lock(&m_mutex);
	char* retBuffer = NULL;
	if (lock.Lock())
	{
		try
		{
			//获取索引
			char* pTempIndex =m_strBuffer;
			CTrendDataIndex* pTrendIndex = NULL;
			BOOL bFind = FALSE;
			CTrendDataIndex* pIndex = NULL;
			for ( int i=0; i<m_Head.m_lCount; i++)
			{
				pTrendIndex = (CTrendDataIndex*)pTempIndex;
				//分时走势 增加比较代码
				if ( m_nTrendType == NewTrace && pTrendIndex &&  !IsBadReadPtr(pTrendIndex,sizeof(CTrendDataIndex))
					   && pTrendIndex->m_code.CompareCode(pCode) && pTrendIndex->m_lDate == lDate)
				{
					bFind = TRUE;
					break;
				}
				if ( m_nTrendType != NewTrace && pTrendIndex && pTrendIndex->m_lDate == lDate)
				{
					bFind = TRUE;
					break;
				}
				pTempIndex += sizeof(CTrendDataIndex);
			}
			if (bFind && pTrendIndex)
			{

				DWORD dwDataSize = pTrendIndex->m_lDataSize;  //数据块大小			
				DWORD dwFileSet = pTrendIndex->m_lDataPosition; //数据内容位置
				retBuffer = new char[dwDataSize]; 
				m_pFile->Seek(dwFileSet, CFile::begin);
				m_pFile->Read(retBuffer, dwDataSize);
				nSize = dwDataSize;				
			}

		}catch(...)
		{
			lock.Unlock();
			return NULL;
		}
		lock.Unlock();
	}	
	return retBuffer;
}

BOOL CTrendFile::Write(char* pData, long lSize, long lDate, CodeInfo* pCode)
{
	if (pData == NULL || m_pFile == NULL || m_strBuffer == NULL)
		return FALSE;

	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		try
		{
			char* pTempIndex = m_strBuffer;
			CTrendDataIndex* pIndex = NULL;
			int lindexCount = m_Head.m_lCount;
			BOOL bFind = FALSE;
			for ( int i=0; i<m_Head.m_lCount; i++)
			{
				pIndex = (CTrendDataIndex*)pTempIndex;
				if (pIndex && pIndex->m_lDataSize >0)
				{
					//分时走势 比较代码
					if ( m_nTrendType == NewTrace && pIndex->m_code.CompareCode(pCode) /*&& pIndex->m_lDate == lDate*/)
					{
						bFind = TRUE;
						break;
					}
					if ( m_nTrendType != NewTrace && pIndex->m_lDate == lDate)
					{
						bFind = TRUE;
						break;
					}
				}
				pTempIndex += sizeof(CTrendDataIndex);
			}
			//该日期不存在 
			if ( !bFind )
			{
				if (   m_nTrendType == Tick  )
					pIndex = (CTrendDataIndex*)m_strBuffer;
				else
				{
					if (m_Head.m_nIndexSize > lindexCount)
					{
						pIndex = (CTrendDataIndex*)(m_strBuffer + 
							lindexCount * sizeof(CTrendDataIndex));
					}
					else
					{		
						return FALSE;
						//pIndex = new CTrendDataIndex;
					}
				}					
				m_Head.m_lCount ++;
				memcpy((void*)&pIndex->m_code,(void*)pCode, sizeof(CodeInfo));
				DWORD dwOffset = sizeof(CTrendFileHead) + sizeof(CTrendDataIndex)*m_Head.m_nIndexSize; //文件头+索引
				DWORD dwDataIndex = (m_Head.m_lCount-1)*m_Head.m_nDataSize*EVERYTRENDSIZE;   //数据块得偏移位置  tick数据偏移为0

				dwDataIndex += dwOffset;
				pIndex->m_lDataPosition =  dwDataIndex;
				//如果是分笔 清空前面天数的数据
				if ( m_nTrendType == Tick )
				{
					m_Head.m_lCount=1;					
					pIndex->m_lDataPosition =  dwOffset;
				}
			}
			if (m_nTrendType == Tick )
				m_pFile->SetLength(0);
			pIndex->m_lDate = lDate;
			pIndex->m_lDataSize = lSize;
			DWORD dwOffset = pIndex->m_lDataPosition;
			//写数据内容
			m_pFile->Seek(dwOffset, CFile::begin);
			m_pFile->Write(pData, lSize);	

			//写文件头
			m_pFile->SeekToBegin();
			m_pFile->Write(&m_Head, sizeof(CTrendFileHead));
			m_pFile->Seek(sizeof(CTrendFileHead), CFile::begin);
			m_pFile->Write(m_strBuffer, m_Head.m_nIndexSize*sizeof(CTrendDataIndex));

		}
		catch(...)
		{
			lock.Unlock();
			return FALSE;
		}
		lock.Unlock();
	}	
	return TRUE;
}
BOOL CTrendFile::MakeFile(CString strFileName)
{
	CFileFind  findFind; 
	//文件不存在
	if(!findFind.FindFile(strFileName))
	{
		//创建目录
		char sPath[1024] = {0};
		strcpy(sPath, strFileName);
		for (size_t i = strlen(sPath) - 1; i >= 0; i--)
		{
			if (sPath[i] == '\\' || sPath[i] == '/')
			{
				sPath[i] = '\0';
				break;
			}
		}
		if (strlen(sPath) > 0 && strlen(sPath) != strlen(strFileName))
			if (!CreateMultipleDirectory(sPath))
				return FALSE;
	}
	return TRUE;
}
BOOL CTrendFile::CreateMultipleDirectory(char * sDirectory)
{   
	HANDLE fFile; //   File   Handle   
	WIN32_FIND_DATA fileinfo; //   File   Information   Structure   
	char sTemp[1024] = {0}; //   Temporary   Cstring   Object   

	//如果目录已存在则返回成功
	fFile =  FindFirstFile(sDirectory, &fileinfo);   
	if(fileinfo.dwFileAttributes   ==   FILE_ATTRIBUTE_DIRECTORY)   
	{   
		FindClose(fFile);   
		return TRUE;   
	}   

	for(size_t i = 0; i < strlen(sDirectory); i++)
	{   
		if(sDirectory[i] == '\\' || sDirectory[i] == '/')
		{
			CreateDirectory(sTemp, NULL);
		}   
		sTemp[i] = sDirectory[i];
		if(i == strlen(sDirectory)-1) //   If   we   reached   the   end   of   the   file   add   the   remaining   string   
		{
			CreateDirectory(sTemp, NULL);
		}
	}   

	//判断目录是否创建成功
	fFile = FindFirstFile(sDirectory, &fileinfo);   
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)   
	{   
		FindClose(fFile);   
		return   TRUE;   
	}   
	else   
	{   
		FindClose(fFile);   
		return   FALSE;   
	}   
}   

BOOL CTrendFile::DelFile(CString strParent)
{
	CFileFind   f;

	BOOL   bFind   =   f.FindFile(strParent   +   "*.* ");
	while(bFind)
	{
		bFind   =   f.FindNextFile();
		if(f.IsDots())   continue;
		if(f.IsDirectory())
		{
			DelFile(f.GetFilePath()+"\\");
		}
		else if ( f.GetFileName().Find(_T("fen")) >0 || f.GetFileName().Find(_T("tik")) > 0)
		{
			TRACE("\r\n del filename is %s", f.GetFileName());
			DeleteFile(strParent   +   f.GetFileName());
		}
	}
	return TRUE;
}