#include "StdAfx.h"
#include ".\mapviewfile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 
CMapViewFile::CMapViewFile()
{
	m_pMapViewStart = NULL;
	m_dwMapViewSize = 0;
	m_dwDefaultExtendSize = 1024 * 1024 * 8;
}
CMapViewFile::CMapViewFile(DWORD dwDefaultExtendSize)
{
	m_pMapViewStart = NULL;
	m_dwMapViewSize = 0;
	m_dwDefaultExtendSize = dwDefaultExtendSize;
}

CMapViewFile::~CMapViewFile(void)
{
	if (m_pMapViewStart != NULL)
	{
		UnmapViewOfFile(m_pMapViewStart);
		m_pMapViewStart = NULL;
	}
}

BOOL CMapViewFile::CreateMultipleDirectory(char * sDirectory)
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

BOOL CMapViewFile::MakeFile(CString strFileName)
{
	CFileFind  findFind; 
	if ( strFileName.IsEmpty())
		return FALSE;
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

		CFile file;
		file.Open(strFileName,CFile::modeCreate);
		file.Close();
	}
	return TRUE;
}


BOOL CMapViewFile::Open(CString strFileName,DWORD dwExpandSize)
{
	CriticalGuard guard(&m_critical);

	m_strFileName = strFileName;

	
	//创建文件
	if(!MakeFile(m_strFileName))
		return FALSE;

	if (m_pMapViewStart != NULL)
	{
		UnmapViewOfFile(m_pMapViewStart);
		m_pMapViewStart = NULL;
	}

	HANDLE hFile = CreateFile(m_strFileName,GENERIC_WRITE|GENERIC_READ,0,NULL
		,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		//printf("File could not be opened.");
		return FALSE;
	}

	//创建映射
	DWORD dwFileSize = GetFileSize(hFile,NULL);
	if(dwFileSize < 1000)
		dwFileSize = dwExpandSize;
	m_dwMapViewSize = dwFileSize;
	HANDLE hFileMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,
		m_dwMapViewSize+sizeof(char),NULL);

	
	if(hFileMap != NULL)
	{
		m_pMapViewStart = MapViewOfFile(hFileMap,FILE_MAP_WRITE,0,0,0);
		if (!m_pMapViewStart)
		{
			int nErro = GetLastError();
			CString strErroInfo("文件映射建立失败");
			strErroInfo += strFileName;
			TRACE(strErroInfo);
			CString strErrorNo;
			strErrorNo.Format("错误号：%d",nErro);
			TRACE(strErrorNo);

		}
	}
	if(hFileMap != NULL)
		CloseHandle(hFileMap);
	if(hFile != NULL)
		CloseHandle(hFile);

	return TRUE;
}	

LPVOID CMapViewFile::Read(DWORD dwOffset)
{
	if (m_pMapViewStart == NULL)
		return NULL;

	while (dwOffset > m_dwMapViewSize)
		ExpandFile(m_dwDefaultExtendSize);

	char* pStart = (char*)m_pMapViewStart;
	return pStart + dwOffset;
}

BOOL CMapViewFile::Write(DWORD dwOffset,LPVOID pBuf,DWORD dwLength)
{
	if (m_pMapViewStart == NULL)
		return FALSE;
	
	while (dwOffset + dwLength > m_dwMapViewSize)
		ExpandFile(m_dwDefaultExtendSize);
	if (pBuf == NULL)
		return FALSE;

	memcpy((char *)m_pMapViewStart + dwOffset, pBuf, dwLength);
	return TRUE;
}

BOOL CMapViewFile::Clear(DWORD dwOffset,DWORD dwLength)
{
	if (m_pMapViewStart == NULL)
		return FALSE;
	if (m_pMapViewStart != NULL)
		memset((char *)m_pMapViewStart + dwOffset, 0, dwLength);
	return TRUE;
}


BOOL CMapViewFile::ExpandFile(DWORD dwExpandSize)
{
	DWORD dOffsize = dwExpandSize;
	CriticalGuard guard(&m_critical);

	if (m_pMapViewStart != NULL)
	{
		UnmapViewOfFile(m_pMapViewStart);
		m_pMapViewStart = NULL;
	}

	HANDLE hFile = CreateFile(m_strFileName,GENERIC_WRITE|GENERIC_READ,0,NULL
		,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		return FALSE;
	} 

	//创建映射
	DWORD dwFileSize = GetFileSize(hFile,NULL);
	if(dwFileSize < 1000)
		dwFileSize = dwExpandSize;
	m_dwMapViewSize = dwFileSize;
	HANDLE hFileMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,
		m_dwMapViewSize + dwExpandSize + sizeof(char),NULL);
	m_dwMapViewSize += dwExpandSize + sizeof(char);//更新映射大小
	
	
	if(hFileMap != NULL)
	{
		//m_pMapViewStart = MapViewOfFile(hFileMap,FILE_MAP_WRITE,(DWORD)(dwExpandSize>>32), (DWORD)(dwExpandSize&0xFFFFFFFF),dwExpandSize);
		m_pMapViewStart = MapViewOfFile(hFileMap,FILE_MAP_WRITE,0,0,0);
	}

	if(hFileMap != NULL)
		CloseHandle(hFileMap);
	if(hFile != NULL)
		CloseHandle(hFile);

	//LeaveCriticalSection
	return TRUE;
}
void CMapViewFile::UnMapFile()
{
	if (m_pMapViewStart != NULL)
	{
		UnmapViewOfFile(m_pMapViewStart);
		m_pMapViewStart = NULL;
	}
}
